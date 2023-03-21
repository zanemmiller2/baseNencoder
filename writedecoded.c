// --------- includes -------------
#include "baseNencoder.h"

void writedecoded(int fd_out, char* output, int outputBytes) {
  size_t nwrite;
  for (size_t offset = 0; offset < outputBytes;) {
    if ((nwrite = write(fd_out, offset + (char*)output, outputBytes - offset)) < 0) {
      perror("error");
      exit(-1);
    }
    offset += nwrite;
  }
}