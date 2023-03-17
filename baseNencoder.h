#include <stdint.h>     // typedef uint8_t, UINT8_MAX
#include <sys/types.h>  // ssize_t, uint8_t
#include <fcntl.h>      // open, O_RDONLY
#include <unistd.h>     // read, write
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit
#include <string.h>     // strcmp

#define MAXLINE 76

// Check that uint8_t type exists
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif

// ---------- globals ---------------
static char const alphabet64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char const alphabet32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";

// ----- function declarations ------
void encodeBase64(int fd);
void encodeBase32(int fd);