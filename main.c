// --------- includes -------------
#include "baseNencoder.h"

void main(int argc, char* argv[]) {
  int fd = STDIN_FILENO;
  char* option = "-e";
  char* baseN = "64";
  char* file_name = "-";

  /* ------------- Parse Command Line Arguments ------------- */
  parsecl(argc, argv, &option, &baseN, &file_name);


  /* ----------------- Get Input Descriptor ----------------- */
  if ((argc > 1) && (strcmp(file_name, "-") != 0)) {
    fd = open(file_name, O_RDONLY);
    if (fd < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }
  }

  /* ------------------------ Encode ------------------------ */
  if (strcmp(baseN, "32") == 0) {
    encodeBase32(fd);
  }
  else if (strcmp(baseN, "16") == 0) {
    if (strcmp(option, "-d") == 0) {
      decodeBase16(fd);
    }
    else {
      encodeBase16(fd);
    }
  }
  // default base64
  else {
    if (strcmp(option, "-d") == 0) {
      decodeBase64(fd);
    }
    else {
      encodeBase64(fd);
    }
    // return 0;
  }
}





