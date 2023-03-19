// --------- includes -------------
#include "baseNencoder.h"
// --------- defines ---------------
#define INBUFFSIZE64 4
#define OUTBUFFSIZE64 3

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


/* base64 decoder:  decodes data in base64 format to ascii */
void decodeBase64(int fd_in) {
  size_t nread, nwrite;
  int i, j, count;
  uint8_t inBuf[INBUFFSIZE64], outBuf[OUTBUFFSIZE64], indexes[INBUFFSIZE64], buffchar[1];
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
    if (count == INBUFFSIZE64) {

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


      /* -------------------------- Write -------------------------- */
      for (size_t offset = 0; offset < count * 3 / 4;) {
        if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, count * 3 / 4)) < 0) {
          perror("error");
          exit(-1);
        }
        offset += nwrite;
      }
      count = 0;
    }
  }
}