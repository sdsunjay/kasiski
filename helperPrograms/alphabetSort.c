#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int compareStrings(const void *a, const void *b);
void sort(char** names, size_t size);

int main(void)
{
   char* names[]={"sunjay", "albert", "hicker", "dober", "all", "allcat",
"monkey","daveed","sunjae","sanjay","davidw","dickee","dougee","davidq"};
   sort(names, 14);

}
void alphasort(int size)
{
   /*loop counters*/
   int i;
   void* base=mword;
   printf("Unsorted\n");
   for(i=0;i<size;i++)
      printf("Name: %s\n",names[i]);

   qsort(base, size, sizeof(mword)/sizeof(matchingWord),compareStrings);
   printf("Sorted\n");
   for(i=0;i<size;i++)
      printf("Name: %s\n",names[i]);

}
int compareStrings(const void *a, const void *b)
{  
   
    matchingWord *ia = (matchingWord*)a;
    matchingWord  *ib = (matchingWord*)b;
   return strcmp(htable[ia.index1][ia.index2], htable[ib.index1][ib.index2]);

}
