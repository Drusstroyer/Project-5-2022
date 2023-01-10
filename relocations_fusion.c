#include "relocations_fusion.h"

FusionReloc *create_fusion_tab(Elf32_Shdr *header1, Elf32_Shdr *header2, int size1, int size2, SectionContent *Content1, SectionContent *Content2)
{
    FusionReloc *fusion_tab = malloc(sizeof(FusionReloc));
    int size = 0;
    int number = 0;
    for (int c = 0; c < size2; c++)
    {
        if ((header2[c].sh_type == SHT_REL || header2[c].sh_type == SHT_RELA))
        {
            number++;
        }
    }
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            if ((header1[j].sh_type == SHT_REL || header1[j].sh_type == SHT_RELA) && !strcmp(Content1[i].name, Content2[j].name))
            {
                number--;
            }
        }
        size++;
    }
    size += number;                                      // sizeof Elf32_Shdr tab counting as one occurences that have de same name trcmp(Content1[i].name, Content2[j].name) + everything else
    fusion_tab->tab = malloc(sizeof(Elf32_Shdr) * size); // header table creation
    int fusion = 0;
    int k = 0;
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            if ((header1[j].sh_type == SHT_REL || header1[j].sh_type == SHT_RELA) && !strcmp(Content1[i].name, Content2[j].name))
            {
                fusion = 1;

                (fusion_tab->tab)[k].header = malloc(sizeof(Elf32_Shdr));
                memcpy(&(fusion_tab->tab)[k].header, &header1[i], sizeof(Elf32_Shdr));
                (fusion_tab->tab)[k].header->sh_entsize += (header2[j]).sh_entsize;
                k++;
            }
        }
        if (!fusion)
        {
            (fusion_tab->tab)[k].header = malloc(sizeof(Elf32_Shdr));
            memcpy(&(fusion_tab->tab)[k].header, &header1[i], sizeof(Elf32_Shdr));
            k++;
        }
        fusion = 0;
    }
    return fusion_tab;
}
