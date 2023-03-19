// --------- includes -------------
#include "baseNencoder.h"
// --------- defines ---------------
#define INBUFFSIZE64 4
#define OUTBUFFSIZE64 3

/* base64 decoder:  decodes data in base64 format to ascii */
void decodeBase64(int fd_in) {
  size_t nread, nwrite;
  int i, j;
  uint8_t inBuf[INBUFFSIZE64];
  char outBuf[OUTBUFFSIZE64];
  int indexes[INBUFFSIZE64];
  
  // any block of encoded characters less than 
  while ((nread = read(fd_in, inBuf, INBUFFSIZE64)) == INBUFFSIZE64) {
    if (nread < 0) {
      perror("error");
      exit(-1);
    }
    
    // conver the ascii index to base64 index
    for (j = 0; j < nread; j++) {
      indexes[j] = 0;
      for (i = 0; i < 64; i++) {
        if (alphabet64[i] == inBuf[j]) {
          // remove prefix "00" from each encoded character
          indexes[j] = (i << 2);
          break;
        }
      }
    }
    
    outBuf[0] = ((indexes[0] & 0xFC) | (indexes[1] >> 6));
    outBuf[1] = (((indexes[1] << 2) & 0xF0) | (indexes[2] >> 4));
    outBuf[2] = (((indexes[2] << 4) & 0xC0) | (indexes[3] >> 2));

    if(inBuf[INBUFFSIZE64 - 1] == '='){
      outBuf[2] = 0;
    }
    if(inBuf[INBUFFSIZE64 - 2] == '='){
      outBuf[1] = 0;
    }

    if(inBuf[INBUFFSIZE64 - 3] == '='){
      outBuf[0] = 0;
    }

    /* -------------------------- Write -------------------------- */
    for (size_t offset = 0; offset < OUTBUFFSIZE64;) {
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, OUTBUFFSIZE64)) < 0) {
        perror("error");
        exit(-1);
      }
      offset += nwrite;
  }

}
}