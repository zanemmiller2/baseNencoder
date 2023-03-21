# baseNencoder

## SUMMARY

_base64encode [OPTION] [FILE]_

&nbsp;&nbsp;&nbsp;&nbsp;Options:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-n [BASE] encodes in given base number. Defaults to base64 encoding if -n flag not supplied.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-d [BASE] decodes in given base number. Defaults to base64 decoding if -d flag not supplied. or -d flag supplied with no [BASE]

<br/>

## DESCRIPTION:

C program that encodes and decodes data in base 16, 32 or 64 and prints to stdout.

Takes at most 1 file path (or reads from standard input if no file path or file argument is "-").

<ins>base16 encoding</ins> is completed by taking a block of one octet (8 bit string) and converting it to two BASE16 characters.

<ins>base16 decoding</ins> is completed by taking a block of two octets (16 bit string) of base16 characters and converting it to one ASCII characters.

<ins>base32 encoding</ins> is completed by taking a block of five octets (40 bit string) and converting it to eight BASE32 characters.

<ins>base64 encoding</ins> is completed by taking a block of three octets (24 bit string) and converting it to four BASE64 characters.

<ins>base64 decoding</ins> is completed by taking a block of four base64 characters and converting it to three ASCII characters.

Prints to standard output wrapping to a new line every 76 characters. Pads incomplete output strings with "=".

<br/>

## COMPILE:

> ```bash
> gcc main.c base16encoder.c base32encoder.c base64encoder.c parsecl.c -o baseNencode
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
  > ./baseNencode -n 64 testfile
  > ```

- #### to ENCODE from stdandard input:

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
  > ./baseNencode -n 64
  > ```

- #### to DECODE from stdandard input:

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
  > ./baseNencode -d 64
  > ```

- #### to compare outputs with builtin function:

  > defaults to base 64 with no flags:
  >
  > ```bash
  > cmp -l <(./baseNencode testfile64_raw) <(base64 testfile64_raw)
  > ```

  > ```bash
  > cmp -l <(./baseNencode -n 64 testfile64_raw) <(base64 testfile64_raw)
  > ```

  > ```bash
  > cmp -l <(/.baseNencode -d 64 encodedtest64) <(base64 -d encodedtest64)
  > ```

  > ```bash
  > cmp -l <(/.baseNencode -d encodedbase64) <(base64 -d encodedbase64)
  > ```

  > ```bash
  > cmp -l <(./baseNencode -n 32 testfile32_raw) <(base32 testfile32_raw)
  > ```

  > ```bash
  > cmp -l <(./baseNencode -d 32 encodedbase32) <(base32 encodedbase32)
  > ```

  > ```bash
  > cmp -l <(./baseNencode -n 16 testfile16_raw) <(cat testfile16_raw | xxd -p -u)
  > ```

  > ```bash
  > cmp -l <(./baseNencode -d 16 encodedbase16) <(cat testfile16_raw)
  > ```

<br/>

## NOTES:

Planned expansion is to add decoding functionality for reamining base32. Also plan to include base32Hex encoding, encoding/decoding in other base systems and --wrap and --ignore-garbage flags.

<br/>

## AUTHOR:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Written by:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Zane Miller  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<millerzanem@gmail.com>  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<https://github.com/zanemmiller2/baseNencoder>
