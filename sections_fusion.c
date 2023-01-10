#include "sections_fusion.h"

void FusionFile(FILE* file1, FILE* file2)
{
    Elf32_Ehdr header1;
    Elf32_Ehdr header2;
    load_elf_header(file1,&header1);    
    load_elf_header(file2,&header2);    
    Elf32_Shdr* section_header1 = malloc(sizeof(Elf32_Shdr)*header1.e_shnum);
    Elf32_Shdr* section_header2 = malloc(sizeof(Elf32_Shdr)*header2.e_shnum);    
    load_sections_header(file1,&header1,section_header1);   
    load_sections_header(file2,&header2,section_header2); 
    
    SectionContent SectionName1=*GetContent(file1,section_header1[header1.e_shstrndx]);    
    SectionContent SectionName2=*GetContent(file2,section_header2[header2.e_shstrndx]);
    SectionContent* SectionFile1=ReadAllSections(file1,section_header1,header1.e_shnum,SectionName1);
    SectionContent* SectionFile2=ReadAllSections(file2,section_header2,header2.e_shnum,SectionName2);

    //FUSION PROGBITS
    printf("Fusion: Progbits\n");
    FusionContent ProgBits=*FusionSectionProgBits(section_header1,SectionFile1,section_header2,SectionFile2,header1.e_shnum,header2.e_shnum);
    Elf32_Ehdr new_header;
    memcpy(&new_header,&header1,sizeof(Elf32_Ehdr));
    new_header.e_shnum=ProgBits.nb;
    //new_header.e_shstrndx=GetHeader(*SectionName,section_header,".symtab",NewContents.nb); //it will not change

    //FUSION SYMBOLS
    printf("Fusion: Symtabs\n");
    int symtab1=GetHeader(ProgBits.contents[new_header.e_shstrndx],ProgBits.headers,".symtab",new_header.e_shnum);
    int symtabname1=GetHeader(ProgBits.contents[new_header.e_shstrndx],ProgBits.headers,".strtab",new_header.e_shnum);
    int symtabname2=GetHeader(SectionName2,section_header2,".strtab",header2.e_shnum);
    int symtab2=GetHeader(SectionName2,section_header2,".symtab",header2.e_shnum);    
    int symtabsize1=ProgBits.headers[symtab1].sh_size / ProgBits.headers[symtab1].sh_entsize;
    int symtabsize2=section_header2[symtab2].sh_size / section_header2[symtab2].sh_entsize;

    SectionContent* Symbols=FusionSymtab(ReadSymbtab(ProgBits.headers[symtab1],&ProgBits.contents[symtab1]),ReadSymbtab(section_header2[symtab2],&SectionFile2[symtab2]),ProgBits.contents[symtabname1],SectionFile2[symtabname2],symtabsize1,symtabsize2);
    
    free(ProgBits.contents[symtabname1].content);
    free(ProgBits.contents[symtab1].content);
    
    //replace with new symtab
    ProgBits.contents[symtab1].content=Symbols[0].content;
    ProgBits.contents[symtab1].size=Symbols[0].size;

    //replace with new symbols name
    ProgBits.contents[symtabname1].content=Symbols[1].content;
    ProgBits.contents[symtabname1].size=Symbols[1].size;
    //display_sections_header(&new_header,ProgBits.headers,ProgBits.contents[new_header.e_shstrndx]);    
    ShowSymtab(ReadSymbtab(ProgBits.headers[symtab1], &ProgBits.contents[symtab1]), ProgBits.contents[symtab1].size/sizeof(Elf32_Sym), ProgBits.contents[symtabname1]);
    printf("\nFusion Success\n");
}

