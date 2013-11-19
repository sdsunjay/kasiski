#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ISIZE 1000
int check(int hash,char* str);
void addhash(int hash,char* str);
void createHashTable();
int hash(char* str);
void printHTable();
void emptyHTable();


char*** htable;
int sizeHTable;
int main(int argc, char* argv[])
{   
   
   char* str;
   int i;
   int added=0;
   int notadded=0;
   str=(char*)malloc(sizeof(char)*10000);
   strcpy(str,"HEEEEEEEEEEEEEEEEEELLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
   createHashTable();
   for(i=0;i<strlen(str);i++)
   {
      if(check(hash(str),str))
      {
	 addhash(hash(str),str);
	 added++;
	 if(added>1000)
	 {
	    printf("%d objects added\n",added);
	    printf("%d objects not added\n",notadded);
	    printHTable();
	    added=0;
	    notadded=0;
	    emptyHTable();
	    printf("Table emptied\n");
      
	 }
      }
      else
      {
	 printf("not adding %s!\nIt is already in Htable\n",str);
	 notadded++;
      }
      str++;
   }
   return 0;
}

/**return if it is in there or not*/
int hash(char* str)
{
   unsigned long hash =115391;
   int c;
   while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   return hash%1000;

}
int check(int hash,char* str)
{
   int i;
   i=0;
   if(str==NULL)
   {
      printf("STR IS NULL\n");
      exit(0);
   }
   printf("initial search began at %d index\n",hash);
   printf("size of htable is %d\n",sizeHTable);
   if(htable[hash]!=NULL)
   {
      if(htable[hash][hash]!=NULL)
      {
	 while(((hash+i)<=sizeHTable) && htable[hash][hash+i]!=NULL)
	 {
	    //printf("searching index: %d of table at index %d\n",hash+i,hash);
	    if(htable[hash]==NULL)
	       break;
	    if(htable[hash][hash+i]==NULL)
	       break;
	    if(strcmp(htable[hash][hash+i],str)==0)
	       return 0;
	    i++;
	 }
      }
   }

   return 1;
}
void addhash(int hash,char* str)
{
   int coll;
   int i,j;
   int tempsize;
   coll=1;
   if(htable[hash]==NULL)
   {
      printf("NULL at %d\n\n",hash);
      exit(1);
   }
   else if(htable[hash][hash]==NULL)
   {
      htable[hash][hash]=(char*)realloc(htable[hash][hash],strlen(str)*sizeof(char));
      strcpy(htable[hash][hash],str);
   }
   else
   {
      while(htable[hash][hash+coll] !=NULL)
      {
	 if(hash+coll>=sizeHTable)
	 {
	    for(i=0;i<1000;i++)
	    {
	       printf("%d realloced to size %d\n",i,sizeHTable*2);
	       htable[i]=(char**) realloc(htable[i],(sizeof(char*)*(sizeHTable*2)));
	       for(j=sizeHTable;j<sizeHTable*2;j++)
	       {
		  htable[i][j]=NULL; 
		  //printf("for array of strings at %d, string at %d is %s\n",i,j,htable[i][j]);
	       }
	    }

	    sizeHTable*=2;
	 }
	 coll++;
      }
      htable[hash][hash+coll]=(char*)realloc(htable[hash][hash+coll],strlen(str)*sizeof(char));
      strcpy(htable[hash][hash+coll],str);
   }

}
void printHTable()
{
   int i;
   int j;
   for(i=0;i<sizeHTable;i++){
      for(j=0;j<sizeHTable;j++)
      {
	 if(htable[i]!=NULL){
	    if(htable[i][j]!=NULL)
	    {
	       printf("%d %d. %s\n",i,j,htable[i][j]);
	    }
	    free(htable[i][j]);
	 }
      }
      free(htable[i]);
   }
}
void emptyHTable()
{
   int i,j;
   for(i=0;i<sizeHTable;i++)
      for(j=0;j<sizeHTable;j++)
	 if(htable[i]!=NULL)
	    if(htable[i][j]!=NULL)
	    {
	       htable[i][j]=NULL;
	       //free(htable[i][j]);

	    }
}
