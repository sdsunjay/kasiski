#include "header.h"
#define COUNT_DEFAULT 3
#define LOWER_INT_DEF 48
#define HIGHER_INT_DEF 57
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
   
   /*argv counter*/
   int i;
   /*input file pointer*/
   FILE* fp;
   /*output file pointer*/
   FILE* fp1;
   /*initializations*/
   vflag=0;
   fp=NULL;
   fp1=NULL;
   mnum=COUNT_DEFAULT;
   /*skip 'ftable' */
   i=1;
   /*while last argument has not been parsed, parse*/
   while(argc>i)
   {
      if(strcmp(argv[i],"-m")==0)
      {
	 i++;
	 if(!(argv[i][0]>=LOWER_INT_DEF && argv[i][0]<=HIGHER_INT_DEF))
	 {
	    printf("usage: kasiski [ -v ] [ -m length ] [ infile [ outfile ] ]\n");
	    exit(1);
	 }
	 mnum= atoi(argv[i]);
	 if(vflag)
	    printf("Count only substrings of %d length\n",mnum);
      }
      else if(strcmp(argv[i],"-v")==0)
      {
	 vflag=1;
	 printf("Verbose mode: Engaged!\n");
      }
      else
      {
	 fp=fopen(argv[i],"r");
	 if(vflag)
	    printf("Reading from %s\n",argv[i]);
	 if(fp==NULL)
	 {
	    printf("usage: kasiski [ -v ] [ -m length ] [ infile [ outfile ] ]\n");
	    exit(1);
	 }
	 i++;
	 if(i!=argc)
	 {
	    fp1=fopen(argv[i],"w");
	    if(vflag)
	       printf("Outputting to %s\n",argv[i]);
	    if(fp1==NULL)
	    {
	       printf("usage: kasiski [ -v ] [ -m length ] [ infile [ outfile ] ]\n");
	       exit(1);
	    }

	 }
	 else if(vflag)
	    printf("Outputting to to stdout\n");
      }
      i++;
   }
   reader(fp,fp1);

} 

void reader(FILE* fp,FILE* fp1)
{
   int i;
   int charsRead;
   int size;
   char* buffer;
   char line[300];
   int result;
   charsRead=0;
   buffer=NULL;
   size=500;
   /*read from stdin*/
   if(fp==NULL)
   {
      while(fgets(line,300,stdin)!=NULL)
      {   
	 /*realloc stuff*/
	 charsRead+=301;
	 if(!buffer)
	 {   
	    buffer=(char*) realloc(buffer,(sizeof(char)*size));
	 }   
	 if(size<charsRead)
	 {   
	    //printf("\n(Realloc)\n");
	    size=size*2;;
	    buffer=(char*) realloc(buffer,(sizeof(char)*(size)));

	 }   
	 strcat(buffer, line);

      }
      size=strlen(buffer);
   } 
   /*read from file*/
   else
   {
      /*obtain file size:*/
      fseek (fp , 0 , SEEK_END);
      size = ftell(fp);
      rewind (fp);

      /*allocate memory for the whole file*/
      buffer = (char*) calloc (size,sizeof(char));
      if(buffer==NULL)
      {
	 perror("Buffer is NULL\n");
	 exit (1);
      }
      /*copy the file into the buffer*/
      result = fread (buffer,1,size,fp);
      if (result != size) 
      {
	 fprintf (stderr,"Reading error\n"); 
	 exit (1);
      }
      fclose (fp);
   }
   size=cleanBuffer(buffer,size);
   if(vflag)
      printf("new buffer %s\n",buffer);
   
   createHashTable();
   printf("SIZE: %d\n",size);
   countSubstrings(buffer,size);
      //printHTable();
   free(buffer);
   output(fp1);
   

}

int cleanBuffer(char* buffer, int size)
{
   int i;
   int j;
   int t;
   t=0;
   for(i=0;i<size;i++)
   {
      if(!isalpha(buffer[i]))
      {
	 for(j=i;j<size-1;j++)
	 {
	    buffer[j]=buffer[j+1];
	 }
	 buffer[j]='\0';
	 t++;
      }

      if(buffer[i]>=97 && buffer[i]<=122)
      {
	 buffer[i]-=32;
      } 
   }
   return size-t;
}

