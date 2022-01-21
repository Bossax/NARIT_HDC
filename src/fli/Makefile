FLI_PREFIX = /opt/fli/libfli-1.104
FLI_DEFS = -I$(FLI_PREFIX)/include
FLI_LIBS = -L$(FLI_PREFIX)/lib -lfli -lm
# FLI_LIBS += -fPIC

CC = gcc
CFLAGS = -Wall -O2 -g
ODIR = ./build/
SDIR = .
OBJ = acquisition.o
OUTPUTNAME = acquisition
OUTDIR = /opt/fli/example
MKDIR = mkdir -p

OUTPUT_DIR = ./build/
BMP_DIR = -I./bmp/build

${OUTPUTNAME}: ${OBJ} bmp.o
	${CC} ${CFLAGS} -o ${OUTPUT_DIR}${OUTPUTNAME} ${ODIR}${OBJ} bmp/bmp.o ${FLI_LIBS} 

${OBJ}: ${OUTPUTNAME}.c
	${CC} ${FLI_DEFS} ${BMP_DIR} -Wall -c ${OUTPUTNAME}.c -o ./build/${OBJ}

bmp.o:
	${CC} -Wall -c ./bmp/bmp.c -o ./bmp/bmp.o
