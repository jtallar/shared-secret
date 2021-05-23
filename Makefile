# Compilation flags
GCC = gcc
GCCFLAGS = -std=c99 -Wall -g -pedantic
LIBRARIES = -lm

# Objects are not the same for each exe
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

LIBRARY = ./include
OUTPUT = a.out

all: $(OBJECTS)
	$(GCC) $(GCCFLAGS) -I$(LIBRARY) $(OBJECTS) -o $(OUTPUT) $(LIBRARIES)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I$(LIBRARY) -c $< -o $@

clean:
	rm -rf $(S_OBJECTS) $(OBJECTS)

.PHONY: all clean