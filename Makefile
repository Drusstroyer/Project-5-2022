CC=gcc
CFLAGS=-Wall -Werror
ARM=arm-none-eabi-
READ=readelf
ELF=objcopy
PATHEX=elf_linker-1.0/Examples_fusion/

$(PATHEX)file1.o:
	$(ARM)$(CC) $(CFLAGS) -c $(PATHEX)file1.c $(PATHEX)file1.o
$(PATHEX)file2.o:
	$(ARM)$(CC) $(CFLAGS) -c $(PATHEX)file2.c $(PATHEX)file2.o

$(PATHEX)file1.elf:$(PATHEX)file1.o
	$(ARM)$(ELF) $(CFLAGS) -I binary -O elf32-little $(PATHEX)file1.o $(PATHEX)file1.elf

$(PATHEX)file2.elf:$(PATHEX)file2.o
	$(ARM)$(ELF) $(CFLAGS) -I binary -O elf32-little $(PATHEX)file2.o $(PATHEX)file2.elf



elf_header.o: elf_header.h
section_header.o: section_header.h


readelf1: $(PATHEX)file1.elf
	$(ARM)$(READ) -h $(PATHEX)file1.elf

readelf2: $(PATHEX)file2.elf
	$(ARM)$(READ) -h $(PATHEX)file2.elf

clear:
	rm ./*.o
	rm ./*.elf
