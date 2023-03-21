// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define ENCODER_INBUFFSIZE_32 5
#define ENCODER_OUTBUFFSIZE_32 8


/* encodeBase32: reads data from input_fd enodes it in base32, and stores it in inBuffer */
void encodeBase32(int fd_in) {
  ssize_t nread, nwrite;
  int count = 0, i, j;
  uint8_t inBuf[ENCODER_INBUFFSIZE_32], outBuf[ENCODER_OUTBUFFSIZE_32];

  /* -------------------------- Read -------------------------- */
  while ((nread = read(fd_in, inBuf, ENCODER_INBUFFSIZE_32)) != 0) {
    if (nread < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }

    /* -------------------- Encode algorithm -------------------- */
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

    if (nread < ENCODER_INBUFFSIZE_32) {
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

      for (i; i < ENCODER_OUTBUFFSIZE_32; i++) {
        outBuf[i] = alphabet32[32];
      }
    }

    /* -------------------------- Write -------------------------- */
    for (size_t offset = 0; offset < ENCODER_OUTBUFFSIZE_32;) {
      // read it out in 2 blocks of 4 to catch line feed at char 76
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, ENCODER_OUTBUFFSIZE_32 / 2)) < 0) {
        perror("error");
        exit(-1);
      }

      offset += nwrite;
      count += nwrite;

      // write new line every 76 characters
      if (count % MAXLINE76 == 0) {
        write(STDOUT_FILENO, "\n", sizeof(char));
      }
    }

    memset(inBuf, 0, ENCODER_INBUFFSIZE_32);             // sanitze input buffer
    memset(outBuf, 0, ENCODER_OUTBUFFSIZE_32);           // sanitze output buffer
  }
  write(STDOUT_FILENO, "\n", 1);
}