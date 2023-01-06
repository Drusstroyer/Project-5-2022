#include "sections_relocation.h"

Elf32_Rela *ReadRelatab(FILE *f_elf, Elf32_Shdr *section_header, int Reltab_index)
{
   SectionContent *Content = GetContent(f_elf, section_header[Reltab_index]); // name stocked in content->name
   // SectionName=GetContent(f_elf,section_header[SymStringName_index])
   Elf32_Rela *reloc_tab = malloc(sizeof(Elf32_Rela) * ((section_header[Reltab_index].sh_size) / (section_header[Reltab_index].sh_entsize)));
   if (!reloc_tab)
   {
      perror("Error: ");
      exit(1);
   }
   reloc_tab = (Elf32_Rela *)Content->content;
   return reloc_tab;
}

Elf32_Rel *ReadReltab(FILE *f_elf, Elf32_Shdr *section_header, int Reltab_index)
{
   SectionContent *Content = GetContent(f_elf, section_header[Reltab_index]); // name stocked in content->name
   // SectionName=GetContent(f_elf,section_header[SymStringName_index])
   Elf32_Rel *reloc_tab = malloc(sizeof(Elf32_Rel) * ((section_header[Reltab_index].sh_size) / (section_header[Reltab_index].sh_entsize)));
   if (!reloc_tab)
   {
      perror("Error: ");
      exit(1);
   }
   reloc_tab = (Elf32_Rel *)Content->content;
   return reloc_tab;
}
// Elf32_Rela* ReadRelatab(Elf32_Shdr sections_header,int nbRel, SectionContent* Content)
// {
//     Elf32_Rela * reloc_tab = malloc(sizeof(Elf32_Rel)*nbRel);
//     if(!reloc_tab){
//         perror("Error: ");
//         exit(1);
//     }
//     reloc_tab = (Elf32_Rela*)Content->content;
//     return reloc_tab;
// }

// Elf32_Rel* ReadReltab(Elf32_Shdr sections_header, int nbRel, SectionContent* Content)
// {
//     Elf32_Rel * reloc_tab = malloc(sizeof(Elf32_Rel)*nbRel);
//     if(!reloc_tab){
//         perror("Error: ");
//         exit(1);
//     }
//     reloc_tab = (Elf32_Rel*)Content->content;
//     return reloc_tab;
// }

// void ShowReloctab(Elf32_Rela *reloc_tab, Elf32_Shdr *section_header, int Reltab_index, SectionContent Content, SectionContent SymbolName)
// {
//    if (section_header[Reltab_index].sh_type == SHT_RELA)
//    {
//       printf("\n");
//       ShowRelatab(reloc_tab, (section_header[Reltab_index].sh_size) / section_header[Reltab_index].sh_entsize, Content, SymbolName);
//    }
//    else if (section_header[Reltab_index].sh_type == SHT_REL)
//    {
//       // Elf32_Rel* tmp3=ReadReltab(h_symtab, (section_header[Reltab_index].sh_size)/section_header[Reltab_index].sh_entsize, Content2);
//       printf("\n");
//       Elf32_Rel *reloc_tab_rel = (Elf32_Rel *)reloc_tab;
//       ShowReltab(reloc_tab_rel, (section_header[Reltab_index].sh_size) / section_header[Reltab_index].sh_entsize, Content, SymbolName);
//    }
// }

void ShowRelatab(Elf32_Rela *reloc_tab, int nbRel, SectionContent Content, SectionContent SymbolName, Elf32_Sym * symbol_tab)
{
   printf("Relocation section \'.rela.text\' at offset 0x%x contains %i entries:\n", reloc_tab->r_offset, nbRel);
   printf("  Offset          Info           Type           Sym. Value    Sym. Name + Addend\n");
   for (int i = 0; i < nbRel; i++) // check boucle condition
   {
      printf("%08X", reloc_tab[i].r_offset);                                    // offset
      printf(" %08X", reloc_tab[i].r_info);                                     // info
      printf("    %s", ENUM_TableRelocType(ELF32_R_TYPE(reloc_tab[i].r_info))); // type : SECTION, NOTYPE, ...
      printf(" %08X", symbol_tab[ELF32_R_SYM(reloc_tab[i].r_info)].st_value);
      printf(" %s + %d", &SymbolName.content[symbol_tab[ELF32_R_SYM(reloc_tab[i].r_info)].st_name], reloc_tab[i].r_addend);
      printf(" \n");
   }
}

void ShowReltab(Elf32_Rel *reloc_tab, int nbRel, SectionContent Content, SectionContent SymbolName, Elf32_Sym * symbol_tab)
{
   printf("Relocation section \'.rel.text\' at offset 0x%x contains %i entries:\n", reloc_tab->r_offset, nbRel);
   printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
   for (int i = 0; i < nbRel; i++) // check boucle condition
   {
      printf("%08X", reloc_tab[i].r_offset);                                   // offset
      printf("  %08X", reloc_tab[i].r_info);                                   // info
      printf(" %s", ENUM_TableRelocType(ELF32_R_TYPE(reloc_tab[i].r_info))); // type : SECTION, NOTYPE, ...*
      printf("      %08X", symbol_tab[ELF32_R_SYM(reloc_tab[i].r_info)].st_value);
      printf("    %6s", &SymbolName.content[symbol_tab[ELF32_R_SYM(reloc_tab[i].r_info)].st_name]);
      printf(" \n");
   }
}

char *ENUM_TableRelocType(Elf32_Word type)
{
   // return a string with the value associate
   char *c_word = malloc(25);
   type = ELF32_R_TYPE(type);
   switch (type)
   {
   case R_ARM_NONE:
      strcpy(c_word, "R_ARM_NONE");
      break;
   case R_ARM_PC24:
      strcpy(c_word, "R_ARM_PC24");
      break;
   case R_ARM_CALL:
      strcpy(c_word, "R_ARM_CALL");
      break;
   case R_ARM_JUMP24:
      strcpy(c_word, "R_ARM_JUMP24");
      break;
   case R_ARM_PREL31:
      strcpy(c_word, "R_ARM_PREL31");
      break;
   case R_ARM_V4BX:
      strcpy(c_word, "R_ARM_V4BX");
      break;
   case R_ARM_MOVT_ABS:
      strcpy(c_word, "R_ARM_MOVT_ABS");
      break;
   case R_ARM_MOVW_ABS_NC:
      strcpy(c_word, "R_ARM_MOVW_ABS_NC");
      break;
   case R_ARM_ABS32:
      strcpy(c_word, "R_ARM_ABS32");
      break;
   case R_ARM_THM_JUMP24:
      strcpy(c_word, "R_ARM_THM_JUMP24");
      break;
   case R_ARM_TLS_CALL:
      strcpy(c_word, "R_ARM_THM_CALL");
      break;
   case R_ARM_THM_JUMP19:
      strcpy(c_word, "R_ARM_THM_JUMP19");
      break;
   case R_ARM_THM_ABS5:
      strcpy(c_word, "R_ARM_THM_ABS5");
      break;
   case R_ARM_THM_JUMP6:
      strcpy(c_word, "R_ARM_THM_JUMP6");
      break;
   }
   return c_word;
}
/*Other option, less code using casts*/