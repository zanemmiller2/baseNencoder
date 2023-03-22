// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define ENCODER_INBUFFSIZE_Z85 5    // 4 + '\0'
#define ENCODER_OUTBUFFSIZE_Z85 6   // 5 + '\0'

/* encodeBasez85: reads data from input_fd enodes it in basez85 (ASCII85), and stores it in inBuffer */
void encodeBasez85(int fd_in) {
  size_t nread, nwrite;
  int i, j, numpads, outcount, incount;
  uint8_t inBuf[ENCODER_INBUFFSIZE_Z85], outBuf[ENCODER_OUTBUFFSIZE_Z85];
  char hexstring[ENCODER_INBUFFSIZE_Z85 * 2 - 1];
  unsigned long long int hexnum;
  numpads = 0;

  /* -------------------------------------------------------------
  #
  #     read in 4 bytes (32 bit DOUBLE WORD) at a time
  #
  ------------------------------------------------------------- */
  while ((nread = read(fd_in, inBuf, ENCODER_INBUFFSIZE_Z85 - 1)) != 0) {
    incount = nread;
    if (nread < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }

    // Pad input less than 4 bytes with "00"
    if(inBuf[incount - 1] == '\n'){
      --incount;
      while (incount < ENCODER_INBUFFSIZE_Z85 - 1){
        inBuf[incount++] = 0;
        numpads++;
      }
    }

    /* -------------------------------------------------------------
    #
    #     convert each byte in input to to its hex representation
    #
    ------------------------------------------------------------- */
    j = 0;
    for (i = 0; i < incount; i++) {
      if (inBuf[i] < 0 || inBuf[i] > 255) {
        printf("ERROR: INVALID CHARACTER CANNOT ENCODE %c\n", inBuf[i]);
        exit(-1);
      }
      hexstring[j++] = alphabetz85[inBuf[i] / 16];
      hexstring[j++] = alphabetz85[inBuf[i] % 16];
    }

    /* -------------------------------------------------------------
    #
    #     convert hexstring to unsigned long long int
    #
    ------------------------------------------------------------- */
    hexnum = (unsigned long long int)strtoull(hexstring, NULL, 16);


    /* -------------------------------------------------------------
 or   #
    #     convert input's integer value to base z85
    #
    ------------------------------------------------------------- */
    j = 0;
    outcount = 0;
    while (hexnum > 0) {
      outBuf[j++] = alphabetz85[hexnum % 85];
      hexnum = hexnum / 85;
      outcount += 1;
    }

    /* -------------------------------------------------------------
    #
    #     reverse string
    #
    ------------------------------------------------------------- */
    int l = 0;
    int r = outcount - 1;
    while (l < r) {
      char temp = outBuf[l];
      outBuf[l++] = outBuf[r];
      outBuf[r--] = temp;
    }

    /* -------------------------------------------------------------
    #
    #     reverse string
    #
    ------------------------------------------------------------- */
    // exclude the padding for output in z85
    outcount -= numpads;

    for (size_t offset = 0; offset < outcount;) {
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, outcount - offset)) < 0) {
        perror("error");
        exit(-1);
      }
      offset += nwrite;
    }

    /* -------------------------------------------------------------
    #
    #     sanitize arrays
    #
    ------------------------------------------------------------- */
    memset(inBuf, 0, ENCODER_INBUFFSIZE_Z85);
    memset(outBuf, 0, ENCODER_OUTBUFFSIZE_Z85);
    memset(hexstring, 0, ENCODER_INBUFFSIZE_Z85 * 2 - 1);
    hexnum = 0;
    incount = 0;
    outcount = 0;

  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
}