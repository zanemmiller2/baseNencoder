// --------- includes -------------
#include "baseNencoder.h"

#define INBUFLEN 10000
#define OUTBUFLEN (INBUFLEN * 7 / 5)

/* encodebase58: reads data from input_fd enodes it in base58, and stores it in inBuffer */
void encodeBase58(int fd_in){
  ssize_t nread, nwrite;
  int count = 0, i, j;
  uint8_t inBuf[INBUFLEN];
  uint8_t outBuf[OUTBUFLEN];


  /* -------------------------------------------------------------
  # 
  #     Read in data from fd
  #
  ------------------------------------------------------------- */

  /* -------------------------------------------------------------
  # 
  #     Convert each input byte's integer 
  #     representation to Hex
  #
  ------------------------------------------------------------- */

  /* -------------------------------------------------------------
  # 
  #     add byte's hex representation to buffer
  #
  ------------------------------------------------------------- */

  /* -------------------------------------------------------------
  # 
  #     concatenate the hex string and convert 
  #     to BIG integer
  #
  ------------------------------------------------------------- */

  /* -------------------------------------------------------------
  # 
  #     convert BIG integer to base 58 and 
  #     add to output buffer
  #
  ------------------------------------------------------------- */

  /* -------------------------------------------------------------
  # 
  #     return/write string in reversed order
  #
  ------------------------------------------------------------- */

}