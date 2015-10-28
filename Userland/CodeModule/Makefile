include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin 
SOURCES=$(wildcard *.c) 
OBJECTS=$(SOURCES:.c=.o) 
ASMSOURCES=$(wildcard asm/*.asm) 
ASMOBJECTS=$(ASMSOURCES:.asm=.o)
LIBSOURCES=$(wildcard libs/*.c)
LIBOBJECTS=$(LIBSOURCES:.c=.o)

STATICLIBS=

all: $(MODULE)

$(MODULE): $(OBJECTS) $(ASMOBJECTS) $(LIBOBJECTS) $(STATICLIBS) 
	$(LD) $(LDFLAGS) -T shell.ld -o ../$(MODULE) $(OBJECTS) $(ASMOBJECTS) $(LIBOBJECTS) $(STATICLIBS)

%.o: %.c
	 $(GCC) $(GCCFLAGS) -I./include -c $< -o $@ 

%.o: %.asm 
	$(ASM) $(ASMFLAGS) $< -o $@

clean: 
	rm -rf asm/*.o libs/*.o *.o

.PHONY: all clean
