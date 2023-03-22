// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define DECODER_INBUFFSIZE_16 3
#define DECODER_OUTBUFFSIZE_16 2

/* b16_isvalidchar:   checks for valid HEX characters. Returns true if valid, false otherwise */
int b16_isvalidchar(char c)
{
  if (c >= '0' && c <= '9')
    return true;
  if (c >= 'A' && c <= 'F')
    return true;
  if (c == '=')
    return true;
  return false;
}


/* decodeBase16:  decodes data in base16 format to ascii */
void decodeBase16(int fd_in) {
  size_t nread, nwrite;
  int i, j, count;
  uint8_t inBuf[DECODER_INBUFFSIZE_16], outBuf[DECODER_OUTBUFFSIZE_16], indexes[DECODER_INBUFFSIZE_16], buffchar[2];
  count = 0;

  // read in 1 byte at a time -- checking for only valid b64 characters
  while ((nread = read(fd_in, buffchar, 1)) != 0) {
    if (nread < 0) {
      perror("error");
      exit(-1);
    }
    
    // stores valid base64 characters in the inBuf for processing
    if(b16_isvalidchar(buffchar[0])){
      inBuf[count] = *buffchar;
      count++;
    }

    // have a full (4 byte) input buffer 
    if (count == DECODER_INBUFFSIZE_16 - 1) {

      // convert the ascii index to its corepsonding base64 index
      for (j = 0; j < count; j++) {
        if (!b16_isvalidchar(inBuf[j])) {
          printf("error: Invalid base16 character");  // should already be taken care of when prepping the input buffer
          exit(-1);
        }
        
        for (i = 0; i < 17; i++) {
          if (alphabet16[i] == inBuf[j]) {
            indexes[j] = i;
            break;
          }
        }
      }

      /* --------------------- Get ASCII Character from Integer --------------------- */
      outBuf[0] = ((indexes[0] << 4) | indexes[1]);

      /* -------------------------- Write -------------------------- */
      writedecoded(STDOUT_FILENO, outBuf, count / 2);

      // Sanitze arrays
      memset(outBuf, 0, DECODER_OUTBUFFSIZE_16);
      memset(indexes, 0, DECODER_INBUFFSIZE_16);
      memset(inBuf, 0, DECODER_INBUFFSIZE_16);
      memset(buffchar, 0, 2);

      count = 0;
    }
  }
}