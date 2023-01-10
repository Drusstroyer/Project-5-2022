#ifndef S_FUSION
#define S_FUSION
#include <elf.h>
#include "sections_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sections_header.h"
#include "sections_content.h"

void FusionFile(FILE* file1, FILE* file2);
FusionContent* FusionSectionProgBits(Elf32_Shdr* SectionHeader1,SectionContent* Sections1,Elf32_Shdr* SectionHeader2 , SectionContent* Section2, int nbSection1, int nbSection2);
SectionContent* FusionSymtab(Elf32_Sym* Sym1,Elf32_Sym* Sym2,SectionContent Name1,SectionContent Name2,int nb1,int nb2);

#endif