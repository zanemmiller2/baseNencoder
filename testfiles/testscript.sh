#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'
BLUE='\033[0;36m'
CHECKMARK='\t\xE2\x9C\x94'


##################################################
#		BASEz85 TESTS
##################################################

# make basez85 testfiles
# head -c1000 /dev/random > testfileZ85_raw

echo -e "${BLUE}TESTING BASEZ85 ENCODING FROM FILE (baseNencode -n z85 testfileZ85_raw)${NC}"
result=$(cmp -l <(./baseNencode -n z85 testfiles/testfileZ85_raw) <(cat testfiles/encodedbaseZ85))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASEZ85 ENCODING FROM FILE (baseNencode -n Z85 testfileZ85_raw)${NC}"
result=$(cmp -l <(./baseNencode -n Z85 testfiles/testfileZ85_raw) <(cat testfiles/encodedbaseZ85))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASEZ85 DECODING FROM FILE (baseNencode -d Z85 encodedbaseZ85)${NC}"
result=$(cmp -l <(./baseNencode -d Z85 testfiles/encodedbaseZ85) <(cat testfiles/testfileZ85_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASEZ85 DECODING FROM FILE (baseNencode -d z85 encodedbaseZ85)${NC}"
result=$(cmp -l <(./baseNencode -d z85 testfiles/encodedbaseZ85) <(cat testfiles/testfileZ85_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;


################################################
#		BASE64 TESTS
################################################

# make base64 test files
# head -c1000 /dev/random > testfile64_raw
# base64 testfile64_raw > encodedbase64

echo -e "${BLUE}TESTING BASE64 ENCODING FROM FILE (baseNencode testfile64_raw)${NC}"
result=$(cmp -l <(./baseNencode testfiles/testfile64_raw) <(base64 testfiles/testfile64_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE64 ENCODING FROM FILE (baseNencode -n 64 testfile64_raw)${NC}"
result=$(cmp -l <(./baseNencode -n 64 testfiles/testfile64_raw) <(base64 testfiles/testfile64_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE64 DECODING FROM FILE (baseNencode -d 64 encodedbase64)${NC}"
result=$(cmp -l <(./baseNencode -d 64 testfiles/encodedbase64) <(base64 -d testfiles/encodedbase64))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE64 DECODING FROM FILE (baseNencode -d encodedbase64)${NC}"
result=$(cmp -l <(./baseNencode -d testfiles/encodedbase64) <(base64 -d testfiles/encodedbase64))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

##################################################
#		BASE58 TESTS
##################################################

# make base58 testfiles
# head -c1000 /dev/random > testfile58_raw

echo -e "${BLUE}TESTING BASE58 ENCODING FROM FILE (baseNencode -n 58 testfile58_raw)${NC}"
result=$(cmp -l <(./baseNencode -n 58 testfiles/testfile58_raw) <(cat testfiles/encodedbase58))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE58 DECODING FROM FILE (baseNencode -d 58 encodedbase58)${NC}"
result=$(cmp -l <(./baseNencode -d 58 testfiles/encodedbase58) <(cat testfiles/testfile58_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

##################################################
#		BASE32 TESTS
##################################################

# make base32 testfiles
# head -c1000 /dev/random > testfile32_raw
# base32 testfile32_raw > encodedbase32

echo -e "${BLUE}TESTING BASE32 ENCODING FROM FILE (baseNencode -n 32 testfile32_raw)${NC}"
result=$(cmp -l <(./baseNencode -n 32 testfiles/testfile32_raw) <(base32 testfiles/testfile32_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE32 DECODING FROM FILE (baseNencode -d 32 encodedbase32)${NC}"
result=$(cmp -l <(./baseNencode -d 32 testfiles/encodedbase32) <(base32 -d testfiles/encodedbase32))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

##################################################
#		BASE16 TESTS
##################################################

# make base16 test files 
# head -c1000 /dev/random > testfile16_raw
# (cat testfile16_raw | xxd -p -u) > encodedbase16

echo -e "${BLUE}TESTING BASE16 ENCODING FROM FILE (baseNencode -n 16 testfile16_raw)${NC}"
result=$(cmp -l <(./baseNencode -n 16 testfiles/testfile16_raw) <(cat testfiles/testfile16_raw | xxd -p -u))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;

echo -e "${BLUE}TESTING BASE16 DECODING FROM FILE (baseNencode -d 16 encodedbase16)${NC}"
result=$(cmp -l <(./baseNencode -d 16 testfiles/encodedbase16) <(cat testfiles/testfile16_raw))
if ["$result" == ""]; then echo -e "${GREEN}SUCCESS${CHECKMARK}${NC}\n";
else echo -e "${RED}ERROR: $result${NC}\n"; fi;


