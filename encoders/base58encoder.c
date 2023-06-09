// --------- includes -------------
#include "../baseNencoder.h"

// ------------ defines ------------
#define ENCODER_INBUFFSIZE_58 257   // 256 + '\0'
#define ENCODER_OUTBUFFSIZE_58 355  // 256 * 1.38 + '\0'
#define B58_ENCODE_BASE 58

/* convert_hex_to_string_int:
      takes an hexstring and converts its integer
      representation to an array of integers representing
      the digits of the arbitrarily large integer
*/
void convert_hex_to_string_int(char string_hex[], int hexlen, unsigned int bigint[], int bigint_len, int power16) {
  int i = 0;
  int right, left, j;
  int hexval;


  uint8_t tempbig[bigint_len];

  // for each char in the hex string
  for (i = 0; i < hexlen; i++, power16--) {

    // get the hex characters integer value
    for (int k = 0; k < 16; k++) {
      if (string_hex[i] == alphabet16[k]) {
        hexval = k;
      }
    }

    /* ########## convert (hexval * 16 ^ power16--) to string of integers  ########### */
    memset(tempbig, 0, bigint_len);
    tempbig[0] = 1;

    // for each power in the current level
    for (int pow = 0; pow < power16; pow++) {

      // multiply every number in temp array by 16
      for (j = 0; j < bigint_len; j++) {
        tempbig[j] *= 16;
      }

      // reduce and carry -- tempbig
      for (j = 0; j < bigint_len; j++) {
        if (tempbig[j] >= 10) {
          int remainder = tempbig[j] % 10;
          int carry = tempbig[j] / 10;
          tempbig[j] = remainder;
          tempbig[j + 1] += carry;
        }
      }
    }
    /* ####################################################################### */



    /* ############# multiply every number in tempbig by hexval  ############# */
    for (j = 0; j < bigint_len; j++) {
      tempbig[j] *= hexval;
    }

    // reduce and carry tempbig after multiplying by hexval
    for (j = 0; j < bigint_len; j++) {
      if (tempbig[j] >= 10) {
        int remainder = tempbig[j] % 10;
        int carry = tempbig[j] / 10;
        tempbig[j] = remainder;
        tempbig[j + 1] += carry;
      }
    }

    // add it to the total array
    for (j = 0; j < bigint_len; j++) {
      bigint[j] += tempbig[j];
    }

    // reduce and carry -- bigint
    for (j = 0; j < bigint_len; j++) {
      if (bigint[j] >= 10) {
        int remainder = bigint[j] % 10;
        int carry = bigint[j] / 10;
        bigint[j] = remainder;
        bigint[j + 1] += carry;
      }
    }
    /* ####################################################################### */
  }

  // reverse bigint
  left = 0;
  right = bigint_len - 1;
  while (left < right) {
    int temp = bigint[left];
    bigint[left] = bigint[right];
    bigint[right] = temp;
    left++;
    right--;
  }
  // return
}


/* encodebase58:
      reads data from input_fd and encodes it in base58
*/
void encodeBase58(int fd_in) {
  size_t nread, nwrite;
  int outcount, i, j, totalCount, num_hex_chars, power16, hexstring_len, bigint_len, left, right;
  uint8_t inBuf[ENCODER_INBUFFSIZE_58], outBuf[ENCODER_OUTBUFFSIZE_58];
  totalCount = 0;

  /* -------------------------------------------------------------
  #
  #     Read in data from fd (at most 255 bytes)
  #
  ------------------------------------------------------------- */
  nread = read(fd_in, inBuf, ENCODER_INBUFFSIZE_58 - 1);
  if (nread < 0) {
    perror("error");  // invalid file descriptor
    exit(-1);
  }
  if (nread >= ENCODER_INBUFFSIZE_58 - 1) {
    printf("WARNING: ONLY ENCODING THE FIRST 255 BYTES. IF INPUT IS LARGER THAN 255 BYTES: DEFINITELY ERRORS\n");
  }

  /* -------------------------------------------------------------
  #
  #     Convert input to hexstring
  #
  ------------------------------------------------------------- */
  num_hex_chars = ((nread - 1) * 2) + 1;
  char hexstring[num_hex_chars];
  memset(hexstring, 0, num_hex_chars);

  for (i = 0, j = 0; i < nread; i++) {
    if (inBuf[i] < 0 || inBuf[i] > 255) {
      printf("ERROR: INVALID CHARACTER CANNOT ENCODE %c\n", inBuf[i]);
      exit(-1);
    }
    hexstring[j++] = alphabet16[inBuf[i] / 16];
    hexstring[j++] = alphabet16[inBuf[i] % 16];
  }
  hexstring[j] = '\0';

  /* -------------------------------------------------------------
  #
  #     convert hexstring to integer as an array of digits
  #
  ------------------------------------------------------------- */
  power16 = j - 1;
  hexstring_len = power16 + 1;
  bigint_len = ((hexstring_len) * 1.2) / 1;

  unsigned int bigint[bigint_len];
  for (i = 0; i < bigint_len; i++) {
    bigint[i] = 0;
  }
  convert_hex_to_string_int(hexstring, hexstring_len, bigint, bigint_len, power16);


  /* -------------------------------------------------------------
  #
  #     convert input's integer value to base 58
  #
  ------------------------------------------------------------- */
  j = 0;
  while (bigint_len > 0) {
    int b58_index = mod_bigint(bigint, B58_ENCODE_BASE, bigint_len);
    outBuf[j++] = alphabet58[b58_index];
    bigint_len = divide_bigint(bigint, B58_ENCODE_BASE, bigint_len);
    outcount += 1;
  }

  /* -------------------------------------------------------------
  #
  #     reverse string
  #
  ------------------------------------------------------------- */
  left = 0;
  right = outcount - 1;
  while (left < right) {
    char temp = outBuf[left];
    outBuf[left] = outBuf[right];
    outBuf[right] = temp;
    left++;
    right--;
  }

  /* -------------------------------------------------------------
  #
  #     write string
  #
  ------------------------------------------------------------- */
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
  memset(inBuf, 0, ENCODER_INBUFFSIZE_58);
  memset(outBuf, 0, ENCODER_OUTBUFFSIZE_58);
  memset(hexstring, 0, (nread * 2) + 1);
  outcount = 0;

  write(STDOUT_FILENO, "\n", 1);
}


