#ifndef S_CONTENT
#define S_CONTENT
#include <elf.h>
#include "elf_header.h"
#include "sections_header.h"
#include "sections_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


Elf32_Sym* ReadSymbtab(Elf32_Shdr sections_header,SectionContent* Content);
void ShowSymtab(Elf32_Sym * symbol_tab,int size,SectionContent SymbolName);
SectionContent* ReadAllSections(FILE* file,Elf32_Shdr* section_header, int nb,SectionContent SectionName );
void disp_section_content(SectionContent * section,int size);
char * ENUM_TableSymbolType(Elf32_Word type);
char * ENUM_TableSymbolBinding(Elf32_Word type);
char * ENUM_TableSymbolNdx(Elf32_Half type);
char * ENUM_TableSymbolVis(unsigned char type);

#endif