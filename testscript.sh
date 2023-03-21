#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'
BLUE='\033[0;36m'
gcc main.c base16encoder.c base32encoder.c base64encoder.c base64decoder.c base16decoder.c  parsecl.c -o baseNencode

################################################
#		BASE64 TESTS
################################################
head -c1000 /dev/random > testfile64_raw
base64 testfile64_raw > encodedbase64
echo -e "${BLUE}Testing base64 encoding from file with no flags${NC}"
result=$(cmp -l <(./baseNencode testfile64_raw) <(base64 testfile64_raw))
if ["$result" == ""]; then echo -e "${GREEN}Success${NC}";
else echo -e "${RED}Error: $result${NC}"; fi;

echo -e "${BLUE}Testing base64 encoding from file with -n 64 flag${NC}"
result=$(cmp -l <(./baseNencode -n 64 testfile64_raw) <(base64 testfile64_raw))
if ["$result" == ""]; then echo -e "${GREEN}Success${NC}";
else echo -e "${RED}Error: $result${NC}"; fi;

echo -e "${BLUE}Testing base64 decoding from file with -d 64 flags${NC}"
result=$(cmp -l <(./baseNencode -d 64 encodedbase64) <(base64 -d encodedbase64))
if ["$result" == ""]; then echo -e "${GREEN}Success${NC}";
else echo -e "${RED}Error: $result${NC}"; fi;

echo -e "${BLUE}Testing base64 decoding from file with -d flag only${NC}"
result=$(cmp -l <(./baseNencode -d encodedbase64) <(base64 -d encodedbase64))
if ["$result" == ""]; then echo -e "${GREEN}Success${NC}";
else echo -e "${RED}Error: $result${NC}"; fi;


##################################################
#		BASE16 TESTS
##################################################
head -c1000 /dev/random > testfile16_raw
(cat testfile16_raw | xxd -p -u) > encodedbase16

echo -e "${BLUE}Testing base16 encoding from file with -n 16 flag${NC}"
result=$(cmp -l <(./baseNencode -n 16 testfile16_raw) <(cat testfile16_raw | xxd -p -u))
if ["$result" == ""]; then echo -e "${GREEN}Success${NC}";
else echo -e "${RED}Error: $result${NC}"; fi;

echo -e "${BLUE}Testing base16 decoding from file${NC}"
result=$(cmp -l <(./baseNencode -d 16 encodedbase16) <(cat testfile16_raw))
if ["$result" == ""]; then echo -e "${GREEN}Success${NC}";
else echo -e "${RED}Error: $result${NC}"; fi;

##################################################
#		BASE32 TESTS
##################################################
head -c1000 /dev/random > testfile32_raw

echo -e "${BLUE}Testing base32 encoding from file with -n 32 flag${NC}"
result=$(cmp -l <(./baseNencode -n 32 testfile32_raw) <(base32 testfile32_raw))
if ["$result" == ""]; then echo -e "${GREEN}Success${NC}";
else echo -e "${RED}Error: $result${NC}"; fi;


