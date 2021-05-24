# Compilation flags
GCC = gcc
GCCFLAGS = -std=c99 -D_DEFAULT_SOURCE -Wall -g -pedantic
LIBRARIES = -lm

# Objects are not the same for each exe
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

LIBRARY = ./include
OUTPUT = ss

all: $(OBJECTS)
	$(GCC) $(GCCFLAGS) -I$(LIBRARY) $(OBJECTS) -o $(OUTPUT) $(LIBRARIES)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I$(LIBRARY) -c $< -o $@

clean:
	rm -rf $(S_OBJECTS) $(OBJECTS) $(OUTPUT)

.PHONY: all clean