#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'
BLUE='\033[0;36m'
CHECKMARK='\t\xE2\x9C\x94'

echo -e "${BLUE}COMPILING SOURCE CODE............${NC}"
gcc ../main.c ../base16encoder.c ../base16decoder.c ../base32encoder.c ../base32decoder.c ../base58encoder.c ../base58decoder.c ../base64encoder.c ../base64decoder.c ../basez85encoder.c ../parsecl.c ../writedecoded.c -o baseNencodetest


gcc ../main.c ../base16encoder.c ../base16decoder.c ../base32encoder.c ../base32decoder.c ../base58encoder.c ../base58decoder.c ../base64encoder.c ../base64decoder.c ../basez85encoder.c ../parsecl.c ../writedecoded.c -o ../baseNencode

##################################################
#		BASEz85 TESTS
##################################################

# make basez85 testfiles
# head -c1000 /dev/random > testfileZ85_raw

echo -e "${BLUE}TESTING BASEZ85 ENCODING FROM FILE (baseNencode -n z85 testfileZ85_raw)${NC}"
result=$(cmp -l <(./baseNencodetest -n z85 testfileZ85_raw) <(cat encodedbaseZ85))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASEZ85 ENCODING FROM FILE (baseNencode -n Z85 testfileZ85_raw)${NC}"
result=$(cmp -l <(./baseNencodetest -n Z85 testfileZ85_raw) <(cat encodedbaseZ85))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;


################################################
#		BASE64 TESTS
################################################

# make base64 test files
# head -c1000 /dev/random > testfile64_raw
# base64 testfile64_raw > encodedbase64

echo -e "${BLUE}TESTING BASE64 ENCODING FROM FILE (baseNencode testfile64_raw)${NC}"
result=$(cmp -l <(./baseNencodetest testfile64_raw) <(base64 testfile64_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE64 ENCODING FROM FILE (baseNencode -n 64 testfile64_raw)${NC}"
result=$(cmp -l <(./baseNencodetest -n 64 testfile64_raw) <(base64 testfile64_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE64 DECODING FROM FILE (baseNencode -d 64 encodedbase64)${NC}"
result=$(cmp -l <(./baseNencodetest -d 64 encodedbase64) <(base64 -d encodedbase64))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE64 DECODING FROM FILE (baseNencode -d encodedbase64)${NC}"
result=$(cmp -l <(./baseNencodetest -d encodedbase64) <(base64 -d encodedbase64))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

##################################################
#		BASE58 TESTS
##################################################

# make base58 testfiles
# head -c1000 /dev/random > testfile58_raw

echo -e "${BLUE}TESTING BASE58 ENCODING FROM FILE (baseNencode -n 58 testfile58_raw)${NC}"
result=$(cmp -l <(./baseNencodetest -n 58 testfile58_raw) <(cat encodedbase58))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE58 DECODING FROM FILE (baseNencode -d 58 encodedbase58)${NC}"
result=$(cmp -l <(./baseNencodetest -d 58 encodedbase58) <(cat testfile58_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

##################################################
#		BASE32 TESTS
##################################################

# make base32 testfiles
# head -c1000 /dev/random > testfile32_raw
# base32 testfile32_raw > encodedbase32

echo -e "${BLUE}TESTING BASE32 ENCODING FROM FILE (baseNencode -n 32 testfile32_raw)${NC}"
result=$(cmp -l <(./baseNencodetest -n 32 testfile32_raw) <(base32 testfile32_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE32 DECODING FROM FILE (baseNencode -d 32 encodedbase32)${NC}"
result=$(cmp -l <(./baseNencodetest -d 32 encodedbase32) <(base32 -d encodedbase32))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

##################################################
#		BASE16 TESTS
##################################################

# make base16 test files 
# head -c1000 /dev/random > testfile16_raw
# (cat testfile16_raw | xxd -p -u) > encodedbase16

echo -e "${BLUE}TESTING BASE16 ENCODING FROM FILE (baseNencode -n 16 testfile16_raw)${NC}"
result=$(cmp -l <(./baseNencodetest -n 16 testfile16_raw) <(cat testfile16_raw | xxd -p -u))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE16 DECODING FROM FILE (baseNencode -d 16 encodedbase16)${NC}"
result=$(cmp -l <(./baseNencodetest -d 16 encodedbase16) <(cat testfile16_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;