void countSubstrings(char* buffer, int size)
{


   /*loop counters*/
   int i;
   int j;
   int k;

   /*hold various substrings*/
   char* sub1;
   char* sub2;

   /*run along buffer*/
   char* sub1run;
   char* runner;
   char* c;

   int msize;
   int locsize;
   msize=5000;
   total=0;
   mindex=0;

   sub1 = (char*) calloc (size,sizeof(char));
   sub2 = (char*) calloc (size,sizeof(char));
   mword= (matchingWord*) calloc (msize,sizeof(matchingWord));

   mword[mindex].count=0;
   size++;
   if(vflag)
      printf("total size %d",size);

   /*all length from 3 to size/2*/
   for(i=mnum;i<=(size/2)+1;i++)
   {
      //emptyHTable();
      //printf("%d\n",i);
      //printf("buffer: %s\n",buffer);
      sub1run=&buffer[0];
      //printf("%d substring: %s\n\n",i,sub1);

      /*all substrings of size i*/
      for(k=0;k<=size-i-i;k++)
      {
	 sub1=memset(sub1, '\0', i);
	 
	 strncpy(sub1,sub1run,i);
	sub1[strlen(sub1)]='\0'; 
	 runner=sub1run+i;	

	 if(vflag)
	    printf("SUB1  %s\n",sub1);
	 //printf("%s\n",sub1);
	 //printf("FAIL: COunt: %d  Length: %d Loc: %d\n",mword[mindex].count,mword[mindex].length,mword[mindex].loc[mword[mindex].count]);
	 //printf("sub1run: %s\n",sub1run);
	 locsize=50;
	 mword[mindex].loc= (int*) calloc (locsize,sizeof(int));
	 mword[mindex].apart= (int*) calloc (locsize,sizeof(int));
	 mword[mindex].count=0;
	 //printf("Mindex: %d\nK: %d\n",mindex,k);
	 mword[mindex].loc[mword[mindex].count]=k;

	 //printf("RUNNER  %s\n",runner);
	 sub2=memset(sub2, '\0', i+1);
	 strncpy(sub2,runner,i);
	 /*fills sub2 with all substrings of length i 
	  *  which are to the right of sub1 that we have not seen before*/
	 /*if we have not seen this substring before*/ 
	 if(check(hash(sub1),sub1))
	    while((c=strstr(runner,sub1))!=NULL) 
	    {
	       sub2=memset(sub2, '\0', i);
	       //printf("SUB2\t%s\n",sub2);
	       strncpy(sub2,c,i);
	       runner=c;
	       /*checks to see if they match*/
	       if(strcmp(sub1,sub2)==0)
	       {
		  total++;
		  mword[mindex].count++;
		  if(mword[mindex].count>=locsize-1)
		  {
		     //printf("Count: %d\nlocsize: %d\n",mword[mindex].count,locsize);
		     locsize=locsize*2;
		     mword[mindex].loc=(int*)realloc(mword[mindex].loc,(sizeof(int)*(locsize)));
		     mword[mindex].apart=(int*)realloc(mword[mindex].apart,(sizeof(int)*(locsize)));
		  }
		  //check memory locations and see the difference, then add difference to initial location
		  mword[mindex].loc[mword[mindex].count]=(c-sub1run)+mword[mindex].loc[mword[mindex].count-1];
		  //printf("location is %d\nCount is %d\n",c-sub1run,mword[mindex].count);
		  //if(mword[mindex].count==1)
		  // printf("First location is  %d \n",mword[mindex].loc[0]);
		  //printf("FAIL1: COunt: %d  Length: %d Loc: %d\n",mword[mindex].count,mword[mindex].length,mword[mindex].loc[mword[mindex].count]);
		  //printf("location is  %d \n",mword[mindex].loc[mword[mindex].count]);
	       }
	       runner++;
	    }
	 if(mword[mindex].count>0)
	 {
	    //printf("COUNT %d\n\n",mword[mindex].count);
	    mword[mindex].length=i;      
	    spaceApart();

	    /*add newly found word to hashtable*/
	    addhash(hash(sub1),sub1);
	    //if(vflag)
	    // printf("%s\n",mword[mindex].str);
	    mindex++;
	 }

	 sub1run++;
	 if(mindex>=msize)
	 {
	    //printf("realloc\n");
	    msize*=2;
	    mword=(matchingWord*) realloc(mword,(sizeof(matchingWord)*(msize)));

	 }
      }
      if(total==0)
      {
	 printf("no more words on size %d and greater to be found\n",i);
	 //break;
      }  
      else
      {
	 total=0;
      }
   }
   free(sub1);
   free(sub2);
}

