include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
DRIVERSOURCES=$(wildcard drivers/*.c)
DRIVEROBJECTS=$(DRIVERSOURCES:.c=.o)
INTERRUPTSOURCES=$(wildcard interrupt/*.c)
INTERRUPTOBJECTS=$(INTERRUPTSOURCES:.c=.o)
MEMORYSOURCES = $(wildcard memory/*.c)
MEMORYOBJECTS = $(MEMORYSOURCES:.c=.o)
SCHEDULERSOURCES = $(wildcard scheduler/*.c)
SCHEDULEROBJECTS = $(SCHEDULERSOURCES:.c=.o)

VIRTUALMEMORYSOURCES = $(wildcard VirtualMemory/*.c)
VIRTUALMEMORYOBJECTS = $(VIRTUALMEMORYSOURCES:.c=.o)

IPCSOURCES = $(wildcard ipc/*.c)
IPCOBJECTS = $(IPCSOURCES:.c=.o)

OBJECTS2 = $(DRIVEROBJECTS) $(INTERRUPTOBJECTS) $(MEMORYOBJECTS) $(SCHEDULEROBJECTS) $(IPCOBJECTS) $(VIRTUALMEMORYOBJECTS)



LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS2) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS2) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
		$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf memory/*.o asm/*.o drivers/*.o interrupt/*.o scheduler/*.o VirtualMemory/*.o *.o *.bin 

.PHONY: all clean
