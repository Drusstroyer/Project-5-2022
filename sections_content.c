#include "sections_content.h"

Elf32_Sym* ReadSymbtab(Elf32_Shdr sections_header,int nbSym, SectionContent* Content)
{ 
    Elf32_Sym * Symtab = malloc(sizeof(Elf32_Sym)*nbSym);
    Symtab = (Elf32_Sym*)Content->content;
    return Symtab;
}

void ShowSymtab(Elf32_Sym * symbol_tab,int nbSym,SectionContent SymbolName)
{    
    printf("Symbol table \'.symtab\' contains %i entries:\n", nbSym);
    printf("  Num:     Value  Size Type    Bind   Vis      Ndx Name\n");
    for(int i=0; i< nbSym; i++)     //check boucle condition
    {
        printf("     %i:", i);
        printf(" %08X", symbol_tab[i].st_value); //value of symbol
        printf("%6i", symbol_tab[i].st_size); //his size
        printf(" %-7s", ENUM_TableSymbolType(symbol_tab[i].st_info)); //type : SECTION, NOTYPE, ...
        printf(" %-6s", ENUM_TableSymbolBinding(symbol_tab[i].st_info));
        printf(" %3s",ENUM_TableSymbolVis(symbol_tab[i].st_other));// the rights
        printf("  %3s",ENUM_TableSymbolNdx(symbol_tab[i].st_shndx)); //section index
        printf(" %s",&SymbolName.content[symbol_tab[i].st_name]); //name
        printf(" \n");
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
         sprintf(c_half, "%d",type);
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