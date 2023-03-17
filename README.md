# baseNencoder

## SUMMARY

_base64encode [OPTION] [FILE]_

&nbsp;&nbsp;&nbsp;&nbsp;Options:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-n encodes in given base number. Defaults to base64 encoding if -n flag not supplied.

<br/>

## DESCRIPTION:

C program that encodes binary data in base 32 or base 64 and prints to stdout.

Takes at most 1 file path (or reads from standard input if no file path or file argument is "-").

base32 encoding is completed by taking a block of five octets (40 bit string) and converting it to eight BASE32 characters.

base64 encoding is completed by taking a block of three octets (24 bit string) and converting it to four BASE64 characters.

Prints to standard output wrapping to a new line every 76 characters. Pads incomplete output strings with "=".

<br/>

## COMPILE:

> ```bash
> gcc main.c base32encoder.c base64encoder.c parsecl.c -o baseNencode
> ```

<br/>

## EXAMPLES:

A test file is supplied for testing generated with:

> ```bash
> head -c1000000 /dev/random > testfile
> ```

- #### to run sample test file in base64:

> ```bash
> ./baseNencode testfile
> ./baseNencode -n 64 testfile
> ```

- #### to encode from stdandard input:

> ```bash
> ./baseNencode
> ./baseNencode -n 32
> ./baseNencode -n 64
> ```

- #### to compare outputs with base64 builtin function:

> ```bash
> cmp -l <(./baseNencode -n 64 < testfile) <(base64 testfile)
> cmp -l <(./baseNencode < testfile) <(base64 testfile)
> cmp -l <(./baseNencode -n 32 < testfile) <(base32 testfile)
> ```

<br/>

## NOTES:

Planned expansion is to add base16 encoding functionality and decoding functionality for all three bases (16, 32, 64). Also plan to include base32Hex encoding and --wrap and --ignore-garbage flags.

<br/>

## AUTHOR:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Written by:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Zane Miller  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<millerzanem@gmail.com>  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<https://github.com/zanemmiller2/baseNencoder>
