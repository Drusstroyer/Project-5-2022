#ifndef S_FUSION
#define S_FUSION
#include <elf.h>
#include "sections_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include "sections_header.h"
#include "sections_content.h"

void FusionFile(FILE* file1, FILE* file2);
SectionContent* ReadProgbit(FILE* file,Elf32_Shdr* section_header, int nb);

#endif