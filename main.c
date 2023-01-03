#include <stdio.h>
#include <stdlib.h>
#include "elf_header.h"
#include "sections_content.h"
#include "sections_header.h"



int main(int argc, char* argv[])
{
    FILE* f_elf = fopen(argv[1],"r");
    Elf32_Ehdr* header = malloc(sizeof(Elf32_Ehdr));
    load_elf_header(f_elf,header);    
    printf("size header:%d", header->e_shentsize);
    Elf32_Shdr* section_header = malloc(sizeof(Elf32_Shdr)*header->e_shnum);
    display_elf_header(header);
    load_sections_header(f_elf,header,section_header);   
    SectionContent* TableContent;
    if(header->e_shstrndx != SHN_UNDEF)
    {
        TableContent=GetContent(f_elf,section_header[header->e_shstrndx]);
    }
    else
    {
        TableContent=malloc(sizeof(SectionContent));
        TableContent->size=0;
    }
    
    display_sections_header(header,section_header,*TableContent);
    int i=-1;
    Elf32_Shdr h_symtab;
    do
    {
        i++;
        h_symtab = section_header[i];
    }while(section_header[i].sh_type != SHT_SYMTAB);

    Elf32_Sym* tmp=ReadSymbtab(h_symtab,(int)header->e_shnum,TableContent[i]);
    ShowSymtab(tmp,header->e_shnum);
    fclose(f_elf);
    free(header);
    return 0;
}