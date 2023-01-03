#ifndef S_CONTENT
#define S_CONTENT
#include <elf.h>
#include "elf_header.h"
#include "sections_header.h"
#include "sections_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


Elf32_Sym* ReadSymbtab(Elf32_Shdr sections_header,int nbSym,SectionContent Content);
void ShowSymtab(Elf32_Sym * symbol_tab, int nbSym);

char * ENUM_TableSymbolType(Elf32_Word type);
char * ENUM_TableSymbolBinding(Elf32_Word type);

#endif