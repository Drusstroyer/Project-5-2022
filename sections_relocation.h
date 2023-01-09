#ifndef S_RELOCATION
#define S_RELOCATION
#include <elf.h>
#include "elf_header.h"
#include "sections_header.h"
#include "sections_struct.h"
#include "sections_content.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// typedef struct t_SectionIndexName
// {
//     char* name;
//     int* Reltab_array;
// }SectionIndexName;

Elf32_Rela *ReadRelatab(FILE *f_elf, Elf32_Shdr *section_header, int Reltab_index);
Elf32_Rel *ReadReltab(FILE *f_elf, Elf32_Shdr *section_header, int Reltab_index);


// void ShowReloctab(Elf32_Rela *reloc_tab, Elf32_Shdr *section_header, int Reltab_index, SectionContent Content, SectionContent SymbolName);

void ShowRelatab(Elf32_Rela *reloc_tab, int nbRel, SectionContent SymbolName, Elf32_Sym * symbol_tab, Elf32_Shdr *section_header, SectionContent SectionName);
void ShowReltab(Elf32_Rel *reloc_tab, int nbRel, SectionContent SymbolName, Elf32_Sym * symbol_tab, Elf32_Shdr *section_header, SectionContent SectionName);

char *ENUM_TableRelocType(Elf32_Word type);
#endif