#include <stdio.h>
#include <stdlib.h>
#include "elf_header.h"
#include "sections_content.h"
#include "sections_header.h"
#include "sections_relocation.h"

int main(int argc, char* argv[])
{
    FILE* f_elf = fopen(argv[1],"r");
    Elf32_Ehdr* header = malloc(sizeof(Elf32_Ehdr));
    load_elf_header(f_elf,header);    
    printf("size header:%d", header->e_shentsize);
    Elf32_Shdr* section_header = malloc(sizeof(Elf32_Shdr)*header->e_shnum);
    display_elf_header(header);
    load_sections_header(f_elf,header,section_header);   
    SectionContent* SectionName;
    if(header->e_shstrndx != SHN_UNDEF)
    {
        SectionName=GetContent(f_elf,section_header[header->e_shstrndx]);// get the string table of sections name
    }
    else
    {
        SectionName=malloc(sizeof(SectionContent));
        SectionName->size=0;
    }
    
    display_sections_header(header,section_header,*SectionName);
    int i=-1;
    int Symtab_index=-1;
    int SymStringName_index=-1;
    int Reltab_index = -1;
    Elf32_Shdr h_symtab;    
    do
    {
        i++;         
        if(!strcmp(&SectionName->content[section_header[i].sh_name],".strtab"))
        {         
            SymStringName_index = i;
        }
        else
        {
            if(!strcmp(&SectionName->content[section_header[i].sh_name],".symtab")){
                h_symtab = section_header[i];
                Symtab_index = i;   
            }            
            else if(!strcmp(&SectionName->content[section_header[i].sh_name],".rel.text")){
                Reltab_index = i;
            }  
        }                        
    }while(i<header->e_shnum);

    SectionContent* Content = GetContent(f_elf,section_header[Symtab_index]);
    SectionContent* SymbolName=GetContent(f_elf,section_header[SymStringName_index]);
    Elf32_Sym* tmp=ReadSymbtab(h_symtab,(section_header[Symtab_index].sh_size)/(section_header[Symtab_index].sh_entsize),Content);
    printf("\n");
    ShowSymtab(tmp,(section_header[Symtab_index].sh_size)/section_header[Symtab_index].sh_entsize,*SymbolName);

    //affichage .rel/.rela
    Elf32_Rela * reloc_tab = ReadReloctab(f_elf, section_header, h_symtab, Reltab_index);
    ShowReloctab(reloc_tab, section_header, Reltab_index, *Content, *SymbolName);
    // SectionContent* Content2 = GetContent(f_elf,section_header[Reltab_index]);
    // SectionName=GetContent(f_elf,section_header[SymStringName_index]);
    // if(section_header[Reltab_index].sh_type == SHT_RELA){
    //     Elf32_Rela* tmp2=ReadRelatab(h_symtab, (section_header[Reltab_index].sh_size)/section_header[Reltab_index].sh_entsize, Content2);
    //     printf("\n");
    //     ShowRelatab(tmp2, (section_header[Reltab_index].sh_size)/section_header[Reltab_index].sh_entsize,*SectionName);
    // }
    // else if(section_header[Reltab_index].sh_type == SHT_REL){
    //     Elf32_Rel* tmp3=ReadReltab(h_symtab, (section_header[Reltab_index].sh_size)/section_header[Reltab_index].sh_entsize, Content2);
    //     printf("\n");
    //     ShowReltab(tmp3, (section_header[Reltab_index].sh_size)/section_header[Reltab_index].sh_entsize,*SectionName);
    // }
    // // Elf32_Rela* tmp2=ReadReloctab(h_symtab, (section_header[Reltab_index].sh_size)/section_header[Reltab_index].sh_entsize, Content2);
    // printf("\n");
    // ShowReloctab(tmp2, (section_header[Reltab_index].sh_size)/section_header[Reltab_index].sh_entsize,*SectionName);
    fclose(f_elf);
    free(header);
    return 0;
}