 #include "sections_header.h"

void load_sections_header( FILE * file,  Elf32_Ehdr* elf_header,Elf32_Shdr* sections_header)
 {
   //load the section header from file
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

void display_sections_header(Elf32_Ehdr* elf_header,Elf32_Shdr* sections_header,SectionContent TableName)
 {     
      printf("Il y a %i en-têtes de section, débutant à l'adresse de décalage 0x%X:\n\n",elf_header->e_shnum,elf_header->e_shoff);
      printf("En-têtes de section : \n");
      printf("  [Nr] Nom              Type            Adr      Décala.Taille ES Fan LN Inf Al\n");
      for(int i=0;i<elf_header->e_shnum;i++) 
      {         
         printf("  [ %d] ",i);         
         if(TableName.size !=0)
         {
            printf("%-16s",&TableName.content[sections_header[i].sh_name]); //read the first byte in .strtab
         }
         else
         {
            printf("%-16s",""); //no .strtab
         }         
         printf(" %-15s",ENUM_SectionHeaderType(sections_header[i].sh_type)); //what type is it?
         printf(" %08X",sections_header[i].sh_addr); //Address if it's appear in memory (.bss)
         printf(" %06X",sections_header[i].sh_offset); //offset of the section from the beginning of the file
         printf(" %06X",sections_header[i].sh_size); //size of the section
         printf(" %02X",sections_header[i].sh_entsize); //?? ES
         printf(" %s",ENUM_Flags(sections_header[i].sh_flags)); //Flags (Clé des fanions : voir readelf)
         printf("  %i",sections_header[i].sh_link); //for specific use (i think we will not use it)
         if(sections_header[i].sh_link == 0 && sections_header[i].sh_type != SHT_REL && sections_header[i].sh_type != SHT_SYMTAB )
         {
            printf("   0"); //Figure 1-12 show : other and sh_link=SHN_UNDEF so sh_link=0
         }
         else
         {
            printf("   %i",sections_header[i].sh_type); //for specific use (i think we will not use it)
         }
         
         
         printf("  %i",sections_header[i].sh_addralign); //Address alignement (don't mind this if it's 0 or 1) used with sh_addr in some case
         printf("\n");
      }
 }

 char * ExtractName(char* FullName,int index)
 {
   int i=index;
   while(FullName[i]!='\0')
      i++;
   char * name = malloc(i+1);
   for(int j=index;j<i;j++)
      name[j-index]=FullName[j];
   return name;
   
 }
 
 int GetHeader(SectionContent Names,Elf32_Shdr * Sections,const char * section_name,int nbsections)
 {
   int i=0;
   do
   {
      i++;            
      if(!strcmp(&Names.content[Sections[i].sh_name],section_name))
      {            
            return i;
      }                       
   }while(i<nbsections);
   return -1;
 }

 char * ENUM_Flags(Elf32_Word flags)
 {
      //those flag show us if the section can allocate/exec or write
      char* f_flag = malloc(4);
      f_flag[3]='\0';
      if(flags & 0x1)
      {
         f_flag[1]='W';
      }
      else
      {
         f_flag[1]=' ';
      }
      if(flags & SHF_ALLOC)
      {
         f_flag[2]='A';
      }
      else
      {
         f_flag[2]=' ';
      }
      if(flags & SHF_EXECINSTR)
      {
         f_flag[0]='X'; 
      }
      else
      {
         f_flag[0]=' ';
      }
      if(flags & SHF_MASKPROC)
      {
         f_flag[0]='?'; //idk
      }      
      return f_flag;
 }

 char * ENUM_SectionHeaderType(Elf32_Word type)
 {
   //return a string with the value associate
   char * c_word = malloc(12);
   switch(type)
   {
      case SHT_NULL:
         strcpy(c_word,"NULL");
         break;
      case SHT_PROGBITS:
         strcpy(c_word,"PROGBITS");
         break;
      case SHT_SYMTAB:
         strcpy(c_word,"SYMTAB");
         break;
      case SHT_STRTAB:
         strcpy(c_word,"STRTAB");
         break;
      case SHT_RELA:
         strcpy(c_word,"RELA");
         break;
      case SHT_HASH:
         strcpy(c_word,"HASH");
         break;
      case SHT_DYNAMIC:
         strcpy(c_word,"DYNAMIC");
         break;
      case SHT_NOTE:
         strcpy(c_word,"NOTE");
         break;
      case SHT_NOBITS:
         strcpy(c_word,"NOBITS");
         break;
      case SHT_REL:
         strcpy(c_word,"REL");
         break;
      case SHT_SHLIB:
         strcpy(c_word,"SHLIB");
         break;
      case SHT_DYNSYM:
         strcpy(c_word,"DYNSYM");
         break;
      case SHT_LOPROC:
         strcpy(c_word,"LOPROC");
         break;
      case SHT_HIPROC:
         strcpy(c_word,"HIPROC");
         break;
      case SHT_LOUSER:
         strcpy(c_word,"LOUSER");
         break;
      case SHT_HIUSER:
         strcpy(c_word,"HIUSER");
         break;
   }
   return c_word;
 }

