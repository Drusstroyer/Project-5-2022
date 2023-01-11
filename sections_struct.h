#ifndef S_STRUCT
#define S_STRUCT
#include <elf.h>

typedef struct t_SectionContent
{
    char *name;
    char *content;
    int size;
} SectionContent;

typedef struct t_FusionContent
{
    Elf32_Shdr *headers;
    SectionContent *contents;
    int nb;
} FusionContent;

typedef struct t_RelocTab
{
    Elf32_Shdr *header;
    Elf32_Rel *Rel;
    Elf32_Rela *Rela;
    int size;
} RelocTab;

typedef struct t_FusionReloc
{
    RelocTab *tab;
    int size;
} FusionReloc;

#endif