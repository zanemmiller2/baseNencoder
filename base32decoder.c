// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define DECODER_INBUFFSIZE_32 8
#define DECODER_OUTBUFFSIZE_32 5

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
    if (b32_isvalidchar(buffchar[0])) {
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
            // remove "000" prefix
            indexes[j] = i << 3;
            break;
          }
        }
      }

      /* --------------------- Get ASCII Value --------------------- */
      // upper 5 bits of input byte 0 (5), and upper 3 bits of input byte 1 (3)
      outBuf[0] = (indexes[0] | indexes[1] >> 5);
      // bits 4 - 5 input byte 1 (2), upper 5 bits of input byte 2 (5), upper 1 bit of input byte 3 (1)
      outBuf[1] = ((indexes[1] << 3 | indexes[2] >> 2) | indexes[3] >> 7);
      // bits 2 - 5 input byte 3 (4), upper 4 bits of input byte 4 (4)
      outBuf[2] = (indexes[3] << 1 | indexes[4] >> 4);
      // bit 5 input byte 4 (1), upper 5 bits of input byte 5 (5), upper 2 bits of input byte 6 (2)
      outBuf[3] = ((indexes[4] << 4 | indexes[5] >> 1) | indexes[6] >> 6);
      // bits 3 - 5 of input byte 6 (3), upper 5 bits of input byte 7 (5)
      outBuf[4] = (indexes[6] << 2 | indexes[7] >> 3);


      /* -------------- Check for padding characters --------------- */
      if (inBuf[2] == '=') {
        count -= 6;
      }
      else if (inBuf[4] == '=') {
        count -= 4;
      }
      else if (inBuf[5] == '=') {
        count -= 3;
      }
      else if (inBuf[7] == '=') {
        count -= 1;
      }

      /* -------------------------- Write -------------------------- */
      writedecoded(STDOUT_FILENO, outBuf, count * 5 / 8);
      count = 0;
    }
  }
}


