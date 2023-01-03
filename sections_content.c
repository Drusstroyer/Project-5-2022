#include "sections_content.h"

Elf32_Sym* ReadSymbtab(Elf32_Shdr sections_header,int nbSym, SectionContent Content)
{ 
    Elf32_Sym * Symtab = malloc(sizeof(Elf32_Sym)*nbSym);
    printf("struct: %li",sections_header.sh_size/sizeof(Elf32_Sym));
    printf("EL : %i",sections_header.sh_entsize);
    return Symtab;
}

void ShowSymtab(Elf32_Sym * symbol_tab,int nbSym)
{
    printf("Symbol table '%i' contains %i entries:\n", symbol_tab->st_name, nbSym);
    printf("  Nul:    Value    Size Type    Bind    Vis    Ndx Name\n");
    for(int i=0; i< nbSym; i++)     //check boucle condition
    {
        printf("    %i:", i);
        printf("    %X", symbol_tab->st_value);
        printf("    %i", symbol_tab->st_size);
        printf("    %s", ENUM_TableSymbolType(symbol_tab->st_info)); //type
        printf("    %s", ENUM_TableSymbolBinding(symbol_tab->st_info));
        printf(" ");
        printf(" \n");
        //printf(" %s\n", symbol_tab->st_name);
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