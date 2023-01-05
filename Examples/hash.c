#include "hash.h"

int hash(char *m) 
{
  int l = strlen(m);
  int i, h;
  h = 0;
  for(i=0; i<l; i++)
    h = h + (m[i]*pow(127, l-i-1));
  h = h % MaxSizeArray;
  return h;
}