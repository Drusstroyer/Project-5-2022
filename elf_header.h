#ifndef ELF_HEADER
#define ELF_HEADER
#include <elf.h>
#include "elf_linker-1.0/util.h"
#include <stdio.h>
#include "sections_struct.h"


void load_elf_header(FILE* file, Elf32_Ehdr * const elf_header);
int check_elf_header(const Elf32_Ehdr * const elf_header);
void display_elf_header(const Elf32_Ehdr * const elf_header);
#endif