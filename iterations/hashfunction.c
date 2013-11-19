#include "header.h"
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
   return 0;
} 

void reader(FILE* fp,FILE* fp1)
{
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
	 charsRead+=300;
	 if(!buffer)
	 {   
	    buffer=(char*) realloc(buffer,(sizeof(char)*size));
	 }   
	 if(size<charsRead)
	 {   
	    /*realloc buffer user space
	     * The user must be typing a lot*/
	    size=size*2;;
	    buffer=(char*) realloc(buffer,(sizeof(char)*(size)));

	 }   
	 strcat(buffer, line);

      }
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
   filter_alpha(buffer);
   size=strlen(buffer);
   if(vflag)
      printf("new buffer %s\n",buffer);
   
   createHashTable();
   if(vflag)   
      printf("SIZE: %d\n",size);
   countSubstrings(buffer,size);
      //if(vflag)
	 printHTable();
   free(buffer);
   output(fp1);
   

}

void filter_alpha(char *s) {
   for (char *p = s; *p; ++p)
      if (isalpha(*p))
	 *s++ = *p;
   *s = '\0';
}

void countSubstrings(char* buffer, int size)
{


   /*loop counters*/
   int i;
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
   int tempC;

   msize=2000;
   total=0;
   mindex=0;
   tempC=0;

   sub1 = (char*) calloc (size,sizeof(char));
   sub2 = (char*) calloc (size,sizeof(char));
   mword= (matchingWord*) calloc (msize,sizeof(matchingWord));

   mword[mindex].count=0;
   if(vflag)
      printf("total size %d",size);
   if(vflag)
      printf("READ: %s\n",buffer);
   /*all length from 3 to size/2*/
   for(i=mnum;i<ceil((size/2));i++)
   {
      if(vflag)
	 printf("Looking for substrings of size %d\n",i);
      sub1run=&buffer[0];
      //if(strlen(sub1)==13)
      // printf("Looking for %s\n",sub1);

      /*all substrings of size i*/
      for(k=0;k<=size-i-i;k++)
      {
	 sub1=memset(sub1, '\0', i);

	 strncpy(sub1,sub1run,i);
	 sub1[i]='\0'; 
	 //if(strlen(sub1)>=mnum)
	 if(check(hash(sub1),sub1,i))
	 {
	    runner=sub1run+i;	

	    // if(vflag)
	    //  printf("SUB1  %s\n",sub1);
	    locsize=50;
	    mword[mindex].loc= (int*) calloc (locsize,sizeof(int));
	    mword[mindex].apart= (int*) calloc (locsize,sizeof(int));
	    mword[mindex].count=0;
	    /* if(vflag)
	       {
	       printf("Number of distinct words found: %d\n",mindex);
	    //printf("We have moved %d along\n",k);
	    }*/
	    mword[mindex].loc[mword[mindex].count]=k;

	    sub2=memset(sub2, '\0', i);
	    strncpy(sub2,runner,i);
	    sub2[i]='\0';
	    /*fills sub2 with all substrings of length i 
	     *  which are to the right of sub1 that we have not seen before*/
	    /*if we have not seen this substring before*/ 
	    while((c=strstr(runner,sub1))!=NULL) 
	    {
	       sub2=memset(sub2, '\0', i);
	       //printf("SUB2\t%s\n",sub2);
	       strncpy(sub2,c,i);
	       runner=c;
	       /*checks to see if they match*/
	       if(strcmp(sub1,sub2)==0)
	       {
		  //printf("%s FOUND\n",sub1);
		  total++;
		  mword[mindex].count++;
		  tempC=mword[mindex].count;
		  /* if(mword[mindex].count>=locsize-1)
		     {
		     if(vflag)
		     printf("realloc number of locations to %d\n",locsize);
		     locsize=locsize*2;
		     mword[mindex].loc=(int*)realloc
		     (mword[mindex].loc,(sizeof(int)*(locsize)));
		     mword[mindex].apart=(int*)realloc
		     (mword[mindex].apart,(sizeof(int)*(locsize)));
		     }*/
		  /*check locations and see the difference
		   * then add difference to initial location*/
		  mword[mindex].loc[tempC]=(c-sub1run)+k;
		  mword[mindex].apart[tempC]=(mword[mindex].loc[tempC] -mword[mindex].loc[tempC-1]);
		  //=c-mword[mindex].loc[tempC];
		  if(vflag)
		  {
		     if(mword[mindex].count==1)
			printf("First location is  %d \n",mword[mindex].loc[0]);
		  }
	       }
	       runner++;
	    }
	 }
	 if(mword[mindex].count>0)
	 {
	    //printf("COUNT %d\n\n",mword[mindex].count);
	    mword[mindex].length=i;      
	    /*add newly found word to hashtable*/
	    addhash(hash(sub1),sub1,i);
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
	 //printf("no more words on size %d and greater to be found\n",i);
	 break;

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
   int i;
   sizeHTable=ISIZE;
   htable=(char***)calloc(sizeHTable,sizeof(char**));
   for(i=0;i<sizeHTable;i++)
   {
      htable[i]=(char**)calloc(sizeHTable,sizeof(char*));

   }
}

uint32_t hash(char* str)
{
   uint32_t hash=0;
   size_t i;
   int len;
   len=strlen(str);
   for(i=0;i<len;++i)
   {
      //printf("%d\n",i);
      hash += str[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
   }
   //printf("hash return %d",hash%1024);
   hash += (hash << 3);
   hash ^= (hash >> 11);
   hash += (hash << 15);
   return  hash%1024;

}
int check(uint32_t hash,char* str, int len)
{
   int i;
   i=0;
   if(str==NULL)
   {
      if(vflag)
	 printf("checking null string!\n");
      return 0;
   }
   if(htable[hash]!=NULL)
   {
      if(htable[hash][hash]!=NULL)
      {
	 while( htable[hash][hash+i]!=NULL)
	 {
	    if(strncmp(htable[hash][hash+i],str,len)==0)
	    {
	       return 0;
	    }
	    i++;
	 }
      }
   }

   return 1;
}
void addhash(uint32_t hash,char* str, int len)
{
   int coll;
   int i,j;
   void* temp;
   coll=0;
   if(htable[hash]==NULL)
   {
      printf("NULL at %d\n\n",hash);
      exit(1);
   }
   if(htable[hash][hash]==NULL)
   {
      //printf("%s is in currently\n",htable[hash][hash+coll]);
      printf("String being added %s\n",str);
      htable[hash][hash] =(char*)realloc(htable[hash][hash],len*sizeof(char));
      htable[hash][hash]=memset(htable[hash][hash], '\0', len);
      strcpy(htable[hash][hash],str);
      if(len==12)
      {
      printf("ADDED %s (strlen %d) ",htable[hash][hash],(int) strlen(str));
      printf("to table at %d\n",hash+coll);
   
      }
   }
   else
   {
      coll=1;
      while(htable[hash][hash+coll] !=NULL)
      {
	 if(hash+coll>=sizeHTable)
	 {
	    for(i=0;i<1024;i++)
	    {
	       printf("realloced to size %d\n",sizeHTable*2);
	       temp=realloc(htable[i],(sizeof(char*)*(sizeHTable*2)));
	       if(temp!=NULL)
	       {
		  htable[i]=(char**) temp;
		  for(j=sizeHTable-1;j<sizeHTable*2;j++)
		  {
		     htable[i][j]=memset(htable[i][j], '\0', sizeHTable*2);
		     htable[i][j]=NULL; 
		  }
	       }
	       else
	       {
		  //printf("\n\nNOT ENOUGH MEM\n");
		  exit(EXIT_FAILURE);
	       }
	    }

	    sizeHTable*=2;
	 }
	 coll++;
      }
      htable[hash][hash+coll] =realloc(htable[hash][hash+coll],strlen(str)*sizeof(char));
      htable[hash][hash+coll]=memset(htable[hash][hash+coll], '\0', strlen(str));
      printf("111String being added %s\n",str);
      strncpy(htable[hash][hash+coll],str,len);
      htable[hash][hash+coll][len]='\0';
      //printf("ADDED %s (strlen %d) ",htable[hash][hash],(int) strlen(str));
      //printf("to table at %d\n",hash+coll);
   }
   mword[mindex].index1=hash;
   mword[mindex].index2=hash+coll;
}
void printHTable()
{

   int i;
   int j;
   printf("*****\nprinting table\n*****\n");
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
   /*loop through all letters, A-Z*/
   int i;
   /*loop though all asterixs*/
   int j;

   char* temp;
   temp=NULL;
   insertAlpha(mindex);
   insertCount(mindex);
   insertLength(mindex);
   fprintf(stderr,"SKIPME This is a naive implementation of skipping overlapping\n");
   fprintf(stderr,"SKIPME strings.  It misses the problem of the skipped string\n");
   fprintf(stderr,"SKIPME possibly matching somewhere down the road.\n");

   if(fp1==NULL) 
   {
      printf("Length   Count Word    Location (distance)\n");
      printf("======   ===== ====    ========\n");
   }
   else
   {
      fprintf(fp1,"Length   Count Word     Location (distance)\n");
      fprintf(fp1,"======   ===== ====    ========\n");
   }
   if(fp1==NULL)
   {
      for(i=0;i<mindex;i++)
      {
	 if(mword[i].count>0)
	 {
	    temp=htable[mword[i].index1][mword[i].index2];
	    /*count is incremented for nico output as he counts original find*/
	    mword[i].count++;
	    printf("%6d%7d ",mword[i].length, mword[i].count);
	    printf("%*s    ",mword[0].length+1,temp);
	    printf("%-d",mword[i].loc[0]);
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
	    temp=htable[mword[i].index1][mword[i].index2];
	    /*count is incremented for nico output as he counts original find*/
	    mword[i].count++;
	    fprintf(fp1,"%6d%8d ",mword[i].length, mword[i].count);
	    fprintf(fp1,"%*s    %-d",mword[0].length+1,temp,mword[i].loc[0]);

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

void insertAlpha(int size)
{
   char* tempstr;
   int i, didSwap = 1, limit = size-1;
   matchingWord temp;
   while (didSwap) {
      didSwap = 0;
      for (i = 0; i < limit; i++) {
	 tempstr=htable[mword[i].index1][mword[i].index2];
	 if (strcmp (tempstr, htable[mword[i+1].index1][mword[i+1].index2]) > 0) 
	 {
	    temp          = mword[i];
	    mword[i]   = mword[i+1];
	    mword[i+1] = temp;
	    didSwap = 1;
	 }
      }
      limit--;
   }
}