void createHashTable()
{
   sizeHTable=ISIZE;
   int i;
   htable=(char***)calloc(sizeHTable,sizeof(char**));
   for(i=0;i<sizeHTable;i++)
   {
      // printf("%d innitialized\n",i);
      htable[i]=(char**)calloc(sizeHTable,sizeof(char*));

   }
}

/**return if it is in there or not*/
int hash(char* str)
{
   unsigned long hash =115391;
   int c;
   char* t;
   t=str+2;
   while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   while (c = *t++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   //printf("HASH: %d\n",hash%1000);
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
   if(htable[hash]!=NULL)
   {
      if(htable[hash][hash]!=NULL)
      {
	 while(((hash+i)<=sizeHTable) && htable[hash][hash+i]!=NULL)
	 {
	    if(htable[hash]==NULL)
	       break;
	    else if(htable[hash][hash+i]==NULL)
	       break;
	    else if(strcmp(htable[hash][hash+i],str)==0)
	    {
	       //printf("%s not allowed in htable, already there\n",str);
	       return 0;
	    }
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
   void* temp;
   coll=0;
   printf("ADDING %s to table\n",str);
   if(htable[hash]==NULL)
   {
      printf("NULL at %d\n\n",hash);
      exit(1);
   }
   if(htable[hash][hash]==NULL)
   {
      //printf("%s is in currently\n",htable[hash][hash+coll]);
      //printf("%d is strlen of %s\n",strlen(str),str);
      htable[hash][hash] =(char*)realloc(htable[hash][hash],strlen(str)*sizeof(char));
      htable[hash][hash]=memset(htable[hash][hash], '\0', strlen(str));
      strcpy(htable[hash][hash],str);
   }
   else
   {
      coll=1;
      while(htable[hash][hash+coll] !=NULL)
      {
	 if(hash+coll>=sizeHTable)
	 {
	    for(i=0;i<1000;i++)
	    {
	       printf("realloced to size %d\n",sizeHTable*2);
	       temp=realloc(htable[i],(sizeof(char*)*(sizeHTable*2)));
	       if(temp!=NULL)
		  htable[i]=(char**) temp;
	       else
	       {
		  //printf("\n\nNOT ENOUGH MEM\n");
		  exit(EXIT_FAILURE);
	       }

	       for(j=sizeHTable;j<sizeHTable*2;j++)
	       {
		  htable[i][j]=NULL; 
	       }
	    }

	    sizeHTable*=2;
	 }
	 coll++;
      }

      temp =realloc(htable[hash][hash+coll],strlen(str)*sizeof(char));
      if(temp!=NULL)
	 htable[hash][hash+coll]=(char*) temp;
      else
      {
	 //printf("\n\nNOT ENOUGH MEM\n");
	 exit(EXIT_FAILURE);
      }
      //printf("ADDING %s (strlen %d) to table at %d\n",str,(int) strlen(str),hash);
      htable[hash][hash+coll]=memset(htable[hash][hash+coll], '\0', strlen(str));
      //printf("%s is in currently\n",htable[hash][hash+coll]);
      strcpy(htable[hash][hash+coll],str);
   }
   mword[mindex].index1=hash;
   mword[mindex].index2=hash+coll;
}
void printHTable()
{

   int i;
   int j;
   printf("************************************************\nprinting table\n********************************************\n");
   for(i=0;i<sizeHTable;i++){
      for(j=0;j<sizeHTable;j++)
      {
	 if(htable[i]!=NULL){
	    if(htable[i][j]!=NULL)
	    {
	       printf("%d %d. %s\n",i,j,htable[i][j]);
	    }
	 }
      }
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
	    }
   printf("Table emptied\n");
}


void output(FILE* fp1)
{

   insertAlpha(mindex);
   insertCount(mindex);
   insertLength(mindex);
   /*loop through all letters, A-Z*/
   int i;
   /*loop though all asterixs*/
   int j;
   /*temporarily stores final percentage of a given letter*/
   double temp;
   fprintf(stderr,"SKIPME This is a naive implementation of skipping overlapping\n");
   fprintf(stderr,"SKIPME strings.  It misses the problem of the skipped string\n");
   fprintf(stderr,"SKIPME possibly matching somewhere down the road.\n");

   if(fp1==NULL) 
      printf("Length Count Word    Location (distance)\n");
   else
      fprintf(fp1,"Length Count Word     Location (distance)\n");
   if(fp1==NULL)
   {
      for(i=0;i<mindex;i++)
      {
	 if(mword[i].count>0)
	 {
	    /*count is incremented for nico output as he counts original find*/
	    mword[i].count++;
	    printf("%6d%6d %*s   %-d",mword[i].length, mword[i].count,mword[0].length,htable[mword[i].index1][mword[i].index2],mword[i].loc[0]);
	    for(j=1;j<mword[i].count;j++)
	    {
	     printf(" %d",mword[i].loc[j]);
	     printf(" (%d)",mword[i].apart[j]);
	    }
	    //free(mword[i].apart);
	    //problem here
	    //free(mword[i].loc);
	    //free(mword[i].str);
	    printf("\n");
	 }

	 //free(mword[i]);
      }
      //free(mword);
   }
   else
   {
      for(i=0;i<mindex;i++)
      {
	 if(mword[i].count>0)
	 {
	    /*count is incremented for nico output as he counts original find*/
	    mword[i].count++;
	    //printf("%d. Count: %d Length: %d Word: %s\n",i,mword[i].count,mword[i].length,htable[mword[i].index1][mword[i].index2]);
	    fprintf(fp1,"%6d%6d %*s   %-d",mword[i].length, mword[i].count,mword[0].length,htable[mword[i].index1][mword[i].index2],mword[i].loc[0]);

	 }
      }
   }
}

int compare(int a, int b)
{
   if(a>b)
      return -1;
   else if(a<b)
      return 1;
   else
      return 1;
}


void insertLength(int size)
{
   int i;
   int j;
   matchingWord x;
   for(i=0;i<=size;i++)
   {
      x=mword[i];
      for(j=i;j>0&&compare(x.length,mword[j-1].length)<0;j--)
      {
	 mword[j]=mword[j-1];
      }
      mword[j]=x;
   }   
}

void insertCount(int size)
{
   int i;
   int j;
   matchingWord x;
   for(i=0;i<=size;i++)
   {
      x=mword[i];
      for(j=i;j>0&&compare(x.count,mword[j-1].count)<0;j--)
      {
	 mword[j]=mword[j-1];
      }
      mword[j]=x;
   }   
}
/*find the space between two originating strings*/
void spaceApart()
{
   //printf("mword[mindex].count in space apart is %d\n",mword[mindex].count);
   int num,num1,i;
   int count;
   count=mword[mindex].count;
   for(i=1;i<=count;i++)
   {
      num=mword[mindex].loc[i];
      num1=mword[mindex].loc[i-1];

      mword[mindex].apart[i]=num-num1; 
      //printf("location1: %d location2: %d apart: %d\n",num1,num,mword[mindex].apart[i]);
      if(mword[mindex].apart[i]<mword[mindex].length)
	 mword[mindex].apart[i]=-1;
   }
   //printf("mword[mindex].length %d\n,mword[mindex].length);
   //\tmword[mindex].count: %d\n\n,mword[mindex].
   //length,mword[mindex].count);
   // printf("mword[%d].loc %d 
}

void insertAlpha(int size)
{
   int i, didSwap = 1, limit = size-1;
   matchingWord temp;
   while (didSwap) {
      didSwap = 0;
      for (i = 0; i < limit; i++) {
	 if (strcmp (  htable[mword[i].index1][mword[i].index2], htable[mword[i+1].index1][mword[i+1].index2]) > 0) {
	    temp          = mword[i];
	    mword[i]   = mword[i+1];
	    mword[i+1] = temp;
	    didSwap = 1;
	 }
      }
      limit--;
   }
}



