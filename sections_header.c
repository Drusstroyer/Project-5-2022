 #include "sections_header.h"

void load_sections_header( FILE * file,  Elf32_Ehdr* elf_header,Elf32_Shdr* sections_header)
 {
   if(elf_header->e_shoff != 0)
   {
      fseek(file,elf_header->e_shoff,SEEK_SET);
      fread(sections_header,sizeof(Elf32_Shdr),elf_header->e_shnum,file);
   }
   else
   {
      fprintf(stderr,"Err: SectionHeader 0\n");
   }
    
 }

void display_sections_header( Elf32_Ehdr* elf_header,Elf32_Shdr* sections_header)
 {
      printf("[Nr] Nom        Type        Adr      Décala.Taille ES Fan LN Inf Al\n");
      for(int i=0;i<elf_header->e_shnum;i++)
      {         
         printf("[%d] ",i);
         printf(" %i",sections_header[i].sh_name);
         printf(" %i",sections_header[i].sh_type);
         printf("\n");
      }
 }