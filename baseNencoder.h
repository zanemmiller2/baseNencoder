// ------------ includes ------------
#include <stdint.h>     // typedef uint8_t, UINT8_MAX
#include <sys/types.h>  // ssize_t, uint8_t
#include <fcntl.h>      // open, O_RDONLY
#include <unistd.h>     // read, write
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit
#include <string.h>     // strcmp
#include <stdbool.h>    // bool
// ------------ global defines ------------
#define MAXLINE76 76
#define MAXLINE60 60


// Check that uint8_t type exists
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif

// ------------ globals -------------
static char const alphabet64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char const alphabet58[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz=";
static char const alphabet32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
static char const alphabet16[] = "0123456789ABCDEF=";

// ----- function declarations ------
void encodeBase64(int fd);
void encodeBase58(int fd);
void encodeBase32(int fd);
void encodeBase16(int fd);
void decodeBase64(int fd);
void decodeBase32(int fd);
void decodeBase16(int fd);
void parsecl(int argc, char* argv[], char** option, char** baseN, char** file_name);
void writedecoded(int fd_out, char* output, int outputBytes);