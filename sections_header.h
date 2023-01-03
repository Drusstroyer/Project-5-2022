#ifndef S_HEADER 
#define S_HEADER
#include <elf.h>
#include "elf_header.h"
#include "sections_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void load_sections_header( FILE *  file, Elf32_Ehdr *  elf_header,Elf32_Shdr *  sections_header);
void display_sections_header( Elf32_Ehdr* elf_header, Elf32_Shdr *  sections_header,SectionContent TableName);

char * ENUM_SectionHeaderType(Elf32_Word type);
char * ENUM_Flags(Elf32_Word flags);

SectionContent* GetContent(FILE* file,Elf32_Shdr section_header);

char * ExtractName(char* FullName,int index);

#endif

