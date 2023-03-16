// --------- includes -------------
#include <stdint.h>     // typedef uint8_t, UINT8_MAX
#include <sys/types.h>  // ssize_t, uint8_t
#include <fcntl.h>      // open, O_RDONLY
#include <unistd.h>     // read, write
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit
#include <string.h>     // strcmp
// --------- defines ---------------
#define MAXLINE 76
#define INBUFFSIZE 3
#define OUTBUFFSIZE 4
// Check that uint8_t type exists
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif
// ---------- globals ---------------
static char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/=";

// ----- function declarations ------
void readin(int fd);

int main(int argc, char* argv[]) {
  int fd = STDIN_FILENO;

  // invalid number of arguments -- can only decode 1 file
  if (argc > 2){
    printf("error: invalid number of arguments. Can only encode one file at a time.\n");
    exit(-1);
  }

  /* ----------------- Get Input Descriptor ----------------- */
  if((argc > 1) && (strcmp(argv[1], "-") != 0)) {
    fd = open(argv[1], O_RDONLY);
    if (fd < 0){
      perror("error");  // invalid file descriptor
      exit(-1);
    }
  }

  /* ------------------------ Process ------------------------ */
  readin(fd);   
  return 0;
}

/* readin: reads data from input_fd and stores it in inBuffer */
void readin(int fd_in){
  ssize_t nread, nwrite;
  int count = 0, i;
  uint8_t inBuf[INBUFFSIZE], outBuf[OUTBUFFSIZE];

  /* -------------------------- Read -------------------------- */
  while((nread = read(fd_in, inBuf, INBUFFSIZE)) != 0){
    if(nread < 0){
      perror("error");  // invalid file descriptor
      exit(-1);
      }

    /* -------------------- Encode algorithm -------------------- */
    // upper 6 bits of byte 0
    outBuf[0] = alphabet[inBuf[0] >> 2];    
    // lower 2 bits of byte 0, shift left and or with the upper 4 bits of byte 1
    outBuf[1] = alphabet[((inBuf[0] & 0x03) << 4) | (inBuf[1] >> 4)];
    // lower 4 bits of byte 1, shift left and or with upper 2 bits of byte 2
    outBuf[2] = alphabet[((inBuf[1] & 0x0F) << 2) | (inBuf[2] >> 6)];
    // lower 6 bits of byte 2
    outBuf[3] = alphabet[inBuf[2] & 0x3F];  

    for(i = INBUFFSIZE; i > nread; i--){
      outBuf[i] = alphabet[64];             // pad output if less than 3 bytes
    }

    /* -------------------------- Write -------------------------- */
    for (size_t offset = 0; offset < OUTBUFFSIZE;){
      if ((nwrite = write(STDOUT_FILENO, offset + (char *) outBuf, OUTBUFFSIZE - offset)) < 0){
        perror("error");       
        exit(-1);
      }

      offset += nwrite;
      count += nwrite;
      
      // write new line every 76 characters
      if (count % MAXLINE == 0){
        write(STDOUT_FILENO, "\n", sizeof(char));
      }
    }

    memset(inBuf, 0, INBUFFSIZE);             // sanitze input buffer
    memset(outBuf, 0, OUTBUFFSIZE);           // sanitze output buffer
  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
}



