#include "elf_header.h"

void load_elf_header(FILE* file, Elf32_Ehdr * const elf_header)
{
   fseek(file, 0, SEEK_SET);
   fread(elf_header, 1, sizeof(Elf32_Ehdr), file);
}

int check_magic_number(const Elf32_Ehdr * const elf_header)
{ /*check elf header magic numbers*/
   if (elf_header->e_ident[EI_MAG0] == ELFMAG0 && elf_header->e_ident[EI_MAG1 == ELFMAG1] && elf_header->e_ident[EI_MAG2 == ELFMAG2] && elf_header->e_ident[EI_MAG3 == ELFMAG3])
   {
      return 1;
   }
   return 0;
}

int check_object_validity(const Elf32_Ehdr * const elf_header)
{
   /*check if object is valid (either 32 or 64 bits)*/
   if (!elf_header->e_ident[EI_CLASS] == ELFCLASSNONE)
   {
      return 1;
   }
   return 0;
}

int check_data_validity(const Elf32_Ehdr * const elf_header)
{
   /*check if data is valid*/
   if (!elf_header->e_ident[EI_DATA] == ELFDATANONE)
   {
      return 1;
   }
   return 0;
}

int check_version_validity(const Elf32_Ehdr * const elf_header)
{
   /*check if version is EV_CURRENT*/
   if (elf_header->e_ident[EI_VERSION] == EV_CURRENT)
   {
      return 1;
   }
   return 0;
}

int check_elf_header(const Elf32_Ehdr *const elf_header)
{
   if (!check_magic_number(elf_header))
   {
      /*ERROR*/
      printf("Bad elf header, magic numbers invalid\n");
      return 0;
   }
   else if (!check_object_validity(elf_header))
   {
      /*ERROR*/
      printf("Invalid class, different from 32 or 64 bits\n");
      return 0;
   }
   else if (!check_data_validity(elf_header))
   {
      /*ERROR*/
      printf("Invalid data encoding\n");
      return 0;
   }
   else if (!check_version_validity(elf_header))
   {
      /*ERROR*/
      printf("Bad elf version, EV_NONE\n");
      return 0;
   }
   return 1;
}

void display_elf_header(const Elf32_Ehdr *const elf_header)
{
   printf("\ne_ident: %s", elf_header->e_ident);
   printf("e_type: %d\n", elf_header->e_type);
   printf("ELF HEADER:\n");
	switch(elf_header->e_machine){
		case 0x0: printf("Machine: No Machine\n");break;
		case 0x1: printf("Machine: AT&T WE 32100\n");break;
		case 0x2: printf("Machine: SPARC\n");break;
		case 0x3: printf("Machine: Intel Architecture\n");break;
		case 0x4: printf("Machine: Motorola 68000\n");break;
		case 0x5: printf("Machine: Motorola 88000\n");break;
		case 0x7: printf("Machine: Intel 80860\n");break; 
		case 0x8: printf("Machine: MIPS RS3000 Big-Endian\n");break;
		case 0x10: printf("Machine: MIPS RS4000 Big-Endian\n");break;
		case 0x28: printf("Machine: ARM\n");break;
		default: printf("Machine: Reserved for future use\n");break;
	}
   
   printf("e_machine: %d\n", elf_header->e_machine);
   printf("e_version: %d\n", elf_header->e_version);
   printf("e_entry: %d\n", elf_header->e_entry);
   printf("e_phoff: %d\n", elf_header->e_phoff);
   printf("e_shoff: %d\n", elf_header->e_shoff);
   printf("e_flags: %d\n", elf_header->e_flags);
   printf("e_ehsize: %d\n", elf_header->e_ehsize);
   printf("e_phentsize: %d\n", elf_header->e_phentsize);
   printf("e_phnum: %d\n", elf_header->e_phnum);
   printf("e_shentsize: %d\n", elf_header->e_shentsize);
   printf("e_shnum: %d\n", elf_header->e_shnum);
   printf("e_shstrndx: %d\n", elf_header->e_shstrndx);
}