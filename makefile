CC = gcc
CDFLAGS = -Wall
LDFLAGS = 
OBJFILES = decoders/base16decoder.o encoders/base16encoder.o decoders/base32decoder.o encoders/base32encoder.o decoders/base58decoder.o encoders/base58encoder.o decoders/base64decoder.o encoders/base64encoder.o decoders/basez85decoder.o encoders/basez85encoder.o utilities/bigIntmath.o utilities/parsecl.o utilities/writedecoded.o main.o
TARGET = baseNencode

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

test:
	./testfiles/testscript.sh