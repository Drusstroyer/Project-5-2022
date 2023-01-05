#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

//#include "json.h"

#define TEXTE "./Exemples/hugo.txt"
#define DICORES "./Exemples/dictionnaire.txt"

#define MaxSizeArray 16381
#include "globalvariable.h"
#include <dicomanage.h>
#include <wordmanage.h>


/*
struct json_object *createJSON(mot_data_t **d){
  int i;
  json_object *r = json_object_new_object();
  json_object *t = json_object_new_array_ext(MaxSizeArray);
  for(i=0; i<MaxSizeArray; i++) {
    if (d[i] != NULL) {
      int j=0;
      struct json_object *newelem = json_object_new_object();
      char* mot = d[i]->lemot;   
      emplacement_t* list = d[i]->tete_liste;
      json_object *l = json_object_new_array();
      json_object_object_add(newelem, "index", json_object_new_int(i));
      json_object_object_add(newelem, "mot", json_object_new_string(mot));
      while(list != NULL) {
	struct json_object *paire = json_object_new_object();
	json_object_object_add(paire,"ligne",json_object_new_int(list->line));
	json_object_object_add(paire,"colonne",json_object_new_int(list->colonne));
	json_object_array_put_idx(l,j,paire);
	j++;
	list = list->next;
      }
      json_object_object_add(newelem, "liste", l);
      json_object_array_add(t,newelem); 
    }
  }
  json_object_object_add(r,"dico",t);
  return r;
}
*/
extern char *separators; 
extern unsigned int current_line;
extern unsigned int current_col; 

int main()
{
  separators = SEP; 
  current_line=1;
  current_col=1; 
  FILE* f = NULL;
  int i;
  f = fopen(TEXTE, "r");
  if(f== NULL)
  {
    printf("fichier vide");
    return 0;
  }

  unsigned int* line = (unsigned int*) malloc(sizeof(int));
  unsigned int* colonne = (unsigned int*) malloc(sizeof(int));
  char* word = (char*) malloc(sizeof(char)*maxSizeWord);
  dico* dictionary = (dico*) malloc(sizeof(dico));   
  dico* copiedico = NULL;     
  mot_data_t **serialized_dico = (mot_data_t **)malloc(MaxSizeArray*sizeof(mot_data_t *));
  
  while(!feof(f)) {
    word = next_word(f,line,colonne);
    addToDico(&dictionary,word,line,colonne);
  }
  displayDico(f,dictionary,TEXTE);

  for(i=0; i<MaxSizeArray; i++)
    serialized_dico[i] = NULL;
  serializeDico(dictionary,serialized_dico);
  for(i=0; i<MaxSizeArray; i++)
    if (serialized_dico[i] != NULL) {
      printf("Index %d -> ", i);
      displayWord(serialized_dico[i],stdout);
      //printf("\n");
    }
  /*
  struct json_object *o;
  o = createJSON(serialized_dico);
  FILE *fic = fopen("monfic.json","w");
  fprintf(fic, "%s\n",
	  json_object_to_json_string_ext(o,JSON_C_TO_STRING_PRETTY));
  fclose(fic);
  */
  for(i=0; i<MaxSizeArray; i++)
    if (serialized_dico[i] != NULL) {
      deserializeDico(&copiedico, serialized_dico[i]);
    }
  printf("A la fin : \n");
  displayNodes(copiedico, stdout);
  fclose(f);
  return 0;
}

