#include "sections_content.h"

SectionContent* ReadAllSections(FILE* file,Elf32_Shdr* section_header, int nb,SectionContent SectionName)
{
    SectionContent* section_cont = malloc(sizeof(SectionContent)*nb);
    for(int i=0;i<nb;i++)
    {
      section_cont[i]=*GetContent(file,section_header[i]);  
      section_cont[i].name=ExtractName(SectionName.content,section_header[i].sh_name);
    }
    return section_cont;
}

char * CopyContent(SectionContent Section)
{
   //copy the content of a section
   char * result = malloc(Section.size);
   for(int i=0;i<Section.size;i++)
      result[i]=Section.content[i];
   return result;
}

 SectionContent* GetContent(FILE * file,Elf32_Shdr section_header)
 {    
      //get the content of the section from file
      SectionContent* content=malloc(sizeof(SectionContent));
      if(section_header.sh_size)
      {
         content->content = malloc(section_header.sh_size);
         content->size=section_header.sh_size;
         fseek(file,section_header.sh_offset,SEEK_SET);
         fread(content->content,section_header.sh_size,1,file);
      }
      return content;
 }

void disp_section_content(SectionContent * section,int size){   
   for (int i=1;i<size;i++){ //one section per time
      fprintf(stdout," %s\n", section[i].name);
      fprintf(stdout," %i\n", section[i].size);
      for(int j=0;j<section[i].size;j++)
      {
         printf("%04X ",section[i].content[j]);                     
         if(j%8==0)
            printf("\n");
      }
   }
}

Elf32_Sym* ReadSymbtab(Elf32_Shdr sections_header, SectionContent* Content)
{ 
    int size = sections_header.sh_size/sections_header.sh_entsize;
    Elf32_Sym * Symtab = malloc(sizeof(Elf32_Sym)*size);
    Symtab = (Elf32_Sym*)Content->content;   
    return Symtab;
}

void ShowSymtab(Elf32_Sym * symbol_tab,int size,SectionContent SymbolName)
{    
    printf("Symbol table \'.symtab\' contains %i entries:\n", size);
    printf("  Num:     Value  Size Type    Bind   Vis      Ndx Name\n");    
    for(int i=0; i< size; i++)     //check boucle condition
    {
        fprintf(stdout,"     %i:", i);
        fprintf(stdout," %08X", symbol_tab[i].st_value); //value of symbol
        fprintf(stdout,"%6i", symbol_tab[i].st_size); //his size
        fprintf(stdout," %-7s", ENUM_TableSymbolType(symbol_tab[i].st_info)); //type : SECTION, NOTYPE, ...
        fprintf(stdout," %-6s", ENUM_TableSymbolBinding(symbol_tab[i].st_info));
        fprintf(stdout," %3s",ENUM_TableSymbolVis(symbol_tab[i].st_other));// the rights
        fprintf(stdout,"  %3s",ENUM_TableSymbolNdx(symbol_tab[i].st_shndx)); //section index
        fprintf(stdout," %s",&SymbolName.content[symbol_tab[i].st_name]); //name
        fprintf(stdout," \n");
    }     
}

char * ENUM_TableSymbolType(Elf32_Word type)
{
   //return a string with the value associate
   char * c_word = malloc(12);
   type=ELF32_ST_TYPE(type);
   switch(type)
   {
      case STT_NOTYPE:
         strcpy(c_word,"NOTYPE");
         break;
      case STT_OBJECT:
         strcpy(c_word,"OBJECT");
         break;
      case STT_FUNC:
         strcpy(c_word,"FUNC");
         break;
      case STT_SECTION:
         strcpy(c_word,"SECTION");
         break;
      case STT_FILE:
         strcpy(c_word,"FILE");
         break;
      case STT_LOPROC:
         strcpy(c_word,"LOPROC");
         break;
      case STT_HIPROC:
         strcpy(c_word,"HIPROC");
         break;
   }
   return c_word;
}

char * ENUM_TableSymbolBinding(Elf32_Word type)
{
   //return a string with the value associate
   char * c_word = malloc(12);
   type=ELF32_ST_BIND(type);
   switch(type)
   {
      case STB_LOCAL:
         strcpy(c_word,"LOCAL");
         break;
      case STB_GLOBAL:
         strcpy(c_word,"GLOBAL");
         break;
      case STB_WEAK:
         strcpy(c_word,"WEAK");
         break;
      case STB_LOPROC:
         strcpy(c_word,"LOPROC");
         break;
      case STB_HIPROC:
         strcpy(c_word,"HIPROC");
         break;
   }
   return c_word;
}

char * ENUM_TableSymbolNdx(Elf32_Half type)
{
   char * c_half = malloc(12);
   switch(type)
   {
      case SHN_UNDEF:
         strcpy(c_half,"UND");
         break;
      case SHN_ABS:
         strcpy(c_half,"ABS");
         break;
      case SHN_LOPROC:
         strcpy(c_half,"LOPROC");
         break;
      case SHN_HIPROC:
         strcpy(c_half,"HIPROC");
         break;
      case SHN_COMMON:
         strcpy(c_half,"COMMON");
         break;
      case SHN_HIRESERVE:
         strcpy(c_half,"HIRESERVE");
         break;
      default:             
         sprintf(c_half, "%d", type);
         break;
   }
   return c_half;
}

char *ENUM_TableSymbolVis(unsigned char type){
   char * c_char = malloc(12);
   type=ELF32_ST_VISIBILITY(type);
   switch (type)
   {
      case STV_DEFAULT:
            strcpy(c_char,"DEFAULT");
            break;
         case STV_INTERNAL:
            strcpy(c_char,"INTERNAL");
            break;
         case STV_HIDDEN:
            strcpy(c_char,"HIDDEN");
            break;
         case STV_PROTECTED:
            strcpy(c_char,"PROTECTED");
            break;
   }
   return c_char;
}