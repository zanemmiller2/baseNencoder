// --------- includes -------------
#include "baseNencoder.h"

int main(int argc, char* argv[]) {
  int fd = STDIN_FILENO;
  char* option;
  char* baseN = "64";
  char* file_name = "-";
  int i;

  /* ------------- Parse Command Line Arguments ------------- */
  // invalid number of arguments -- can only decode 1 file
  if (argc > 4) {
    printf("error: invalid number of arguments\n");
    exit(-1);
  }

  // option, baseN, and fileName only
  if (argc == 4) {
    if (strcmp(argv[1], "-n") == 0 && (strcmp(argv[2], "32") == 0 || strcmp(argv[2], "64") == 0)) {
      option = argv[1];
      baseN = argv[2];
      file_name = argv[3];
    }
    else {
      printf("error: invalid arguments\n");
      exit(-1);
    }
  }
  // option, baseN only
  else if (argc == 3) {
    if (strcmp(argv[1], "-n") == 0 && (strcmp(argv[2], "32") == 0 || strcmp(argv[2], "64") == 0)) {
      option = argv[1];
      baseN = argv[2];
    }
    else {
      printf("error: invalid arguments\n");
      exit(-1);
    }
  }
  // file_name only
  else if (argc == 2) {
    if (strcmp(argv[1], "-n") == 0) {
      printf("error: missing baseN value and file\n");
      exit(-1);
    }
    file_name = argv[1];
  }


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
  // default base64
  else{
    encodeBase64(fd);
  }

  return 0;
}


