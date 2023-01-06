#include "sections_fusion.h"

void FusionFile(FILE* file1, FILE* file2)
{
    Elf32_Ehdr* header = malloc(sizeof(Elf32_Ehdr));
    Elf32_Shdr* section_header = malloc(sizeof(Elf32_Shdr)*header->e_shnum);
    Elf32_Ehdr* header2 = malloc(sizeof(Elf32_Ehdr));
    Elf32_Shdr* section_header2 = malloc(sizeof(Elf32_Shdr)*header2->e_shnum);
    load_elf_header(file1,header);    
    load_sections_header(file1,header,section_header);   
    load_elf_header(file2,header2);    
    load_sections_header(file2,header2,section_header2);   

    ReadProgbit(file1,section_header,header->e_shnum);
    ReadProgbit(file2,section_header2,header2->e_shnum);
}


SectionContent* ReadProgbit(FILE* file,Elf32_Shdr* section_header, int nb)
{
    SectionContent* relsections = malloc(sizeof(SectionContent)*nb);
    for(int i=0;i<nb;i++)
    {
        if(section_header[i].sh_type == SHT_REL || section_header[i].sh_type == SHT_RELA)
        {
            relsections[i]=*GetContent(file,section_header[i]);
        }
    }
    return relsections;
}