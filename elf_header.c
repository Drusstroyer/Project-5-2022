 #include "elf_header.h"
 
void load_elf_header(const FILE * const file,Elf32_Ehdr * const elf_header)
 {
    fseek(file,0,SEEK_SET);
    fread(elf_header,1,sizeof(elf_header),file);
 }

int check_elf_header(const Elf32_Ehdr * const elf_header)
 {
    if(0 /*TESTING NON VALIDITY*/) {
        /*ERROR*/
        return 0;
    }
    /*OTHER TESTS*/

    return 1;
    
 }

 void display_elf_header(const Elf32_Ehdr * const elf_header)
 {
    printf("e_ident: %s",       elf_header->e_ident);
    printf("e_type: %d",        elf_header->e_type);
    printf("e_machine: %d",     elf_header->e_machine);
    printf("e_version: %d",     elf_header->e_version);
    printf("e_entry: %p",       elf_header->e_entry);
    printf("e_phoff: %p",       elf_header->e_phoff);
    printf("e_shoff: %p",       elf_header->e_shoff);
    printf("e_flags: %d",       elf_header->e_flags);
    printf("e_ehsize: %d",      elf_header->e_ehsize);
    printf("e_phentsize: %d",   elf_header->e_phentsize);
    printf("e_phnum: %d",       elf_header->e_phnum);
    printf("e_shentsize: %d",   elf_header->e_shentsize);
    printf("e_shnum: %d",       elf_header->e_shnum);
    printf("e_shstrndx: %d",    elf_header->e_shstrndx);
 }