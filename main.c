#include <stdio.h>
#include <stdlib.h>
#include "elf_header.h"
#include "sections_content.h"
#include "sections_header.h"
#include "sections_fusion.h"



int main(int argc, char* argv[])
{
    FILE* f_elf;
    FILE* f_elf2;

    switch(argc)
    {
        case 1 :
            printf("Veuillez passer un fichier existant en argument du programme.\n");
            exit(1);

        case 2:
            printf("Usage: ./test <option> fichier ELF\n");
            printf(" Afficher les informations à propos du contenu du format des fichiers ELF\n Les options sont :\n");
            printf("    -h: Afficher l'en-tête du fichier elf\n");
            printf("    -S: Afficher l'en-tête des sections\n");
            printf("    -X: Afficher le contenu des sections en hexadecimal\n");
            printf("    -F: Fusionne\n");
            printf("    -sym: Afficher la table des symboles\n");
            exit(1);
        case 3:
            f_elf=fopen(argv[argc-1],"r");
            break;
        case 4:
            f_elf=fopen(argv[argc-2],"r");
            f_elf2=fopen(argv[argc-1],"r");
            if(f_elf2 == NULL)
                exit(1);
            break;
    }
    if(f_elf == NULL){
        exit(1);
    }
    Elf32_Ehdr* header = malloc(sizeof(Elf32_Ehdr));
    

    if(argc == 4)
    {
        //only fusion here
        FusionFile(f_elf, f_elf2);
        fclose(f_elf);
        fclose(f_elf2);
        exit(1);
    }    
    load_elf_header(f_elf,header);    
    printf("size header:%d\n", header->e_shentsize);
    for(int j =1; j < argc-1; j++){
        if(strcmp(argv[j],"-h")==0){
            display_elf_header(header);
        }

        Elf32_Shdr* section_header = malloc(sizeof(Elf32_Shdr)*header->e_shnum);
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
        
        if(strcmp(argv[j],"-S")==0){
            display_sections_header(header,section_header,*SectionName);
        }        
        int Symtab_index=GetHeader(*SectionName,section_header,".symtab",header->e_shnum);    
        Elf32_Shdr h_symtab;            
        h_symtab = section_header[Symtab_index];        
        SectionContent* Symbols = GetContent(f_elf,section_header[Symtab_index]);
        SectionContent* SymbolsName=GetContent(f_elf,section_header[GetHeader(*SectionName,section_header,".strtab",header->e_shnum)]);
        Elf32_Sym* tmp=ReadSymbtab(h_symtab,Symbols);
        printf("\n");

        if(strcmp(argv[j],"-sym")==0){
            ShowSymtab(tmp,section_header[Symtab_index].sh_size/section_header[Symtab_index].sh_entsize,*SymbolsName);
        }
        
        SectionContent* section_cont = ReadAllSections(f_elf,section_header,header->e_shnum,*SectionName);
        if(strcmp(argv[j],"-X")==0){
            disp_section_content(section_cont,header->e_shnum);
        }

    }

        fclose(f_elf);
        free(header);
        return 0;
    }