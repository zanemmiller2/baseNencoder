# c-base64encoder

base64encode [file_name]

C program that encodes binary data in base 64 and prints to stdout
Takes at most 1 file path (or reads from standard input if no file path or file argument is "-")
Converts 24 bits (3 bytes) of input data and encodes it in base64 (4 bytes) 
Prints to standard output wrapping to a new line every 76 characters. Pads incomplete bytes with "=".
A test file (generated with 'head -c1000000 /dev/random > testfile') is supplied for testing.

run instructions:
# to run sample test file:
./base64encode testfile
# to encode from stdandard input:
./base64encode
# to compare outputs with base64 builtin function:
cmp -l <(./base64encode < testfile) <(base64 testfile)