SectionContent* FusionSymtab(Elf32_Sym* Sym1,Elf32_Sym* Sym2,SectionContent Name1,SectionContent Name2,int nb1,int nb2)
{     
    int size = nb1+nb2;
    Elf32_Sym * Symtab = malloc(sizeof(Elf32_Sym)*size);
    int b_check=1;
    int last_index_name=0;
    char * SymbolsName=malloc(2000);
    //&SymbolName.content[symbol_tab[i].st_name]    
    for (int i=0;i<nb1;i++){       
        if(ELF32_ST_BIND(Sym1[i].st_info) == STB_GLOBAL){
            for (int j=0;j<nb2;j++){
                if(ELF32_ST_BIND(Sym2[j].st_info) == STB_GLOBAL){                    
                    if(!strcmp(&Name1.content[Sym1[i].st_name],&Name2.content[Sym2[j].st_name])){
                        b_check=0;                    
                        if(Sym1[i].st_shndx == SHN_UNDEF && Sym2[j].st_shndx == SHN_UNDEF){
                            if(Sym1[i].st_name != 0)
                                strcpy(&SymbolsName[last_index_name],ExtractName(Name1.content,Sym1[i].st_name));
                            Symtab[i].st_name=last_index_name;
                            last_index_name+=strlen(ExtractName(Name1.content,Sym1[i].st_name))+1;
                            memcpy(&Symtab[i],&Sym1[i],sizeof(Elf32_Sym));
                        }
                        else if(Sym1[i].st_shndx == SHN_UNDEF || Sym2[j].st_shndx == SHN_UNDEF){
                            if(Sym1[i].st_shndx == SHN_UNDEF)
                            {
                                memcpy(&Symtab[i],&Sym2[j],sizeof(Elf32_Sym));
                                if(Sym2[i].st_name != 0)
                                    strcpy(&SymbolsName[last_index_name],ExtractName(Name2.content,Sym2[i].st_name));
                                Symtab[i].st_name=last_index_name;
                                last_index_name+=strlen(ExtractName(Name2.content,Sym2[i].st_name))+1;
                            }
                            else
                            {
                                memcpy(&Symtab[i],&Sym1[i],sizeof(Elf32_Sym));
                                if(Sym1[i].st_name != 0)
                                    strcpy(&SymbolsName[last_index_name],ExtractName(Name1.content,Sym1[i].st_name));
                                Symtab[i].st_name=last_index_name;
                                last_index_name+=strlen(ExtractName(Name1.content,Sym1[i].st_name))+1;
                            }
                        }
                        else
                        {
                            printf("Error: Redefined");
                            exit(1);
                        }
                    }
                }                                
            }
            if(b_check)
            {
                memcpy(&Symtab[i],&Sym1[i],sizeof(Elf32_Sym));
                if(Sym1[i].st_name != 0)
                    strcpy(&SymbolsName[last_index_name],ExtractName(Name1.content,Sym1[i].st_name));
                Symtab[i].st_name=last_index_name;
                last_index_name+=strlen(ExtractName(Name1.content,Sym1[i].st_name))+1;
            }
            b_check=1;
        }
        else if (ELF32_ST_BIND(Sym1[i].st_info) == STB_LOCAL){
            memcpy(&Symtab[i],&Sym1[i],sizeof(Elf32_Sym));            
            if(Sym1[i].st_name != 0)            
                strcpy(&SymbolsName[last_index_name],ExtractName(Name1.content,Sym1[i].st_name));                    
            Symtab[i].st_name=last_index_name;
            last_index_name+=strlen(ExtractName(Name1.content,Sym1[i].st_name))+1;
        }
    }
    b_check=1;
    int nbelem=0;    
    int red=0;
    for (int i=0;i<nb2;i++){ //2eme
        if(ELF32_ST_BIND(Sym2[i].st_info) == STB_GLOBAL){
            
            for (int j=0;j<nb1;j++){ //symtab
                if(!strcmp(&Name1.content[Sym1[j].st_name],&Name2.content[Sym2[i].st_name])){ // to do: same name but one is global and other is local 
                    b_check=0;
                    red++;
                    break;
                }

            }
            if(b_check){                       
                memcpy(&Symtab[i+nb1-red],&Sym2[i],sizeof(Elf32_Sym));
                if(Sym2[i].st_name != 0)
                    strcpy(&SymbolsName[last_index_name],ExtractName(Name2.content,Sym2[i].st_name));            
                Symtab[i+nb1-red].st_name=last_index_name;                
                last_index_name+=strlen(ExtractName(Name2.content,Sym2[i].st_name))+1;
                nbelem++;
            } 
            b_check=1;      
        }
        else{            
            memcpy(&Symtab[i+nb1],&Sym2[i],sizeof(Elf32_Sym));
            if(Sym2[i].st_name != 0)
                strcpy(&SymbolsName[last_index_name],ExtractName(Name2.content,Sym2[i].st_name));            
            Symtab[i+nb1].st_name=last_index_name;
            last_index_name+=strlen(ExtractName(Name2.content,Sym2[i].st_name))+1;
            nbelem++;
        }
    }               
    SectionContent* Symbols=malloc(sizeof(SectionContent)*2);
    SymbolsName=realloc(SymbolsName,last_index_name);
    Symtab=realloc(Symtab,sizeof(Elf32_Sym)*(nbelem+nb1));

    //symtab
    Symbols[0].content=(char*)Symtab;    
    Symbols[0].size=sizeof(Elf32_Sym)*(nbelem+nb1);
    //symbols name
    Symbols[1].content=SymbolsName;
    Symbols[1].size=last_index_name;
    return Symbols;
}

