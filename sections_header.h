#include <elf.h>
#include "elf_header.h"
#include <stdio.h>
#include <stdlib.h>


void load_sections_header( FILE *  file, Elf32_Ehdr *  elf_header,Elf32_Shdr *  sections_header);
void display_sections_header( Elf32_Ehdr* elf_header, Elf32_Shdr *  sections_header);