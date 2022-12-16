#include "elf.h"
#include <stdio.h>


void load_elf_header(const FILE * const file, Elf32_Ehdr * const elf_header);
int check_elf_header(const Elf32_Ehdr * const elf_header);
void display_elf_header(const Elf32_Ehdr * const elf_header);