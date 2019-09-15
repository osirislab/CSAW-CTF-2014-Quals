#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#include "challengeresponse.h"

// In order to read the flag, all challengs must have the proper reponse given.
int locks[8] = { 0 };
const char* flag = "flag.txt";

unsigned int chall_resp[16];
unsigned int* challenges = &chall_resp[0];
unsigned int* responses = &chall_resp[8];

// If all locks are set to nonzero numbers, then unlocked is true.
int checkUnlocked() {
  int unlocked = 1;
  int ii;
  for (ii = 0; ii < 8; ++ii) {
    unlocked *= locks[ii];
  }
  return unlocked;
}

// Read one char from stdin.
char myGetchar() {
  char opcodes[1];
  read(fileno(stdin), opcodes, 1);
  char ret = opcodes[0];
  return ret;
}

// Print the desired "challenge" from memory to stdout.
void getChallenge(char opcode) {
  unsigned char index = opcode & 0x0F;
  unsigned int challenge = challenges[index];
  char* challchar = (char*)&challenge;
  printf("%c%c%c%c", challchar[0], challchar[1], challchar[2], challchar[3]);
  fflush(stdout);
}

// Read a response from stdin, if it matches the desired response in memory,
// then set the lock to unlocked. Otherwise, then just exit.
void getResponse(char opcode) {
  unsigned char index = opcode & 0x0F;
  unsigned int response = responses[index];
  unsigned int buffer;
  read(STDIN_FILENO, (void*)&buffer, 4);
  unsigned int user_response = buffer;
  if(user_response != response) {
    exit(0);
  }
  locks[index] = 1;
}

// Read file to stdout.
void cat(FILE* fp) {
  char buffer[4096];
  size_t nbytes;
  while((nbytes = fread(buffer, sizeof(char), sizeof(buffer), fp)) != 0) {
    fwrite(buffer, sizeof(char), nbytes, stdout);
  }
}

void accessGranted() {
  return;
}

void accessDenied() {
  exit(1);
}

// Checks if unlock is set, if so, then read the flag to stdout. Otherwise, just
// quit.
void readFlagIfUnlocked() {
  if(checkUnlocked()) {
    accessGranted();
    FILE* fp;
    if((fp = fopen(flag, "r")) != 0) {
      cat(fp);
      fflush(stdout);
      fclose(fp);
      fflush(stdout);
    }
  } else {
    accessDenied();
    fflush(stdout);
  }
}

// Main loop.
int main(void) {
  if (fillChallengeResponse(challenges, responses) == -1) {
    exit(1);
  }
  int i;
  printf("CSAW ChallengeResponseAuthenticationProtocol Flag Storage\n");
  fflush(stdout);
  // We want an infinite loop, but there's some weird stuff with optimizations,
  // so I have to do this hack ;)
  for(i = 0; i < 1; ++i) {
    char opcode = myGetchar();
    switch(opcode & 0xF0) {
      case 0xA0:
        getChallenge(opcode);
        i -= 2;
        break;
      case 0xE0:
        getResponse(opcode);
        i -= 2;
        break;
      case 0x80:
        readFlagIfUnlocked();
        break;
    }
    continue;
  }
  return 0;
}
