FLI_PREFIX = /opt/fli/libfli-1.104
FLI_DEFS = -I$(FLI_PREFIX)/include
FLI_LIBS = -L$(FLI_PREFIX)/lib -lfli
# FLI_LIBS += -fPIC

CC = gcc
CFLAGS = -Wall -O2 -g
ODIR = .obj/build
SDIR = .
OBJ = acquisition.o
OUTPUTNAME = acquisition
OUTDIR = /opt/fli/example
MKDIR = mkdir -p


${OUTPUTNAME}: ${OBJ}
	${CC} ${CFLAGS} -o ${OUTPUTNAME} ${OBJ} ${FLI_LIBS}

${OBJ}: ${OUTPUTNAME}.c
	${CC} ${FLI_DEFS} -Wall -c ${OUTPUTNAME}.c -o ${OBJ}

bmp: bmp.o
	${CC} ${CFLAGS} bmp_test.c bmp.o -o bmp -lm

bmp.o: bmp.c
	${CC} -Wall -c bmp.c -o bmp.o

cleanbmp:
	rm bmp
	rm *.bmp
