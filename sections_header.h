#include "elf.h"
#include "elf_header.h"
#include <stdio.h>


void load_sections_header(const FILE * const file,const Elf32_Ehdr * const elf_header,Elf32_Shdr * const sections_header);
void display_sections_header(const Elf32_Ehdr * const sections_header);