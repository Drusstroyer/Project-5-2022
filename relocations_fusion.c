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
    fusion_tab->tab = malloc(sizeof(RelocTab) * size); // header table creation
    int fusion = 0;
    int k = 0;
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            if ((header1[i].sh_type == SHT_REL || header1[i].sh_type == SHT_RELA) && !strcmp(Content1[i].name, Content2[j].name))
            {
                fusion = 1;

                (fusion_tab->tab)[k].header = malloc(sizeof(Elf32_Shdr));
                memcpy(&(fusion_tab->tab)[k].header, &header1[i], sizeof(Elf32_Shdr));
                (fusion_tab->tab)[k].header->sh_size += (header2[j]).sh_size;
                if ((fusion_tab->tab)[k].header->sh_type == SHT_REL)
                {
                    (fusion_tab->tab)[k].Rela = NULL;
                    (fusion_tab->tab)[k].Rel = malloc(sizeof(Elf32_Rel) * (((fusion_tab->tab)[k].header)->sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                    memcpy((fusion_tab->tab)[k].Rel, &Content1->content[header1[ELF32_R_SYM((fusion_tab->tab)[k].Rel->r_info)].sh_name], sizeof(((header1[i]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize)));
                    memcpy((fusion_tab->tab)[k].Rel + ((header1[i]).sh_size), &Content2->content[header1[ELF32_R_SYM((fusion_tab->tab)[k].Rel->r_info)].sh_name], sizeof((header2[j]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                }
                else if ((fusion_tab->tab)[k].header->sh_type == SHT_RELA)
                {
                    fusion_tab->tab->Rel = NULL;
                    (fusion_tab->tab)[k].Rela = malloc(sizeof(Elf32_Rela) * (((fusion_tab->tab)[k].header)->sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                    memcpy((fusion_tab->tab)[k].Rela, &Content1->content[header1[ELF32_R_SYM((fusion_tab->tab)[k].Rela->r_info)].sh_name], sizeof(((header1[i]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize)));
                    memcpy((fusion_tab->tab)[k].Rela + ((header1[i]).sh_size), &Content2->content[header1[ELF32_R_SYM((fusion_tab->tab)[k].Rela->r_info)].sh_name], sizeof((header2[j]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                }
                k++;
            }
        }
        if (!fusion)
        {
            (fusion_tab->tab)[k].header = malloc(sizeof(Elf32_Shdr));
            memcpy(&(fusion_tab->tab)[k].header, &header1[i], sizeof(Elf32_Shdr));
            if ((fusion_tab->tab)[k].header->sh_type == SHT_REL)
            {
                (fusion_tab->tab)[k].Rela = NULL;
                (fusion_tab->tab)[k].Rel = malloc(sizeof(Elf32_Rel) * (((fusion_tab->tab)[k].header)->sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                memcpy((fusion_tab->tab)[k].Rel, &Content1->content[header1[ELF32_R_SYM((fusion_tab->tab)[k].Rel->r_info)].sh_name], sizeof(((header1[i]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize)));
            }
            else if ((fusion_tab->tab)[k].header->sh_type == SHT_RELA)
            {
                fusion_tab->tab->Rel = NULL;
                (fusion_tab->tab)[k].Rela = malloc(sizeof(Elf32_Rela) * (((fusion_tab->tab)[k].header)->sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                memcpy((fusion_tab->tab)[k].Rela, &Content1->content[header1[ELF32_R_SYM((fusion_tab->tab)[k].Rela->r_info)].sh_name], sizeof(((header1[i]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize)));
            }
            k++;
        }
        fusion = 0;
    }
    for (int i = 0; i < size2; i++)
    {
        for (int j = 0; j < size1; j++)
        {
            if ((header2[i].sh_type == SHT_REL || header2[i].sh_type == SHT_RELA) && !strcmp(Content1[i].name, Content2[j].name))
            {
                fusion = 1;
            }
        }
        if (!fusion)
        {
            (fusion_tab->tab)[k].header = malloc(sizeof(Elf32_Shdr));
            memcpy(&(fusion_tab->tab)[k].header, &header2[i], sizeof(Elf32_Shdr));
            if ((fusion_tab->tab)[k].header->sh_type == SHT_REL)
            {
                (fusion_tab->tab)[k].Rela = NULL;
                (fusion_tab->tab)[k].Rel = malloc(sizeof(Elf32_Rel) * (((fusion_tab->tab)[k].header)->sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                memcpy((fusion_tab->tab)[k].Rel, &Content2->content[header2[ELF32_R_SYM((fusion_tab->tab)[k].Rel->r_info)].sh_name], sizeof(((header2[i]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize)));
            }
            else if ((fusion_tab->tab)[k].header->sh_type == SHT_RELA)
            {
                fusion_tab->tab->Rel = NULL;
                (fusion_tab->tab)[k].Rela = malloc(sizeof(Elf32_Rela) * (((fusion_tab->tab)[k].header)->sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize));
                memcpy((fusion_tab->tab)[k].Rela, &Content2->content[header2[ELF32_R_SYM((fusion_tab->tab)[k].Rela->r_info)].sh_name], sizeof(((header2[i]).sh_size) / (((fusion_tab->tab)[k].header)->sh_entsize)));
            }
            k++;
        }
        fusion = 0;
    }

    return fusion_tab;
}
