#ifndef S_FUSION
#define S_FUSION
#include <elf.h>
#include "sections_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sections_header.h"
#include "sections_content.h"
#include "sections_fusion.h"

FusionReloc *create_reloc_tab(Elf32_Shdr *header1, Elf32_Shdr *header2, int size1, int size2, SectionContent *Content1, SectionContent *Content2);

#endif