FusionContent* FusionSectionProgBits(Elf32_Shdr* SectionHeader1,SectionContent* Section1,Elf32_Shdr* SectionHeader2 , SectionContent* Section2, int nbSection1, int nbSection2)
{
    int size = nbSection1+nbSection2;              
    int n_size=0;
    int b_exist=0;
    int flag=0;
    int buffsize=0;
    Elf32_Shdr* HeaderFusion = malloc(sizeof(Elf32_Shdr)*size);
    char * New_Names = malloc(2000);
    int last_index_name=0;
    SectionContent* SectionFusion = malloc(sizeof(SectionContent)*size);    
    for(int i=0;i<nbSection1;i++)
    {
        memcpy(&HeaderFusion[i],&SectionHeader1[i],sizeof(Elf32_Shdr));        
        HeaderFusion[i].sh_offset+=buffsize;
        if(HeaderFusion[i].sh_addralign>=2 && HeaderFusion[i].sh_offset%HeaderFusion[i].sh_addralign>0)
        {
            HeaderFusion[i].sh_offset+=(4-HeaderFusion[i].sh_offset%HeaderFusion[i].sh_addralign);
            buffsize+=(4-HeaderFusion[i].sh_offset%HeaderFusion[i].sh_addralign);
        }        
        
        if(SectionHeader1[i].sh_type == SHT_PROGBITS)
        {
            for(int j=0;j<nbSection2;j++)
            {
                if(SectionHeader2[j].sh_type == SHT_PROGBITS && !strcmp(Section1[i].name,Section2[j].name))
                {
                    //merge section PROGBIT  
                    b_exist=1;
                    SectionFusion[i].size=Section1[i].size+Section2[j].size;     
                    HeaderFusion[i].sh_size=Section1[i].size+Section2[j].size;  
                    HeaderFusion[i].sh_name=last_index_name;
                    buffsize+=Section2[j].size;        
                    SectionFusion[i].name=malloc(strlen(Section1[i].name));
                    strcpy(SectionFusion[i].name,Section1[i].name);
                    strcpy(&New_Names[last_index_name],Section1[i].name);
                    last_index_name+=strlen(Section1[i].name)+1;                    
                    SectionFusion[i].content=realloc(SectionFusion[i].content,Section1[i].size+Section2[j].size);
                    //concat
                    for(int k=0;k<Section2[j].size;k++)
                    {
                        SectionFusion[i].content[k+Section1[i].size]=Section2[j].content[k];
                    }
                    break;
                }
            }
            if(!b_exist)
            {
                //if there is no similar section
                SectionFusion[i].size=Section1[i].size;
                SectionFusion[i].content = CopyContent(Section1[i]);
                SectionFusion[i].name=malloc(strlen(Section1[i].name));
                HeaderFusion[i].sh_name=last_index_name;
                HeaderFusion[i].sh_size=Section1[i].size;
                strcpy(SectionFusion[i].name,Section1[i].name);
                strcpy(&New_Names[last_index_name],Section1[i].name);
                last_index_name+=strlen(Section1[i].name)+1;    
            }
        }
        else
        {
            //if it's not a PROGBIT section
            SectionFusion[i].size=Section1[i].size;
            SectionFusion[i].content = CopyContent(Section1[i]);
            SectionFusion[i].name=malloc(strlen(Section1[i].name));
            HeaderFusion[i].sh_size=Section1[i].size;
            HeaderFusion[i].sh_name=last_index_name;
            strcpy(SectionFusion[i].name,Section1[i].name);
            strcpy(&New_Names[last_index_name],Section1[i].name);
            last_index_name+=strlen(Section1[i].name)+1;    
        }        
        b_exist=0;
    }
    
    int j=0;
    for(int i=0;i<nbSection1;i++){
        if (SectionHeader1[i].sh_type==SHT_PROGBITS){
            for (j=0;j<nbSection2;j++){
                if (!strcmp(SectionFusion[i].name,Section2[j].name)){flag=0;break;}
                else{ flag=1;}
            }
            if (flag==1 && j != nbSection2){
                n_size++;
                memcpy(&HeaderFusion[n_size+nbSection1],&SectionHeader2[j],sizeof(Elf32_Shdr));
                if(HeaderFusion[n_size+nbSection1].sh_addralign>=2 && !HeaderFusion[n_size+nbSection1].sh_offset%HeaderFusion[n_size+nbSection1].sh_addralign)
                    buffsize+=(4-HeaderFusion[n_size+nbSection1].sh_offset%HeaderFusion[n_size+nbSection1].sh_addralign);
                HeaderFusion[n_size+nbSection1].sh_offset+=buffsize;
                HeaderFusion[i].sh_size=Section1[i].size;
                HeaderFusion[i].sh_name=last_index_name;
                SectionFusion[n_size+nbSection1].name=malloc(strlen(Section2[j].name));
                strcpy(SectionFusion[n_size+nbSection1].name,Section2[j].name);
                strcpy(&New_Names[last_index_name],Section1[i].name);
                last_index_name+=strlen(Section1[i].name)+1;    
                SectionFusion[n_size+nbSection1].content=CopyContent(Section2[j]);
                SectionFusion[n_size+nbSection1].size=Section2[j].size;
                buffsize+=Section2[j].size;
            }
        }
    }  
    SectionContent* temp =malloc(sizeof(SectionContent));
    New_Names = realloc(New_Names,last_index_name+1);
    temp->content=New_Names;
    temp->size=last_index_name;
    int SectionNamesIndex=GetHeader(*temp,HeaderFusion,".shstrtab",n_size+nbSection1);
    free(SectionFusion[SectionNamesIndex].content);
    SectionFusion[SectionNamesIndex].content=New_Names;
    SectionFusion[SectionNamesIndex].size=last_index_name;    
    HeaderFusion = realloc(HeaderFusion,sizeof(Elf32_Shdr)*(n_size+nbSection1));
    SectionFusion = realloc(SectionFusion,sizeof(SectionContent)*(n_size+nbSection1));
    FusionContent* FusionResult = malloc(sizeof(FusionContent));
    FusionResult->contents=SectionFusion;
    FusionResult->headers=HeaderFusion;
    FusionResult->nb=n_size+nbSection1;
    return FusionResult;
}