#ifndef S_STRUCT
#define S_STRUCT
#include <elf.h>

typedef struct t_SectionContent
{
    char* name;
    char* content;
    int size;
}SectionContent;

typedef struct t_FusionContent
{
    Elf32_Shdr* headers;
    SectionContent* contents;
    int nb;
}FusionContent;

#endif