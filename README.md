# baseNencoder

## SUMMARY

_base64encode [OPTION] [FILE]_

&nbsp;&nbsp;&nbsp;&nbsp;Options:

<ul>
-n [BASE] encodes in given base number.

<ul>Defaults to base64 encoding if -n flag not supplied. Base must be 16, 32, 58, 64, z85 (Z85).</ul><br>

-d [BASE] decodes in given base number.

<ul>Defaults to base64 decoding if -d flag not supplied. or -d flag supplied with no [BASE]. Base must be 16, 32, 58, 64, z85 (Z85).</ul><br>

</ul>

<br/>

## DESCRIPTION:

C program that encodes and decodes data in base 16, 32, 58 (decoding not supported yet), or 64 and prints to stdout.

Takes at most 1 file path (or reads from standard input if no file path or file argument is "-").

<ins>base16 encoding</ins> is completed by taking a block of one octet (8 bit string) and converting it to two BASE16 characters.

<ins>base16 decoding</ins> is completed by taking a block of two octets (16 bit string) of base16 characters and converting it to one ASCII characters.

<ins>base32 encoding</ins> is completed by taking a block of five octets (40 bit string) and converting it to eight BASE32 characters.

<ins>base32 decoding</ins> is completed by taking a block of eight BASE32 characters (64 bit string) and converting it to five ASCII characters.

<ins>base58 encoding</ins> is completed by reading in a complete stream of data -> converting it to a hexstring representing each byte of the input -> converting the hex string to an array of integers representing the digits of the integer value of the concatenated hexstring -> converting the arbitrarily large "integer" to its b58 representation. The program is limited to encoding at most 256 bytes.

<ins>base58 decoding</ins> is completed by reading in data in blocks of up to eleven encoded bytes at a time and converting it to up to eight ascii characters. The program converts eleven bytes of input at a time do to the size limitations of unsigned long long integers in C.

<ins>base64 encoding</ins> is completed by taking a block of three octets (24 bit string) and converting it to four BASE64 characters.

<ins>base64 decoding</ins> is completed by taking a block of four base64 characters and converting it to three ASCII characters.

<ins>basez85 encoding</ins> is completed by taking a block of four octets (32 bit DOUBLE WORD) and converting it to five z85 characters. Input lengths less than 4 bytes are padded with "00" for conversion and then disregarded in the output.

<ins>basez85 decoding</ins> is completed by reading a block of five z85 characters and converting it to four ASCII characters. New line characters in the input are disregarded in decoding.

Prints to standard output wrapping to a new line every 76 characters. Pads incomplete output strings with "=".

**(Note: this base58 implementation is going to vary slightly from other versions due to the max unsigned integer limitations)**

<br/>

## COMPILE:

> ```bash
> gcc main.c base16encoder.c base16decoder.c base32encoder.c base32decoder.c base58encoder.c base58decoder.c base64encoder.c base64decoder.c basez85encoder.c basez85decoder.c parsecl.c writedecoded.c -o baseNencode
> ```

<br/>

## EXAMPLES:

A test file is supplied for testing generated with:

> ```bash
> head -c1000000 /dev/random > testfile
> ```

- #### to run sample test file:

  > defaults to base64 with no flags:
  >
  > ```bash
  > ./baseNencode testfile
  > ```

  > ```bash
  > ./baseNencode -n 16 testfile
  > ```

  > ```bash
  > ./baseNencode -n 32 testfile
  > ```

  > ```bash
  > ./baseNencode -n 58 testfile
  > ```

  > ```bash
  > ./baseNencode -n 64 testfile
  > ```

  > ```bash
  > ./baseNencode -n z85 testfile
  > ```

  > ```bash
  > ./baseNencode -n Z85 testfile
  > ```

- #### to ENCODE from stdandard input (base16, base32, base58, base64, basez85):

  > defaults to base64 with no flags:
  >
  > ```bash
  > ./baseNencode
  > ```

  > ```bash
  > ./baseNencode -n 16
  > ```

  > ```bash
  > ./baseNencode -n 32
  > ```

  > ```bash
  > ./baseNencode -n 58
  > ```

  > ```bash
  > ./baseNencode -n 64
  > ```

  > ```bash
  > ./baseNencode -n z85
  > ```

  > ```bash
  > ./baseNencode -n Z85
  > ```

- #### to DECODE from stdandard input (base16, base32, base58, base64):

  > defaults to base 64 with no flags
  >
  > ```bash
  > ./baseNencode -d
  > ```

  > ```bash
  > ./baseNencode -d 16
  > ```

  > ```bash
  > ./baseNencode -d 32
  > ```

  > ```bash
  > ./baseNencode -d 58
  > ```

  > ```bash
  > ./baseNencode -d 64
  > ```

  > ```bash
  > ./baseNencode -d z85
  > ```

  > ```bash
  > ./baseNencode -d Z85
  > ```

- #### to compare outputs with builtin function:

  > base -z85 decoding
  >
  > ```bash
  > cmp -l <(./baseNencode -d z85 encodedbaseZ85) <(cat testfileZ85_raw)
  > ```

  > base -Z85 decoding (same as z85 just case insensitive)
  >
  > ```bash
  > cmp -l <(./baseNencode -d Z85 encodedbaseZ85) <(cat testfileZ85_raw)
  > ```

  > base -z85 encoding
  >
  > ```bash
  > cmp -l <(./baseNencode -n z85 testfileZ85_raw) <(cat encodedbaseZ85)
  > ```

  > base -Z85 encoding (same as z85 just case insensitive)
  >
  > ```bash
  > cmp -l <(./baseNencode -n Z85 testfileZ85_raw) <(cat encodedbaseZ85)
  > ```

  > base64 encoding (defaults to base 64 encoding with no flags):
  >
  > ```bash
  > cmp -l <(./baseNencode testfile64_raw) <(base64 testfile64_raw)
  > ```

  > base64 encoding
  >
  > ```bash
  > cmp -l <(./baseNencode -n 64 testfile64_raw) <(base64 testfile64_raw)
  > ```

  > base64 decoding (defaults to base 64 with no base after -d flag):
  >
  > ```bash
  > cmp -l <(/.baseNencode -d encodedbase64) <(base64 -d encodedbase64)
  > ```

  > base64 decoding
  >
  > ```bash
  > cmp -l <(/.baseNencode -d 64 encodedbase64) <(base64 -d encodedbase64)
  > ```

  > base32 encoding
  >
  > ```bash
  > cmp -l <(./baseNencencodedbaseZ85ode -n 32 testfile32_raw) <(base32 testfile32_raw)
  > ```

  > base32 decoding
  >
  > ```bash
  > cmp -l <(./baseNencode -d 32 encodedbase32) <(base32 encodedbase32)
  > ```

  > base16 encoding
  >
  > ```bash
  > cmp -l <(./baseNencode -n 16 testfile16_raw) <(cat testfile16_raw | xxd -p -u)
  > ```

  > base16 decoding
  >
  > ```bash
  > cmp -l <(./baseNencode -d 16 encodedbase16) <(cat testfile16_raw)
  > ```

<br/>

## NOTES:

Planned expansion is to include base32Hex encoding, encoding/decoding in other base systems and --wrap and --ignore-garbage flags.

<br/>

## AUTHOR:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Written by:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Zane Miller  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<millerzanem@gmail.com>  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<https://github.com/zanemmiller2/baseNencoder>
