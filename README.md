# baseNencoder

## SUMMARY

_base64encode [OPTION] [FILE]_

&nbsp;&nbsp;&nbsp;&nbsp;Options:

<ul>
-n [BASE] encodes in given base number.

<ul>Defaults to base64 encoding if -n flag not supplied. Base must be 16, 32, 58, or 64.</ul><br>

-d [BASE] decodes in given base number.

<ul>Defaults to base64 decoding if -d flag not supplied. or -d flag supplied with no [BASE]. Base must be 16, 32, 58 or 64.</ul><br>

</ul>

<br/>

## DESCRIPTION:

C program that encodes and decodes data in base 16, 32, 58 (decoding not supported yet), or 64 and prints to stdout.

Takes at most 1 file path (or reads from standard input if no file path or file argument is "-").

<ins>base16 encoding</ins> is completed by taking a block of one octet (8 bit string) and converting it to two BASE16 characters.

<ins>base16 decoding</ins> is completed by taking a block of two octets (16 bit string) of base16 characters and converting it to one ASCII characters.

<ins>base32 encoding</ins> is completed by taking a block of five octets (40 bit string) and converting it to eight BASE32 characters.

<ins>base32 decoding</ins> is completed by taking a block of eight BASE32 characters (64 bit string) and converting it to five ASCII characters.

<ins>base58 encoding</ins> is completed by reading in data in blocks of eight bytes and converting it to eleven base58 characters. The program converts eight bytes of input at a time do to the size limitations of unsigned long long integers in C.

<ins>base58 decoding</ins> is completed by reading in data in blocks of up to eleven encoded bytes at a time and converting it to up to eight ascii characters. The program converts eleven bytes of input at a time do to the size limitations of unsigned long long integers in C.

<ins>base64 encoding</ins> is completed by taking a block of three octets (24 bit string) and converting it to four BASE64 characters.

<ins>base64 decoding</ins> is completed by taking a block of four base64 characters and converting it to three ASCII characters.

Prints to standard output wrapping to a new line every 76 characters. Pads incomplete output strings with "=".

**(Note: this base58 implementation is going to vary slightly from other versions due to the max unsigned integer limitations)**

<br/>

## COMPILE:

> ```bash
> gcc main.c base16encoder.c base16decoder.c base32encoder.c base32decoder.c base58encoder.c base64encoder.c base64decoder.c parsecl.c writedecoded.c -o baseNencode
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

- #### to ENCODE from stdandard input (base16, base32, base58, base64):

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

- #### to DECODE from stdandard input (base16, base32, base64):

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

- #### to compare outputs with builtin function:

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
  > cmp -l <(./baseNencode -n 32 testfile32_raw) <(base32 testfile32_raw)
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
