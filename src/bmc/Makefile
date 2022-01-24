BMC_PREFIX = /opt/Boston_Micromachines
BMC_DEFS = -I$(BMC_PREFIX)/include
BMC_LIBS = -L$(BMC_PREFIX)/lib -lBMC

CC = gcc
CFLAGS = -Wall -O2 -g
ODIR = ./build
SOURCE_NAME = actuator_poke.c
SOURCE_DIR = ./
OBJ = actuator_poke.o
OUTPUTNAME = actuator_poke

MKDIR = mkdir -p

OUTPUT_DIR = ./build/

.PHONY: clean

$(OUTPUTNAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)$(OUTPUTNAME) $(ODIR)/$(OBJ) $(BMC_LIBS)

$(OBJ): $(SOURCE_NAME) | $(ODIR)
	$(CC) $(BMC_DEFS) -Wall -c $(SOURCE_DIR)$(SOURCE_NAME) -o ./build/$(OBJ)

$(ODIR):
	$(MKDIR) $(ODIR)

clean:
	rm -f $(ODIR)/*
