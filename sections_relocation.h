#ifndef S_RELOCATION
#define S_RELOCATION
#include <elf.h>
#include "elf_header.h"
#include "sections_header.h"
#include "sections_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Elf32_Rela* ReadReloctab(Elf32_Shdr sections_header,int nbRel, SectionContent* Content);
// void ShowReloctab(Elf32_Rela * symbol_tab,int nbRel, SectionContent RelName);

Elf32_Rela* ReadRelatab(Elf32_Shdr sections_header,int nbRel, SectionContent* Content);
Elf32_Rel* ReadReltab(Elf32_Shdr sections_header, int nbRel, SectionContent* Content);

void ShowRelatab(Elf32_Rela * reloc_tab, int nbRel, SectionContent RelName);
void ShowReltab(Elf32_Rel * reloc_tab, int nbRel, SectionContent RelName);

#endif