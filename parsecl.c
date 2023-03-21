// --------- includes -------------
#include "baseNencoder.h"

/* parsecl:   parses the command line arguments and assigns to respective variable */
void parsecl(int argc, char* argv[], char** option, char** baseN, char** file_name) {
  // invalid number of arguments -- can only decode 1 file
  if (argc > 4) {
    printf("error: invalid number of arguments\n");
    exit(-1);
  }

  // encode/decode in provided base from file path
  if (argc == 4) {
    if ((strcmp(argv[1], "-n") == 0 | strcmp(argv[1], "-d") == 0) && (strcmp(argv[2], "64") == 0 || strcmp(argv[2], "58") == 0 || strcmp(argv[2], "32") == 0 || strcmp(argv[2], "16") == 0)) {
      *option = argv[1];
      *baseN = argv[2];
      *file_name = argv[3];
    }
    else {
      printf("error: %s not supported with %s\n", argv[1], argv[2]);
      exit(-1);
    }
  }

  else if (argc == 3) {
    // encode/decode in provided baseN from file path
    if ((strcmp(argv[1], "-n") == 0 | strcmp(argv[1], "-d") == 0) && (strcmp(argv[2], "64") == 0 || strcmp(argv[2], "58") == 0 || strcmp(argv[2], "32") == 0 || strcmp(argv[2], "16") == 0)) {
      *option = argv[1];
      *baseN = argv[2];
    }
    // decode in default base64 from provided file path
    else if (strcmp(argv[1], "-d") == 0){
      *option = argv[1];
      *file_name = argv[2];
    }
    else {
      printf("error: %s not supported with %s\n", argv[1], argv[2]);
      exit(-1);
    }
  }

  
  else if (argc == 2) {
    if (strcmp(argv[1], "-n") == 0) {
      printf("error: missing baseN value and file\n");
      exit(-1);
    }
    // decode default base64 from stdin
    else if(strcmp(argv[1], "-d") == 0){
      *option = argv[1];
    }
    // default encode in base64 from file path
    else{
      *file_name = argv[1];
    }
  }
}