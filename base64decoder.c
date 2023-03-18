// --------- includes -------------
#include "baseNencoder.h"
// --------- defines ---------------
#define INBUFFSIZE64 4
#define OUTBUFFSIZE64 3

typedef struct {
  char* key;
  int index;
} item;

item* linear_search(item* items, size_t size, const char* key) {
  for (size_t i = 0; i < size; i++) {
    if (strcmp(items[i].key, key) == 0) {
      return &items[i];
    }
  }
  return NULL;
}

const item base64indicies[] = { {"A", 0}, {"B", 1}, {"C", 2}, {"D", 3}, {"E", 4}, {"F", 5}, {"G", 6}, {"H", 7}, 
                 {"I", 8}, {"J", 9}, {"K", 10}, {"L", 11}, {"M", 12}, {"N", 13}, {"O", 14}, {"P", 15},
                 {"Q", 16}, {"R", 17}, {"S", 18}, {"T", 19}, {"U", 20}, {"V", 21}, {"W", 22}, {"X", 23},
                 {"Y", 24}, {"Z", 25}, {"a", 26}, {"b", 27}, {"c", 28}, {"d", 29}, {"e", 30}, {"f", 31},
                 {"g", 32}, {"h", 33}, {"i", 34}, {"j", 35}, {"k", 36}, {"l", 37}, {"m", 38}, {"n", 39},
                 {"o", 40}, {"p", 41}, {"q", 42}, {"r", 43}, {"s", 44}, {"t", 45}, {"u", 46}, {"v", 47},
                 {"w", 48}, {"x", 49}, {"y", 50}, {"z", 51}, {"0", 52}, {"1", 53}, {"2", 54}, {"3", 55},
                 {"4", 56}, {"5", 57}, {"6", 58}, {"7", 59}, {"8", 60}, {"9", 61}, {"+", 62}, {"/", 63}, {"=", 64}};
const ssize_t num_chars = sizeof(base64indicies) / sizeof(item);

/* base64 decoder:  decodes data in base64 format to ascii */
void decodeBase64(int fd){
  ssize_t nread, nwrite;
  int count = 0, i;
  uint8_t inBuf[INBUFFSIZE64], outBuf[OUTBUFFSIZE64];

  item* found = linear_search(base64indicies, num_chars, "G");
  printf("%d\n", found->index);
}
