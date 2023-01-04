#include "sections_relocation.h"

 
Elf32_Rela* ReadRelatab(Elf32_Shdr sections_header,int nbRel, SectionContent* Content)
{ 
    Elf32_Rela * reloc_tab = malloc(sizeof(Elf32_Rel)*nbRel);
    if(!reloc_tab){
        perror("Error: ");
        exit(1);
    }
    reloc_tab = (Elf32_Rela*)Content->content;
    return reloc_tab;
}

Elf32_Rel* ReadReltab(Elf32_Shdr sections_header, int nbRel, SectionContent* Content)
{ 
    Elf32_Rel * reloc_tab = malloc(sizeof(Elf32_Rel)*nbRel);
    if(!reloc_tab){
        perror("Error: ");
        exit(1);
    }
    reloc_tab = (Elf32_Rel*)Content->content;
    return reloc_tab;
}

void ShowRelatab(Elf32_Rela * reloc_tab, int nbRel, SectionContent RelName)
{    
    printf("Relocation section \'.rela.text\' at offset 0x%x contains %i entries:\n", reloc_tab->r_offset, nbRel);
    printf("  Offset          Info           Type           Sym. Value    Sym. Name + Addend\n");
    for(int i=0; i<nbRel; i++)     //check boucle condition
    {
        printf("%08x", reloc_tab[i].r_offset); //offset
        printf(" %08x", reloc_tab[i].r_info); //info
        printf(" %c", ELF32_R_TYPE(reloc_tab[i].r_info)); //type : SECTION, NOTYPE, ...
        printf(" %08x", ELF32_R_SYM(reloc_tab[i].r_info));
        printf(" %c + %d", ELF32_R_INFO(ELF32_R_SYM(reloc_tab[i].r_info),ELF32_R_TYPE(reloc_tab[i].r_info)),reloc_tab[i].r_addend);
        printf(" \n");
    }   
}

void ShowReltab(Elf32_Rel * reloc_tab, int nbRel, SectionContent RelName)
{    
    printf("Relocation section \'.rel.text\' at offset 0x%x contains %i entries:\n", reloc_tab->r_offset, nbRel);
    printf("  Offset          Info           Type           Sym. Value    Sym. Name \n");
    for(int i=0; i< nbRel; i++)     //check boucle condition
    {
        printf("%12X", reloc_tab[i].r_offset); //offset
        printf("  %12X", reloc_tab[i].r_info); //info
        printf(" %-7c", ELF32_R_TYPE(reloc_tab[i].r_info)); //type : SECTION, NOTYPE, ...
        printf(" %16x", ELF32_R_SYM(reloc_tab[i].r_info));
        printf(" %-6c", ELF32_R_INFO(ELF32_R_SYM(reloc_tab[i].r_info),ELF32_R_TYPE(reloc_tab[i].r_info)));
        printf(" \n");
    }   
}

/*Other option, less code using casts*/