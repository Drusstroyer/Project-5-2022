 #include "elf_header.h"
 
void load_sections_header(const FILE * file,const  Elf32_Ehdr* elf_header,Elf32_Shdr* sections_header)
 {
    fseek(file,0 /*get value from header*/,SEEK_SET);
    fread(sections_header,1,sizeof(sections_header),file);
 }

void display_sections_header(const Elf32_Ehdr* sections_header)
 {

 }