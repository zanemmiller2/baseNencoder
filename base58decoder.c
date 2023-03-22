// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define DECODER_INBUFFSIZE_58 12    // 11 + 1 for '\0'
#define DECODER_OUTBUFFSIZE_58 9    // 8 + 1 for '\0'


/* b58_isvalidchar:   checks for valid Base58 characters. Returns true if valid, false otherwise */
int b58_isvalidchar(char c)
{
  // skip '0'
  if (c >= '1' && c <= '9')
    return true;
  // skip 'I'
  if (c >= 'A' && c <= 'H')
    return true;
  // skip 'O'
  if (c >= 'J' && c <= 'N')
    return true;
  if (c >= 'P' && c <= 'Z')
    return true;
  // skip 'l'
  if (c >= 'a' && c <= 'k')
    return true;
  if (c >= 'm' && c <= 'z')
    return true;
  return false;
}


/* decodeBase58:  decodes data in base58 format to ascii */
void decodeBase58(int fd_in) {
  size_t nread, nwrite;
  int i, j, incount, outcount;
  uint8_t inBuf[DECODER_INBUFFSIZE_58], outBuf[DECODER_OUTBUFFSIZE_58], indexes[DECODER_INBUFFSIZE_58], buffchar[2];
  incount = 0;
  outcount = 0;


  /* -------------------------------------------------------------
  #
  #     read in up to 11 encoded bytes at a time
  #
  ------------------------------------------------------------- */
  while ((nread = read(fd_in, inBuf, DECODER_INBUFFSIZE_58 - 1)) != 0) {
    if (nread < 0) {
      perror("error");
      exit(-1);
    }
    
    incount = nread;
    /* -------------------------------------------------------------
    #
    #     remove padding and new line characters
    #
    ------------------------------------------------------------- */
    for(i = 0; i < nread; i++){
      if(inBuf[i] == '=' || inBuf[i] == '\n'){
        incount--;
      }
    }

    /* -------------------------------------------------------------
    #
    #     convert the input bytes to their base58 index values
    #
    ------------------------------------------------------------- */
    for (j = 0; j < incount; j++) {
      if (!b58_isvalidchar(inBuf[j])) {
        printf("error: Invalid base58 character"); 
        exit(-1);
      }

      for (i = 0; i < 58; i++) {
        if (alphabet58[i] == inBuf[j]) {
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
      decnum *= 58;
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
    memset(outBuf, 0, DECODER_OUTBUFFSIZE_58);
    memset(indexes, 0, DECODER_INBUFFSIZE_58);
    memset(inBuf, 0, DECODER_INBUFFSIZE_58);
    outcount = 0;
    incount = 0;
  }
}

