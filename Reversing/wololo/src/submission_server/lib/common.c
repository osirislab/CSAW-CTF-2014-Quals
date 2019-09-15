#include "common.h"

int main(int argc, char **argv) {
    
	(void)argc;
	(void)argv;
    
	int fd = 0;
    
#if defined(_SEED_)
	srand(time(0));
#endif

	fd = socketListen(LPORT);
	signal(SIGALRM, alarm_handler);
	loop(fd, handleConnection);
    
	return 0;
}

int socketListen(unsigned short port) {
	int fd = 0;
	int optval;
	sockaddr_t addr;

#ifdef IPV6
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(port);
	addr.sin6_addr = in6addr_any;
#else
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif

	// Ignore children so they disappear
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) 
		err(-1, "Unable to set SIGCHLD handler");

	// Create socket
	fd = socket(MY_AF, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1)
		err(-1, "Unable to create socket");
    
	// Set reuse
	optval = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		err(-1, "Unable to set reuse");

	// Set accepting v4 connections on v6 sockets
#ifdef IPV6
#ifdef IPV6_DUAL_STACK
	optval = 0;
#else
	optval = 1;
#endif
	if(setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(int)) == -1)
		err(-1, "Unable to set dual stack behavior");
#endif
    
	// Bind to socket
	if (bind(fd, (struct sockaddr *)&addr, sizeof(sockaddr_t)) == -1)
		err(-1, "Unable to bind socket");
    
	// Listen for new connections
	if (listen(fd, 20) == -1)
		err(-1, "Unable to listen on socket");
    
	return fd;	
}

void alarm_handler(void)
{
	exit(0);
}

void loop(int fd, int (*childHandler)(int)) {
	int client;
	int status;
	int one = 1;
	pid_t child;

    
	while (one) {
		client = accept(fd, NULL, NULL);
        
		if (client == -1)
			continue;
        
		child = fork();
		if (child == -1)
			continue;

		alarm(30);
        
		if (child == 0) { // child
			drop_privs_user(USER);
			close(fd);
            
			status = childHandler(client);
            
			close(client);
			exit(status);
		} else { // parent
			close(client);
		}
	}
    
	return;
}

int drop_privs_user(const char *user) {
	struct passwd *pwentry;
    
#if !defined(RELEASE)
	return 0;
#endif
    
	pwentry = getpwnam(user);
	if (pwentry == 0)
		err(-1, "Failed to find user %s", user);

	if (drop_privs(pwentry) == -1)
		err(-1, "drop_privs failed!");
    
	return 0;
}

int drop_privs(struct passwd *user) {
	int gid;
	int uid;
	int status;
	char *home;
    
	uid = user->pw_uid;	
	gid = user->pw_gid;	
	home = user->pw_dir;
	
#ifdef CHROOT
	if (chroot(home) == -1)
		err(-1, "chroot failed");
#else	
	if (chdir(home) == -1) {
		status = -1;
		goto ret;
	}
#endif
    
	if (setgroups(0, NULL) == -1) {
		status = -1;
		goto ret;
	}
    
	if (setgid(gid) == -1) {
		status = -1;
		goto ret;
	}
    
	if (setuid(uid) == -1) {
		status = -1;
		goto ret;
	}
    
	status = 0;
    
ret:
	return status;
}

ssize_t sendAll(int sock, char *buf, size_t len) {
	size_t i = 0;
	ssize_t size = 0;
    
	while (len > i) {
		size = send(sock, buf + i, len - i, 0);
		if (size < 1)
			return -1;
		i += size;
	}
    
	return (int)i;
}

ssize_t sendMsg(int sock, char *msg) {
	return sendAll(sock, msg, strlen(msg));
}


ssize_t sendMsgf(int sock, const char *format, ...) {
	ssize_t status = 0;
	va_list list;
	char *ptr = NULL;
	
	va_start(list, format);
	status = vasprintf(&ptr, format, list);
	va_end(list);
    
	if (status == -1)
		goto ret;
    
	status = sendMsg(sock, ptr);
    
ret:
	free(ptr);
	return status;
}

ssize_t readAll(int sock, char *buf, size_t len) {
    ssize_t count = 0;
    ssize_t remaining = len;
    
    if (len > 0) {
        while (remaining > 0) {
            count = recv(sock, buf, remaining, 0);
            
            if (count == -1) {
                perror("readAll");
                break;
            }
            
			buf += count;
            remaining -= count;
        }
    }
    
    return len - remaining;
}

ssize_t readUntil(int sock, char *buf, size_t len, char sentinal) {
    char _buf;
    size_t i = 0;
    ssize_t count;
    
    if (len > 0) {
        for (i = 0; i < len; i++) {
            count = recv(sock, &_buf, 1, 0);
            
            if (count == -1) {
                perror("readUntil");
                break;
            }
            
            buf[i] = _buf;
            
            if (_buf == sentinal)
                break;
        }
    }
    
    return i;
}




