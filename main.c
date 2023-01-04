#include <stdio.h>
#include <stdlib.h>
#include "elf_header.h"
#include "sections_content.h"
#include "sections_header.h"



int main(int argc, char* argv[])
{
    FILE* f_elf;
    if (argc > 1){

        if(argv[argc-1] != NULL){
            
            f_elf = fopen(argv[argc-1],"r");

        }
    }
    else {
        printf("Veuillez passer un fichier existant en argument du programme.\n");
        return 0;
    }
    if(argc == 2){
        printf("Usage: ./test <option> fichier ELF\n");
        printf(" Afficher les informations à propos du contenu du format des fichiers ELF\n Les options sont :\n");
        printf("    -h: Afficher l'en-tête du fichier elf\n");
        printf("    -S: Afficher l'en-tête des sections\n");
        printf("    -sym: Afficher la table des symboles\n");
    }
    if(f_elf == NULL){
        exit(1);
    }
    Elf32_Ehdr* header = malloc(sizeof(Elf32_Ehdr));

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

        int i=-1;
        int Symtab_index=-1;
        int SymStringName_index=-1;
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
                if(!strcmp(&SectionName->content[section_header[i].sh_name],".symtab"))            
                    Symtab_index=i;                
            }                        
        }while(i<header->e_shnum);
        SectionContent* Content = GetContent(f_elf,section_header[Symtab_index]);
        SectionName=GetContent(f_elf,section_header[SymStringName_index]);
        Elf32_Sym* tmp=ReadSymbtab(h_symtab,(int)header->e_shnum,Content);
        printf("\n");

        if(strcmp(argv[j],"-sym")==0){
            ShowSymtab(tmp,header->e_shnum,*SectionName);
        }

    }

        fclose(f_elf);
        free(header);
        return 0;
    }