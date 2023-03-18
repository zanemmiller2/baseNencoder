// --------- includes -------------
#include "baseNencoder.h"

int main(int argc, char* argv[]) {
  int fd = STDIN_FILENO;
  char* option;
  char* baseN = "64";
  char* file_name = "-";

  /* ------------- Parse Command Line Arguments ------------- */
  parsecl(argc, argv, &option, &baseN, &file_name);
  printf("option: %s, base: %s, filename: %s\n", option, baseN, file_name);


  /* ----------------- Get Input Descriptor ----------------- */
  if ((argc > 1) && (strcmp(file_name, "-") != 0)) {
    fd = open(file_name, O_RDONLY);
    if (fd < 0) {
      perror("error");  // invalid file descriptor
      exit(-1);
    }
  }

  /* ------------------------ Encode ------------------------ */
  if(strcmp(baseN, "32") == 0){
    encodeBase32(fd);
  }
  if(strcmp(baseN, "16") == 0){
    encodeBase16(fd);
  }
  // default base64
  else{
    encodeBase64(fd);
  }

  return 0;
}




