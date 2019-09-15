#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

unsigned int getSeed() {
  unsigned int seed;
  int fd;
  fd = open("/dev/urandom", O_RDONLY);
  if(fd == -1) {
    perror("open");
    return 0;
  }
  if(read(fd, &seed, sizeof(seed)) < 0) {
    perror("read");
    return 0;
  }
  if(close(fd)) {
    perror("close");
  }
  return seed;
}

extern int fillChallengeResponse(unsigned int* challenges, unsigned int* responses) {
  unsigned int seed = getSeed();
  int ii;
  if(seed == 0) {
    return -1;
  }
  srand(seed);
  for(ii = 0; ii < 8; ++ii) {
    challenges[ii] = rand() % UINT_MAX;
  }
  seed = getSeed();
  if(seed == 0) {
    return -1;
  }
  for(ii = 0; ii < 8; ++ii) {
    responses[ii] = rand() % UINT_MAX;
  }
  return 0;
}
