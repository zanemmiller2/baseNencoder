#include "baseNencoder.h"

/* parsecl:   parses the command line arguments and assigns to respective variable */
void parsecl(int argc, char* argv[], char** option, char** baseN, char** file_name) {
  // invalid number of arguments -- can only decode 1 file
  if (argc > 4) {
    printf("error: invalid number of arguments\n");
    exit(-1);
  }

  // option, baseN, and fileName only
  if (argc == 4) {
    if (strcmp(argv[1], "-n") == 0 && (strcmp(argv[2], "64") == 0 || strcmp(argv[2], "32") == 0 || strcmp(argv[2], "16") == 0)) {
      *option = argv[1];
      *baseN = argv[2];
      *file_name = argv[3];
    }
    else {
      printf("error: %s not supported with %s\n", argv[1], argv[2]);
      exit(-1);
    }
  }
  // option, baseN only
  else if (argc == 3) {
    if (strcmp(argv[1], "-n") == 0 && (strcmp(argv[2], "64") == 0 || strcmp(argv[2], "32") == 0 || strcmp(argv[2], "16") == 0)) {
      *option = argv[1];
      *baseN = argv[2];
    }
    else {
      printf("error: %s not supported with %s\n", argv[1], argv[2]);
      exit(-1);
    }
  }
  // file_name only
  else if (argc == 2) {
    if (strcmp(argv[1], "-n") == 0) {
      printf("error: missing baseN value and file\n");
      exit(-1);
    }
    
    *file_name = argv[1];
  }
}