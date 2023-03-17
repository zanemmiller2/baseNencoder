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
#define INBUFFSIZE64 3
#define OUTBUFFSIZE64 4
#define INBUFFSIZE32 5
#define OUTBUFFSIZE32 8
// Check that uint8_t type exists
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif
// ---------- globals ---------------
static char const alphabet64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char const alphabet32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";

// ----- function declarations ------
int encodeBase64(int fd);
void encodeBase32(int fd);

int main(int argc, char* argv[]) {
  int fd = STDIN_FILENO;
  char* option;
  char* baseN = "64";
  char* file_name = "-";
  int i;

  // invalid number of arguments -- can only decode 1 file
  if (argc > 4) {
    printf("error: invalid number of arguments\n");
    exit(-1);
  }

  // option, baseN, and fileName only
  if (argc == 4) {
    if (strcmp(argv[1], "-n") == 0 && (strcmp(argv[2], "32") == 0 || strcmp(argv[2], "64") == 0)) {
      option = argv[1];
      baseN = argv[2];
      file_name = argv[3];
    }
    else {
      printf("error: invalid arguments\n");
      exit(-1);
    }
  }
  // option, baseN only
  else if (argc == 3) {
    if (strcmp(argv[1], "-n") == 0 && (strcmp(argv[2], "32") == 0 || strcmp(argv[2], "64") == 0)) {
      option = argv[1];
      baseN = argv[2];
    }
    else {
      printf("error: invalid arguments\n");
      exit(-1);
    }
  }
  // file_name only
  else if (argc == 2) {
    if (strcmp(argv[1], "-n") == 0) {
      printf("error: missing baseN value\n");
      exit(-1);
    }
    file_name = argv[1];
  }


  /* ----------------- Get Input Descriptor ----------------- */
  if ((argc > 1) && (strcmp(file_name, "-") != 0)) {
    fd = open(file_name, O_RDONLY);
    if (fd < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }
  }

  /* ------------------------ Encode ------------------------ */
  if(strcmp(baseN, "32") == 0){
    encodeBase32(fd);
  }
  // default base64
  else{
    encodeBase64(fd);
  }

  return 0;
}

/* encodeBase64: reads data from input_fd enodes it in base64, and stores it in inBuffer */
int encodeBase64(int fd_in) {
  ssize_t nread, nwrite;
  int count = 0, i;
  uint8_t inBuf[INBUFFSIZE64], outBuf[OUTBUFFSIZE64];

  /* -------------------------- Read -------------------------- */
  while ((nread = read(fd_in, inBuf, INBUFFSIZE64)) != 0) {
    if (nread < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }

    /* -------------------- Encode algorithm -------------------- */
    // upper 6 bits of byte 0
    outBuf[0] = alphabet64[inBuf[0] >> 2];
    // lower 2 bits of byte 0, shift left and or with the upper 4 bits of byte 1
    outBuf[1] = alphabet64[((inBuf[0] & 0x03) << 4) | (inBuf[1] >> 4)];
    // lower 4 bits of byte 1, shift left and or with upper 2 bits of byte 2
    outBuf[2] = alphabet64[((inBuf[1] & 0x0F) << 2) | (inBuf[2] >> 6)];
    // lower 6 bits of byte 2
    outBuf[3] = alphabet64[inBuf[2] & 0x3F];

    for (i = INBUFFSIZE64; i > nread; i--) {
      outBuf[i] = alphabet64[64];             // pad output if less than 3 bytes
    }

    /* -----------------------    printf("file name: %s\n", file_name);--- Write -------------------------- */
    for (size_t offset = 0; offset < OUTBUFFSIZE64;) {
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, OUTBUFFSIZE64 - offset)) < 0) {
        perror("error");
        exit(-1);
      }

      offset += nwrite;
      count += nwrite;

      // write new line every 76 characters
      if (count % MAXLINE == 0) {
        write(STDOUT_FILENO, "\n", sizeof(char));
      }
    }

    memset(inBuf, 0, INBUFFSIZE64);             // sanitze input buffer
    memset(outBuf, 0, OUTBUFFSIZE64);           // sanitze output buffer
  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
  return 0;
}


/* encodeBase32: reads data from input_fd enodes it in base32, and stores it in inBuffer */
void encodeBase32(int fd_in) {
  ssize_t nread, nwrite;
  int count = 0, i, j;
  uint8_t inBuf[INBUFFSIZE32], outBuf[OUTBUFFSIZE32];

  /* -------------------------- Read -------------------------- */
  while ((nread = read(fd_in, inBuf, INBUFFSIZE32)) != 0) {
    if (nread < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }

    /* -------------------- Encode algorithm2    -------------------- */
    // upper 5 bits of input byte 1 (drop lower 3 bits)
    outBuf[0] = alphabet32[inBuf[0] >> 3];
    // lower 3 bits of input byte 1 and upper 2 bits of byte 2
    outBuf[1] = alphabet32[((inBuf[0] & 0x07) << 2) | (inBuf[1] >> 6)];
    // bits 3 - 7 of input byte 2
    outBuf[2] = alphabet32[(inBuf[1] & 0x3E) >> 1];
    // lower 1 bit of input byte 2 upper 4 bits of input byte 3
    outBuf[3] = alphabet32[(((inBuf[1] & 0x01) << 4) | (inBuf[2] >> 4))];
    // lower 4 bits of input byte 3 upper 1 bit of input byte 4
    outBuf[4] = alphabet32[((inBuf[2] & 0x0F) << 1) | (inBuf[3] >> 7)];
    // bits 2 - 6 of input byte 4
    outBuf[5] = alphabet32[(inBuf[3] & 0x7C) >> 2];
    // lower 2 bits of input byte 4 upper 3 bits of input byte 5
    outBuf[6] = alphabet32[((inBuf[3] & 0x03) << 3) | (inBuf[4] >> 5)];
    // lower 5 bits of input byte 5
    outBuf[7] = alphabet32[inBuf[4] & 0x1F];

    if (nread < INBUFFSIZE32) {
      if (nread == 1) {
        i = 2;
      }
      else if (nread == 2) {
        i = 4;
      }
      else if (nread == 3) {
        i = 5;
      }
      else {
        i = 7;
      }

      for (i; i < OUTBUFFSIZE32; i++) {
        outBuf[i] = alphabet32[32];
      }
    }

    /* -------------------------- Write -------------------------- */
    for (size_t offset = 0; offset < OUTBUFFSIZE32;) {
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, OUTBUFFSIZE32 / 2)) < 0) {
        perror("error");
        exit(-1);
      }

      offset += nwrite;
      count += nwrite;

      // write new line every 76 characters
      if (count % MAXLINE == 0) {
        write(STDOUT_FILENO, "\n", sizeof(char));
      }
    }

    memset(inBuf, 0, INBUFFSIZE32);             // sanitze input buffer
    memset(outBuf, 0, OUTBUFFSIZE32);           // sanitze output buffer
  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
}




