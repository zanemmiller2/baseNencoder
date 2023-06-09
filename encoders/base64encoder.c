// --------- includes -------------
#include "../baseNencoder.h"
// ------------ defines ------------
#define ENCODER_INBUFFSIZE_64 3
#define ENCODER_OUTBUFFSIZE_64 4

/* encodeBase64: reads data from input_fd enodes it in base64, and stores it in inBuffer */
void encodeBase64(int fd_in) {
  ssize_t nread, nwrite;
  int count = 0, i;
  uint8_t inBuf[ENCODER_INBUFFSIZE_64], outBuf[ENCODER_OUTBUFFSIZE_64];

  /* -------------------------- Read -------------------------- */
  while ((nread = read(fd_in, inBuf, ENCODER_INBUFFSIZE_64)) != 0) {
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

    for (i = ENCODER_INBUFFSIZE_64; i > nread; i--) {
      outBuf[i] = alphabet64[64];             // pad output if less than 3 bytes
    }

    /* -------------------------- Write -------------------------- */
    for (size_t offset = 0; offset < ENCODER_OUTBUFFSIZE_64;) {
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, ENCODER_OUTBUFFSIZE_64 - offset)) < 0) {
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

    memset(inBuf, 0, ENCODER_INBUFFSIZE_64);             // sanitze input buffer
    memset(outBuf, 0, ENCODER_OUTBUFFSIZE_64);           // sanitze output buffer
  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
}