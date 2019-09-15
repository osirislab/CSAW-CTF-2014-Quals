#ifndef __MACH__
#define _GNU_SOURCE 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <err.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <grp.h>
#include <stdarg.h>

#ifdef _SEED_
#include <time.h>
#endif

#ifdef IPV6
#define MY_AF AF_INET6
#define sockaddr_t struct sockaddr_in6
#else
#define MY_AF AF_INET
#define sockaddr_t struct sockaddr_in
#endif

// You will need to define the variables below in your service
extern const char *USER;
extern const int LPORT;

// Service setup functions
int socketListen(unsigned short);
void loop(int fd, int (*childHandler)(int));
int drop_privs_user(const char *user);
int drop_privs(struct passwd *user);
void alarm_handler(void);

// main child handler
int handleConnection(int);

// socket read and write functions
ssize_t sendAll(int, char *, size_t);
ssize_t sendMsg(int, char *);
ssize_t sendMsgf(int, const char *, ...);
ssize_t readAll(int sock, char *buf, size_t len);
ssize_t readUntil(int sock, char *buf, size_t len, char sentinal);
