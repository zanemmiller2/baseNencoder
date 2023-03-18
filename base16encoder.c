// --------- includes -------------
#include "baseNencoder.h"
// --------- defines ---------------
#define INBUFFSIZE16 1
#define OUTBUFFSIZE16 2

/* encodeBase16: reads data from input_fd enodes it in base16 (hex), and stores it in inBuffer */
void encodeBase16(int fd_in) {
  ssize_t nread, nwrite;
  int count = 0, i, j;
  uint8_t inBuf[INBUFFSIZE16], outBuf[OUTBUFFSIZE16];

  /* -------------------------- Read -------------------------- */
  while ((nread = read(fd_in, inBuf, INBUFFSIZE16)) != 0) {
    if (nread < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }

    /* -------------------- Encode algorithm2    -------------------- */
    // upper 4 bits of input byte 1
    outBuf[0] = alphabet16[inBuf[0] >> 4];
    // lower 4 bits of input byte 1
    outBuf[1] = alphabet16[inBuf[0] & 0x0F];


    /* -------------------------- Write -------------------------- */
    for (size_t offset = 0; offset < OUTBUFFSIZE16;) {
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, OUTBUFFSIZE16 - offset)) < 0) {
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

    memset(inBuf, 0, INBUFFSIZE16);             // sanitze input buffer
    memset(outBuf, 0, OUTBUFFSIZE16);           // sanitze output buffer
  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
}