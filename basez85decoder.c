// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define DECODER_INBUFFSIZE_Z85 6    // 5 + '\0'
#define DECODER_OUTBUFFSIZE_Z85 5    // 4 + '\0'


/* bz85_isvalidchar:   checks for valid Basez85 characters. Returns true if valid, false otherwise */
int bz85_isvalidchar(char c)
{
  // // skip '0'
  // if (c <= ' ' || c == '"' || c == "\'" || c == ',' || c == ';' || c == '_' || c == '\\' || c == '`' || c >= '~')
  //   return false;
  if (c <= 32 || c == 34 || c == 39 || c == 44 || c == 59 || c == 92 || c == 95 || c == 96 || c >= 126 )
    return false;

  return true;
}


/* decodeBase58:  decodes data in base58 format to ascii */
void decodeBasez85(int fd_in) {
  size_t nread, nwrite;
  int i, j, incount, outcount, numpads;
  uint8_t inBuf[DECODER_INBUFFSIZE_Z85], outBuf[DECODER_OUTBUFFSIZE_Z85], indexes[DECODER_OUTBUFFSIZE_Z85];
  incount = 0;
  outcount = 0;
  numpads = 0;


  /* -------------------------------------------------------------
  #
  #     read in up to 5 encoded bytes at a time
  #
  ------------------------------------------------------------- */
  while ((nread = read(fd_in, inBuf, DECODER_INBUFFSIZE_Z85 - 1)) != 0) {
    incount = nread;
    if (nread < 0) {
      perror("error");
      exit(-1);
    }

    // skip over new line characters
    if(inBuf[incount - 1] == '\n'){
      --incount;
    }
  

    /* -------------------------------------------------------------
    #
    #     convert the input bytes to their basez85 index values
    #
    ------------------------------------------------------------- */
    for (j = 0; j < incount; j++) {
      if (!bz85_isvalidchar(inBuf[j])) {
        printf("error: Invalid basez85 character"); 
        exit(-1);
      }

      for (i = 0; i < 85; i++) {
        if (alphabetz85[i] == inBuf[j]) {
          indexes[j] = i;
          break;
        }
      }
    }

    /* -------------------------------------------------------------
    #
    #     convert string to base10 represenation
    #
    ------------------------------------------------------------- */
    unsigned long long int decnum = 0;
    for (i = 0; i < incount - 1; i++) {
      decnum += indexes[i];
      decnum *= 85;
    }
    decnum += indexes[i];

    /* -------------------------------------------------------------
    #
    #     convert base10 integer to ascii string
    #
    ------------------------------------------------------------- */
    i = 0;
    while (decnum > 0){
      outBuf[i++] = decnum % 256;
      decnum /= 256;
      outcount += 1;
    }

    /* -------------------------------------------------------------
    #
    #     reverse the output
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
    #     write
    #
    ------------------------------------------------------------- */
    writedecoded(STDOUT_FILENO, outBuf, outcount);

    /* -------------------------------------------------------------
    #
    #     sanitzie the arrays/variables
    #
    ------------------------------------------------------------- */
    memset(outBuf, 0, DECODER_OUTBUFFSIZE_Z85);
    memset(indexes, 0, DECODER_INBUFFSIZE_Z85);
    memset(inBuf, 0, DECODER_INBUFFSIZE_Z85);
    outcount = 0;
    incount = 0;
  }
  write(STDOUT_FILENO, "\n", sizeof(char));   // write new line at end
}

