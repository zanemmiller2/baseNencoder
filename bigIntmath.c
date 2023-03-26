// --------- includes -------------
#include "baseNencoder.h"

/* mod_bigint:
      Returns the modulus of an arbitrarily large integer and an integer.
*/
int mod_bigint(unsigned int bigint[], int divisor, int bigint_len) {
  int res = 0;
  for (int i = 0; i < bigint_len; i++) {
    res = ((res * 10) + bigint[i]) % divisor;
  }
  return res;
}

/* divide_bigint:
      divides an arbitrarily large integer by an integer divisor.
      Function modifies bigint[] with the resulting quotient and
      returns the number of digits in the new quotient.
*/
int divide_bigint(unsigned int bigint[], int divisor, int bigint_len) {
  int i, j;
  int dividend = bigint[0];

  i = 0;
  while (i < bigint_len && dividend < divisor) {
    bigint[i] = 0;
    dividend = dividend * 10 + bigint[++i];
  }

  j = 0;
  while (i < bigint_len) {
    bigint[j++] = dividend / divisor;
    dividend = ((dividend % divisor) * 10) + bigint[++i];
  }
  // return len of new big int
  if (j == 0) {
    return 0;
  }
  return j;
}