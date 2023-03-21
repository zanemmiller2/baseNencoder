// --------- includes -------------
#include "baseNencoder.h"

/* b32_isvalidchar:   checks for valid b32 characters. Returns true if valid, false otherwise */
int b32_isvalidchar(char c)
{
  if (c >= '2' && c <= '7')
    return true;
  if (c >= 'A' && c <= 'Z')
    return true;
  if (c == '=')
    return true;
  return false;
}


/* decodeBase32:  decodes data in base32 format to ascii */
void decodeBase32(int fd_in) {
  size_t nread, nwrite;
  int i, j, count;
  uint8_t inBuf[DECODER_INBUFFSIZE_32], outBuf[DECODER_OUTBUFFSIZE_32], indexes[DECODER_INBUFFSIZE_32], buffchar[2];
  count = 0;

  // read in 1 byte at a time -- checking for only valid b32 characters
  while ((nread = read(fd_in, buffchar, 1)) != 0) {
    if (nread < 0) {
      perror("error");
      exit(-1);
    }
    
    // stores valid base64 characters in the inBuf for processing
    if(b32_isvalidchar(buffchar[0])){
      inBuf[count] = buffchar[0];
      count++;
    }

    // have a full (4 byte) input buffer 
    if (count == DECODER_INBUFFSIZE_32) {

      // conver the ascii index to its corepsonding base64 index
      for (j = 0; j < count; j++) {
        if (!b32_isvalidchar(inBuf[j])) {
          printf("error: Invalid base32 character");  // should already be taken care of when prepping the input buffer
          exit(-1);
        }

        for (i = 0; i < 33; i++) {
          if (alphabet32[i] == inBuf[j]) {
            // TODO remove ?? prefix
            indexes[j] = i;
            break;
          }
        }
      }

      /* --------------------- Get ASCII Value --------------------- */
      // TODO

      /* -------------- Check for padding characters --------------- */
      // TODO 

      /* -------------------------- Write -------------------------- */
      for (size_t offset = 0; offset < count * 5 / 8;) {
        if ((nwrite = write(STDOUT_FILENO, offset + (char*)outBuf, count * 5 / 8)) < 0) {
          perror("error"); 
          exit(-1);
        }
        offset += nwrite;
      }
      count = 0;
    }
  }
}