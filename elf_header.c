#include "elf_header.h"
#define EM_MIPS_RS4_BE 0x10
//#define EM_ARM 0x28

void load_elf_header(FILE * file, Elf32_Ehdr *const elf_header)
{
   fseek(file, 0, SEEK_SET);
   fread(elf_header, 1, sizeof(Elf32_Ehdr), file);
}

int check_magic_number(const Elf32_Ehdr *const elf_header)
{ /*check elf header magic numbers*/
   if ((elf_header->e_ident[EI_MAG0] == ELFMAG0) && (elf_header->e_ident[EI_MAG1] == ELFMAG1) && (elf_header->e_ident[EI_MAG2] == ELFMAG2) && (elf_header->e_ident[EI_MAG3] == ELFMAG3))
   {
      return 1;
   }
   return 0;
}

int check_object_validity(const Elf32_Ehdr *const elf_header)
{
   /*check if object is valid (either 32 or 64 bits)*/
   if (!(elf_header->e_ident[EI_CLASS] == ELFCLASSNONE))
   {
      return 1;
   }
   return 0;
}

int check_data_validity(const Elf32_Ehdr *const elf_header)
{
   /*check if data is valid*/
   if (!(elf_header->e_ident[EI_DATA] == ELFDATANONE))
   {
      return 1;
   }
   return 0;
}

int check_version_validity(const Elf32_Ehdr *const elf_header)
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
   printf("ELF HEADER:\n");
   printf("Magique:                                            %x %x %x %x\n", elf_header->e_ident[EI_MAG0], elf_header->e_ident[EI_MAG1], elf_header->e_ident[EI_MAG2], elf_header->e_ident[EI_MAG3]); /*we suppose header exists*/
   printf("Classe:                                             ");
   switch(elf_header->e_ident[EI_CLASS]){
      case ELFCLASSNONE: printf("Invalid class"); break;
      case ELFCLASS32: printf("ELF32, 32-bits objects");break;
      case ELFCLASS64: printf("ELF64, 64-bits objects"); break;
   }
   printf("\n");

   printf("Donnees:                                            ");
   switch(elf_header->e_ident[EI_DATA]){
      case ELFDATANONE: printf("Invalid data encoding"); break;
      case ELFDATA2LSB: printf("complement à 2, système à octets de poids faible d'abord (little endian)");break;
      case ELFDATA2MSB: printf("complement à 2, système à octets de poids fort d'abord (big endian)"); break;
   }
   printf("\n");

   printf("Version:                                            ");
   switch(elf_header->e_ident[EI_VERSION]){
      case EV_CURRENT: printf("1 (current)");break;
      case EV_NONE: printf("0 (invalid)");break;
   }
   printf("\n");

   printf("Type:                                               ");
   switch(elf_header->e_type){
		case ET_NONE: printf("No file type");break;
      case ET_REL: printf("Relocatable file");break;
		case ET_EXEC: printf("Executable file");break;
		case ET_DYN: printf("Shared object file");break;
		case ET_CORE: printf("Core file");break;
		case ET_LOPROC: printf("Processor-specific LO");break;
		case ET_HIPROC: printf("Processor-specific HI");break;
	}
   printf("\n");
   printf("Machine:                                            ");
   switch(elf_header->e_machine){
		case ET_NONE: printf("No Machine");break;
		case EM_M32: printf("AT&T WE 32100");break;
		case EM_SPARC: printf("SPARC");break;
		case EM_386: printf("Intel Architecture");break;
		case EM_68K: printf("Motorola 68000");break;
		case EM_88K: printf("Motorola 88000");break;
		case EM_860: printf("Intel 80860");break; 
		case EM_MIPS: printf("MIPS RS3000 Big-Endian");break;
		case EM_MIPS_RS4_BE: printf("MIPS RS4000 Big-Endian");break;
		case EM_ARM: printf("ARM\n");break; 
		default: printf("Reserved for future use");break;
	}
   printf("\n");
   
    printf("Version:                                            ");
   switch(elf_header->e_version){
      case EV_CURRENT: printf("1 (current)");break;
      case EV_NONE: printf("0 (invalid)");break;
   }
   printf("\n");

   printf("Adresse du point d'entrée:                          %x\n", elf_header->e_entry);
   printf("Début des en-têtes de programme:                    %d (octets deans le fichier)\n", elf_header->e_phoff);
   printf("Début des en-têtes de section:                      %d (octets deans le fichier)\n", elf_header->e_shoff);
   printf("Fanions:                                            %x\n", elf_header->e_flags);
   printf("Taille de cet en-tête:                              %d (octets)\n", elf_header->e_ehsize);
   printf("Taille de l'en-tête du programme                    %d (octets)\n", elf_header->e_phentsize);
   printf("Nombre d'en-tête du programme:                      %d\n", elf_header->e_phnum);
   printf("Taille des en-têtes de section:                     %d (octets)\n", elf_header->e_shentsize);
   printf("Nombre d'en-têtes de section:                       %d\n", elf_header->e_shnum);
   printf("Table d'indexes des chaines d'en-tête de section:   %d\n", elf_header->e_shstrndx);
}

int main(int argc, char * argv[])
{
   FILE * fp = fopen(argv[1], "r");
   Elf32_Ehdr elf_header; 
   load_elf_header(fp, &elf_header);
   check_elf_header(&elf_header);
   display_elf_header(&elf_header);
   return 1;
}