// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define DECODER_INBUFFSIZE_64 4
#define DECODER_OUTBUFFSIZE_64 3

/* b64_isvalidchar:   checks for valid b64 characters. Returns true if valid, false otherwise */
int b64_isvalidchar(char c)
{
  if (c >= '0' && c <= '9')
    return true;
  if (c >= 'A' && c <= 'Z')
    return true;
  if (c >= 'a' && c <= 'z')
    return true;
  if (c == '+' || c == '/' || c == '=')
    return true;
  return false;
}


/* decodeBase64:  decodes data in base64 format to ascii */
void decodeBase64(int fd_in) {
  size_t nread, nwrite;
  int i, j, count;
  uint8_t inBuf[DECODER_INBUFFSIZE_64], outBuf[DECODER_OUTBUFFSIZE_64], indexes[DECODER_INBUFFSIZE_64], buffchar[2];
  count = 0;

  // read in 1 byte at a time -- checking for only valid b64 characters
  while ((nread = read(fd_in, buffchar, 1)) != 0) {
    if (nread < 0) {
      perror("error");
      exit(-1);
    }
    
    // stores valid base64 characters in the inBuf for processing
    if(b64_isvalidchar(buffchar[0])){
      inBuf[count] = *buffchar;
      count++;
    }

    // have a full (4 byte) input buffer 
    if (count == DECODER_INBUFFSIZE_64) {

      // conver the ascii index to its corepsonding base64 index
      for (j = 0; j < count; j++) {
        if (!b64_isvalidchar(inBuf[j])) {
          printf("error: Invalid base64 character");  // should already be taken care of when prepping the input buffer
          exit(-1);
        }

        for (i = 0; i < 65; i++) {
          if (alphabet64[i] == inBuf[j]) {
            // remove prefix "00" from each encoded character
            indexes[j] = i << 2;
            break;
          }
        }
      }

      /* --------------------- Get ASCII Value --------------------- */
      // top 6 bits of input byte 1 top 2 bits of input byte 2
      outBuf[0] = (((indexes[0] & 0xFC) | (indexes[1] >> 6)));
      // bits 3 - 6 of input byte 2 top 4 bites of input byte 3
      outBuf[1] = (((indexes[1] << 2) & 0xF0) | (indexes[2] >> 4));
      // bits 5 - 6 of input byte 3 top 6 bits of input byte 4
      outBuf[2] = (((indexes[2] << 4) & 0xC0) | (indexes[3] >> 2));

      if(inBuf[DECODER_INBUFFSIZE_64 - 1] == '='){
        count -= 2;
        }

      /* -------------------------- Write -------------------------- */
      writedecoded(STDOUT_FILENO, outBuf, count * 3 / 4);


      // Santize Arrays
      memset(outBuf, 0, DECODER_OUTBUFFSIZE_64);
      memset(inBuf, 0, DECODER_INBUFFSIZE_64);
      memset(indexes, 0, DECODER_INBUFFSIZE_64);
      memset(buffchar, 0, 2);

      count = 0;
    }
  }
}