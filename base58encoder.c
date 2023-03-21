// ------------ includes ------------
#include "baseNencoder.h"

// ------------ defines ------------
#define ENCODER_INBUFFSIZE_58 9
#define ENCODER_OUTBUFFSIZE_58 12


/* encodebase58: reads data from input_fd encodes (in blocks of 8 bytes) it in base58 */
void encodeBase58(int fd_in) {
  size_t nread, nwrite;
  int outcount, i, j;
  uint8_t inBuf[ENCODER_INBUFFSIZE_58], outBuf[ENCODER_OUTBUFFSIZE_58];
  unsigned long long int hexnum;

  /* -------------------------------------------------------------
  #
  #     Read in data from fd
  #
  ------------------------------------------------------------- */
  while ((nread = read(fd_in, inBuf, ENCODER_INBUFFSIZE_58 - 1)) != 0) {
    if (nread < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }

    char hexstring[(nread * 2) + 1];

    /* -------------------------------------------------------------
    #
    #     Convert input to string of each byte's hex value
    #
    ------------------------------------------------------------- */
    j = 0;
    for (i = 0; i < nread; i++) {
      if (inBuf[i] < 0 || inBuf[i] > 255) {
        printf("ERROR: INVALID CHARACTER CANNOT ENCODE %c\n", inBuf[i]);
        exit(-1);
      }
      hexstring[j++] = alphabet16[inBuf[i] / 16];
      hexstring[j++] = alphabet16[inBuf[i] % 16];
    }


    /* -------------------------------------------------------------
    #
    #     convert hexstring to unsigned long long int
    #
    ------------------------------------------------------------- */
    hexnum = (unsigned long long int)strtoull(hexstring, NULL, 16);

    /* -------------------------------------------------------------
    #
    #     convert input's integer value to base 58
    #
    ------------------------------------------------------------- */
    j = 0;
    outcount = 0;
    while (hexnum > 0) {
      outBuf[j++] = alphabet58[hexnum % 58];
      hexnum = hexnum / 58;
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
    #     write string
    #
    ------------------------------------------------------------- */
    for (size_t offset = 0; offset < ENCODER_OUTBUFFSIZE_58;) {
      if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, ENCODER_OUTBUFFSIZE_58 - offset)) < 0) {
        perror("error");
        exit(-1);
      }
      offset += nwrite;
    }

    // Sanitze Arrays
    memset(inBuf, 0, ENCODER_INBUFFSIZE_58);             
    memset(outBuf, 0, ENCODER_OUTBUFFSIZE_58);
    memset(hexstring, 0, (nread * 2) + 1);

    write(STDOUT_FILENO, "  ", sizeof(char) * 2);        // write two spaces between octets
  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
}


