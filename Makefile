CC=gcc
CFLAGS=-Wall -Werror
ARM=arm-none-eabi-
READ=readelf
ELF=objcopy
PATHEX=elf_linker-1.0/Examples_fusion/

test: main.o elf_header.o sections_header.o sections_content.o sections_fusion.o
	$(CC) $(CFLAGS) -o $@ $^
%.o : %.c
	$(CC) $(CFLAGS) -c $^

goelf :
	arm-none-eabi-gcc -c $(ELF)
readelf1: $(PATHEX)file1.elf
	$(ARM)$(READ) -h $(PATHEX)file1.elf

readelf2: $(PATHEX)file2.elf
	$(ARM)$(READ) -h $(PATHEX)file2.elf

main.o: sections_header.h elf_header.h  sections_content.h
elf_header.o: elf_header.h sections_struct.h
sections_header.o: sections_header.h 
sections_content.o: sections_content.h sections_struct.h sections_header.h
sections_fusion.o: sections_fusion.h sections_struct.h

clean:
	make clear
clear:
	rm *.o
	rm *.elf
