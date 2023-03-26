// --------- includes -------------
#include "baseNencoder.h"
// ------------ defines ------------
#define DECODER_INBUFFSIZE_58 355    // 256 * 1.38 + '\0'
#define DECODER_OUTBUFFSIZE_58 257    // 256 + '\0'
#define B58_DECODE_BASE 256


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


void convert_b58_to_string_int(int b58indexes[], unsigned int bigint[], int bigint_len, int num_indexes) {
  int i, bi_i, remainder, carry;

  for (i = 0; i < num_indexes - 1; i++) {
    // add it to the front
    bigint[0] += b58indexes[i];

    // reduce and carry
    for (bi_i = 0; bi_i < bigint_len - 1; bi_i++) {
      remainder = bigint[bi_i] % 10;
      carry = bigint[bi_i] / 10;
      bigint[bi_i] = remainder;
      bigint[bi_i + 1] += carry;
    }

    // multiply everything by 58
    for (bi_i = 0; bi_i < bigint_len - 1; bi_i++) {
      bigint[bi_i] *= 58;
    }

    // reduce and carry
    for (bi_i = 0; bi_i < bigint_len - 1; bi_i++) {
      remainder = bigint[bi_i] % 10;
      carry = bigint[bi_i] / 10;
      bigint[bi_i] = remainder;
      bigint[bi_i + 1] += carry;
    }
  }

  // add last index value
  bigint[0] += b58indexes[i];

  // reduce and carry
  for (bi_i = 0; bi_i < bigint_len - 1; bi_i++) {
    remainder = bigint[bi_i] % 10;
    carry = bigint[bi_i] / 10;
    bigint[bi_i] = remainder;
    bigint[bi_i + 1] += carry;
  }

  // reverse bigint
  int left = 0;
  int right = bigint_len - 1;
  while (left < right) {
    int temp = bigint[left];
    bigint[left] = bigint[right];
    bigint[right] = temp;
    left++;
    right--;
  }
}


/* decodeBase58:  decodes data in base58 format to ascii */
void decodeBase58(int fd_in) {
  size_t nread, nwrite;
  int i, j, outcount;
  uint8_t inBuf[DECODER_INBUFFSIZE_58], outBuf[DECODER_OUTBUFFSIZE_58];
  int indexes[DECODER_INBUFFSIZE_58];
  outcount = 0;


  /* -------------------------------------------------------------
  #
  #     read in up to 11 encoded bytes at a time
  #
  ------------------------------------------------------------- */
  nread = read(fd_in, inBuf, DECODER_INBUFFSIZE_58 - 1);
  if (nread < 0) {
    perror("error");
    exit(-1);
  }
  if (nread >= DECODER_INBUFFSIZE_58 - 1) {
    printf("WARNING: ONLY DECODING THE FIRST 256 BYTES. IF INPUT IS LARGER THAN 255 BYTES: DEFINITELY ERRORS\n");
  }

  /* -------------------------------------------------------------
  #
  #     convert the input bytes to their base58 index values
  #
  ------------------------------------------------------------- */
  for (j = 0; j < nread - 1; j++) {
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
  int bigint_len = (nread - 1) * 2;
  unsigned int bigint[bigint_len];
  for (i = 0; i < bigint_len; i++) {
    bigint[i] = 0;
  }
  convert_b58_to_string_int(indexes, bigint, bigint_len, nread - 1);

  /* -------------------------------------------------------------
  #
  #     convert base10 integer to ascii string
  #
  ------------------------------------------------------------- */
  j = 0;
  while (bigint_len > 0) {
    outBuf[j++] = mod_bigint(bigint, B58_DECODE_BASE, bigint_len);
    bigint_len = divide_bigint(bigint, B58_DECODE_BASE, bigint_len);
    outcount += 1;
  }

  /* -------------------------------------------------------------
  #
  #     reverse the output
  #
  ------------------------------------------------------------- */
  int left = 0;
  int right = outcount - 1;
  while (left < right) {
    char temp = outBuf[left];
    outBuf[left] = outBuf[right];
    outBuf[right] = temp;
    left++;
    right--;
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
}

