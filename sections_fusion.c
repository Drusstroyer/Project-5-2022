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
    FusionSectionProgBits(section_header1,SectionFile1,section_header2,SectionFile2,header1.e_shnum,header2.e_shnum);
    //Elf32_Ehdr new_header;

    //new_header.e_shnum=ProgBits.nb;
    //new_header.e_shstrndx=GetHeader(*SectionName,section_header,".symtab",NewContents.nb); //it will not change

    //display_sections_header(&new_header,ProgBits.headers,ProgBits.contents[new_header.e_shstrndx]);
    printf("\nFusion Success\n");
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
                if(!HeaderFusion[n_size+nbSection1].sh_offset%HeaderFusion[n_size+nbSection1].sh_addralign)
                    buffsize+=(4-HeaderFusion[n_size+nbSection1].sh_offset%HeaderFusion[n_size+nbSection1].sh_addralign);
                HeaderFusion[n_size+nbSection1].sh_offset+=buffsize;
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
    New_Names = realloc(New_Names,last_index_name);
    SectionContent temp;
    temp.content=New_Names;
    temp.size=last_index_name;
    int SectionNamesIndex=GetHeader(temp,HeaderFusion,".shstrtab",n_size+nbSection1);
    free(SectionFusion[SectionNamesIndex].content);
    SectionFusion[SectionNamesIndex].content=temp.content;
    SectionFusion[SectionNamesIndex].size=last_index_name;    
    SectionFusion = realloc(SectionFusion,n_size+nbSection1);
    HeaderFusion = realloc(HeaderFusion,n_size+nbSection1);
    FusionContent* FusionResult = malloc(sizeof(FusionContent));
    FusionResult->contents=SectionFusion;
    FusionResult->headers=HeaderFusion;
    FusionResult->nb=n_size+nbSection1;
    return FusionResult;
}