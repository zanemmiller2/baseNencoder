// --------- includes -------------
#include "baseNencoder.h"

int main(int argc, char* argv[]) {
  int fd = STDIN_FILENO;
  char* option = "-n";
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

  /* ------------------------ Encode/Decode ------------------------ */
  /* -------- help --------*/
  if (strcmp(option, "-h") == 0) {
    char printbuf[100];
    ssize_t nread;
    while ((nread = read(fd, printbuf, 100)) != 0) {
      write(STDOUT_FILENO, printbuf, nread);
    }
    exit(0);
  }
  /* ------ BASEZ85 ------ */
  if (strcmp(baseN, "z85") == 0 || strcmp(baseN, "Z85") == 0) {
    // decode base64
    if (strcmp(option, "-d") == 0) {
      decodeBasez85(fd);
    }
    // encode base64
    else {
      encodeBasez85(fd);
    }
  }
  /* ------ BASE64 ------ */
  else if (strcmp(baseN, "64") == 0) {
    // decode base64
    if (strcmp(option, "-d") == 0) {
      decodeBase64(fd);
    }
    // encode base64
    else {
      encodeBase64(fd);
    }
  }

  /* ------ BASE58 ------ */
  else if (strcmp(baseN, "58") == 0) {
    // decode base58
    if (strcmp(option, "-d") == 0) {
      decodeBase58(fd);
    }
    // encode base58
    else {
      encodeBase58(fd);
    }
  }

  /* ------ BASE32 ------ */
  else if (strcmp(baseN, "32") == 0) {
    if (strcmp(option, "-d") == 0) {
      // decode base32
      decodeBase32(fd);
    }
    // encode base32
    else {
      encodeBase32(fd);
    }
  }

  /* ------ BASE16 ------ */
  else if (strcmp(baseN, "16") == 0) {
    // decode base16
    if (strcmp(option, "-d") == 0) {
      decodeBase16(fd);
    }
    // encode base16
    else {
      encodeBase16(fd);
    }
  }
  else {
    printf("UNKNOWN ERROR...\n");
  }
  return 0;
}





