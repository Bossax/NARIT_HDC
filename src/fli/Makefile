FLI_PREFIX = /opt/fli/libfli-1.104
FLI_DEFS = -I$(FLI_PREFIX)/include
FLI_LIBS = -L$(FLI_PREFIX)/lib -lfli -lm
# FLI_LIBS += -fPIC

CC = gcc
CFLAGS = -Wall -O2 -g
ODIR = ./build
OBJ = acquisition.o

SOURCE_DIR = ./
SOURCE_NAME = acquisition.c
MKDIR = mkdir -p

OUTPUT_NAME = acquisition
OUTPUT_DIR = ./build/

BMP_DIR = -I./bmp/build

.PHONY: test_bmp clean

$(OUTPUT_NAME): $(OBJ) bmp.o
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)$(OUTPUT_NAME) $(ODIR)/$(OBJ) $(ODIR)/bmp.o $(FLI_LIBS)

$(OBJ): $(SOURCE_NAME)
	$(CC) $(FLI_DEFS) $(BMP_DIR) -Wall -c $(SOURCE_NAME) -o ./build/$(OBJ)

test_bmp: bmp
	echo "generate bmp images"
	./build/bmp

bmp: bmp.o
		${CC} ${CFLAGS} bmp_test.c -o $(OUTPUT_DIR)bmp $(ODIR)/bmp.o -lm

bmp.o: bmp.c |$(ODIR)
	$(CC) -Wall -c bmp.c -o $(ODIR)/bmp.o

$(ODIR):
		$(MKDIR) $(ODIR)

clean:
	rm -f $(OUTPUT_DIR)/*
	rm -f *.bmp
