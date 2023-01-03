CC=gcc
CFLAGS=-Wall -Werror
ARM=arm-none-eabi-
READ=readelf
ELF=objcopy
PATHEX=elf_linker-1.0/Examples_fusion/

test: main.o elf_header.o sections_header.o sections_content.o
	$(CC) $(CFLAGS) -o $@ $^
%.o : %.c
	$(CC) $(CFLAGS) -c $^
$(PATHEX)file1.o:
	$(ARM)$(CC) $(CFLAGS) -c $(PATHEX)file1.c $(PATHEX)file1.o
$(PATHEX)file2.o:
	$(ARM)$(CC) $(CFLAGS) -c $(PATHEX)file2.c $(PATHEX)file2.o

$(PATHEX)file1.elf:$(PATHEX)file1.o
	$(ARM)$(ELF) $(CFLAGS) -I binary -O elf32-little $(PATHEX)file1.o $(PATHEX)file1.elf

$(PATHEX)file2.elf:$(PATHEX)file2.o
	$(ARM)$(ELF) $(CFLAGS) -I binary -O elf32-little $(PATHEX)file2.o $(PATHEX)file2.elf

goelf :
	arm-none-eabi-objcopy -I binary -O elf32-little $(ELF).o $(ELF).elf
readelf1: $(PATHEX)file1.elf
	$(ARM)$(READ) -h $(PATHEX)file1.elf

readelf2: $(PATHEX)file2.elf
	$(ARM)$(READ) -h $(PATHEX)file2.elf

main.o: sections_header.h elf_header.h  sections_content.h
elf_header.o: elf_header.h sections_struct.h
sections_header.o: sections_header.h 
sections_content.o: sections_content.h sections_struct.h sections_header.h

clean:
	make clear
clear:
	rm *.o
	rm *.elf
