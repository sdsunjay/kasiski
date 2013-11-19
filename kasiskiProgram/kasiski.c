#include "kasiskiHeader.h"
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
   /*set mnum to the default length of 3*/
   mnum=COUNT_DEFAULT;
   /*skip 'ftable' */
   i=1;
   /*while last argument has not been parsed, parse*/
   while(argc>i)
   {
      /*if '-m' flag specified by user*/ 
      if(strcmp(argv[i],"-m")==0)
      {
	 i++;
	 if(!(argv[i][0]>=LOWER_INT_DEF && argv[i][0]<=HIGHER_INT_DEF))
	 {
	       printf("usage: kasiski [ -v ] [ -m length ]");
	       printf(" [ infile [ outfile ] ]\n");
	    exit(1);
	 }
	 /*no longer starting with substrings of 3 length*/
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
	 /*read from file*/
	 fp=fopen(argv[i],"r");
	 if(vflag)
	    printf("Reading from %s\n",argv[i]);
	 if(fp==NULL)
	 {
	       printf("usage: kasiski [ -v ] [ -m length ]");
	       printf(" [ infile [ outfile ] ]\n");
	    exit(1);
	 }
	 i++;
	 if(i!=argc)
	 {
	    /*set fp1 to output file*/
	    fp1=fopen(argv[i],"w");
	    if(vflag)
	       printf("Outputting to %s\n",argv[i]);
	    /*file use specified to write to is null
	     * fail gracefully*/
	    if(fp1==NULL)
	    {
	       printf("usage: kasiski [ -v ] [ -m length ]");
	       printf(" [ infile [ outfile ] ]\n");
	       exit(1);
	    }

	 }
	 else if(vflag)
	    printf("Outputting to to stdout\n");
      }
      i++;
   }
   /*parse input from file or stdin*/
   reader(fp,fp1);
   return 0;
} 
/*parse and store file @param fp or stdin
 * write to stdout or @param fp2*/ 
void reader(FILE* fp,FILE* fp1)
{
   /*Declarations*/
   int i;
   int charsRead;
   int size;
   /*this is where the file or stdin is stored*/
   char* buffer;
   /*temp variable to read 300 characters into stdin*/
   char line[READ_300];
   int result;

   charsRead=0;
   buffer=NULL;
   size=INITIAL_STRING_SIZE;
   /*read from stdin*/
   if(fp==NULL)
   {
      while(fgets(line,READ_300,stdin)!=NULL)
      {   
	 /*realloc stuff*/
	 charsRead+=READ_300;
	 if(!buffer)
	 {   
	    buffer=(char*) realloc(buffer,(sizeof(char)*size));
	 }   
	 if(size<charsRead)
	 {   
	    /*realloc buffer user space
	     * The user must be typing a lot*/
	    size=(size+size);;
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
   /*filter out junk characters from string*/
   filter_alpha(buffer);
   size=strlen(buffer);
   /*lowercase to uppercase*/
   for(i=0;i<size;i++)
   {
      if(buffer[i]>=LOWER_LCASE_DEF && buffer[i]<=UPPER_LCASE_DEF)
	 buffer[i]-=UPPER_TO_LOWER;
   }
   if(vflag)
      printf("new buffer %s\n",buffer);
   
   /*create and initialize hash table*/
   createHashTable();
   if(vflag)   
      printf("SIZE: %d\n",size);
   countSubstrings(buffer,size);
   if(vflag)
	 printHTable();
   free(buffer);
   output(fp1);
   /*free htable and mword*/
   emptyHTable();
   emptyMword();

}
/*remove all non alpha numeric characters*/
void filter_alpha(char *s)
{
   char* p = s;
   while (*p)
   {
      if (isalpha(*p))
      {
	 *s = *p;
	 s += 1; 
      }
      p += 1;
   }
   *s = '\0';
}
/*search @param buffer for substrings of size mnum to @param size/2*/
void countSubstrings(char* buffer, int size)
{
   /*declarations*/
   /*loop counters*/
   int i;
   int k;

   /*hold various substrings*/
   char* sub1;
   char* sub2;

   /*run along buffer*/
   char* sub1run;
   /*run along buffer starting from sub1run 
    * until the end looking for the subsequence*/
   char* runner;
   char* c;

   /*how many mwords we have initially*/
   int msize;
   /*how many locations we have initially*/
   int locsize;
   /*holds the count for current substring*/
   int tempC;
   /*holds the location for the current substring*/
   int tempLoc;

   /*initializations*/
   msize=INITIAL_MSIZE;
   total=tempLoc=mindex=tempC=0;

   sub1 = (char*) calloc ((size+1/HALF),sizeof(char));
   sub2 = (char*) calloc ((size+1/HALF),sizeof(char));
   mword= (matchingWord*) calloc (msize,sizeof(matchingWord));

   mword[mindex].count=0;
   if(vflag)
      printf("total size %d",size);
   if(vflag)
      printf("READ: %s\n",buffer);
   /*all length from mnum (defaults to 3) to size/2*/
   for(i=mnum;i<=ceil((size/2));i++)
   {
      if(vflag)
	 printf("Looking for substrings of size %d\n",i);
      /*start looking for substrings at the beginning*/
      sub1run=&buffer[0];

      /*all substrings of size i in the entire string*/
      for(k=0;k<=size-i-i;k++)
      {
	 sub1=memset(sub1, '\0', i);
	 strncpy(sub1,sub1run,i);
	 sub1[i]='\0'; 

	 /*only search for the substring if we have not already searched*/
	 if(check(hash(sub1),sub1,i))
	 {
	    runner=sub1run+i;	

	    if(vflag)
	       printf("SUB1  %s\n",sub1);
	    locsize=INITIAL_LOCSIZE;
	    mword[mindex].loc= (int*) calloc (locsize,sizeof(int));
	    mword[mindex].apart= (int*) calloc (locsize,sizeof(int));
	    mword[mindex].count=0;
	    if(vflag)
	    {
	       printf("Number of distinct words found: %d\n",mindex);
	       printf("We have moved %d along\n",k);
	    }
	    /*set initial location to first place we saw it, each time*/
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
	       strncpy(sub2,c,i);
	       runner=c;
	       /*checks to see if they match*/
	       if(strcmp(sub1,sub2)==0)
	       {
		  if(vflag)
		     printf("MATCH FOUND\n");
		  total++;
		  mword[mindex].count++;
		  tempC=mword[mindex].count;
		  /*realloc for more location spots if we need to*/
		  if(mword[mindex].count>=locsize-1)
		  {
		     if(vflag)
			printf("realloc number of locations to %d\n",locsize);
		     locsize=locsize*2;
		     mword[mindex].loc=(int*)realloc
			(mword[mindex].loc,(sizeof(int)*(locsize)));
		     mword[mindex].apart=(int*)realloc
			(mword[mindex].apart,(sizeof(int)*(locsize)));
		  }
		  /*check locations and see the difference
		   * then add difference to initial location*/
		  mword[mindex].loc[tempC]=(c-sub1run)+k;
		  tempLoc=mword[mindex].loc[tempC-1];
		  mword[mindex].apart[tempC]=mword[mindex].loc[tempC]-tempLoc;
		  if(vflag)
		  {
		     printf("location is  %d \n",tempLoc);
		  }
	       }
	       runner++;
	    }
	 }
	 /*if we found an or many occurence of the string*/
	 if(mword[mindex].count>0)
	 {
	    if(vflag)
	       printf("COUNT %d\n",mword[mindex].count);
	    mword[mindex].length=i;      
	    /*add newly found word to hashtable*/
	    addhash(hash(sub1),sub1,i);
	    mindex++;
	 }

	 sub1run++;
	 /*if we need to realloc for more mwords*/
	 if(mindex>=msize)
	 {
	    msize*=HALF;
	    mword=(matchingWord*) realloc(mword,(sizeof(matchingWord)*(msize)));

	 }
      }
      /*if no more words of size i can be found, break*/
      if(total==0)
      {
	 break;

      }  
      else
      {
	 total=0;
      }
   }
   /*we no longer need to check for subsequences
    * lets free some memory*/
   free(sub1);
   free(sub2);
}
/*create and initialize HTable*/
void createHashTable()
{
   /*loop counter*/
   int i;
   /*initialize size of table to 1024*/
   sizeHTable=ISIZE;
   htable=(char***)calloc(sizeHTable,sizeof(char**));
   /*calloc for every spot in the hash table*/
   for(i=0;i<sizeHTable;i++)
   {
      htable[i]=(char**)calloc(sizeHTable,sizeof(char*));

   }
}

/*Jenkin's hash function used to hash @param str
 * return hash
 * Very good hash function with minimal collisions, created by Bob Jenkins
 * More information about him and his hash at 
 * http://burtleburtle.net/bob/ */
uint32_t hash(char* str)
{
   uint32_t hash=0;
   size_t i;
   size_t len;
   len=strlen(str);
   for(i=0;i<len;++i)
   {
      hash += str[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
   }
   hash += (hash << 3);
   hash ^= (hash >> 11);
   hash += (hash << 15);
   return  hash%ISIZE;

}
/*checks to see if the @param str of length @param len is in the 
 * hashtable at @param hash
 * @return wheather the string should be added or not
 * 0 - no, 1 - yes*/
int check(uint32_t hash,char* str, int len)
{
   int i;
   i=0;
   /*double check to make sure the string
    * that we are looking for is not null*/
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
	 /*check all entries forward until we hit a null*/
	 while( htable[hash][hash+i]!=NULL)
	 {
	    /*if we check and we find it, return 0
	     * do not add string to the hash table*/
	    if(strncmp(htable[hash][hash+i],str,len)==0)
	    {
	       return 0;
	    }
	    i++;
	 }
      }
   }
   /*string was not found, add it to the table*/
   return 1;
}
/*add @param str of size @param len to HTable in index @param hash*/
void addhash(uint32_t hash,char* str, int len)
{
   /*declarations*/
   /*extend index, incase we have a collision*/
   uint32_t ext;
   /*loop counters*/
   int i,j;
   /*temp ptr,
    * make sure that when we realloc, no null*/
   void* temp;
   ext=0;
   len++;
   if(htable[hash]==NULL)
   {
      printf("NULL at %d\n\n",hash);
      exit(1);
   }
   /*no collision, hash is good*/
   if(htable[hash][hash]==NULL)
   {
      if(vflag)
	 printf("String being added %s\n",str);
      htable[hash][hash] =(char*)realloc(htable[hash][hash],len*sizeof(char));
      htable[hash][hash]=memset(htable[hash][hash], '\0', len);
      strcpy(htable[hash][hash],str);

   }
   /*a collision has occurred
    * we try to put the string in the next available spot*/
   else
   {
      ext=1;
      while(htable[hash][hash+ext] !=NULL)
      {
	 /*we have tried to put the string too far in the table 
	  * we need to realloc for more space*/
	 if(hash+ext>=(unsigned int) sizeHTable)
	 {
	    for(i=0;i<ISIZE;i++)
	    {
	       if(vflag)
		  printf("realloced to size %d\n",sizeHTable*2);
	       temp=realloc(htable[i],(sizeof(char*)*(sizeHTable*2)));
	       if(temp!=NULL)
	       {
		  htable[i]=(char**) temp;
		  /*set all new space equal to null
		   * I wish there was a calloc for realloc*/
		  for(j=sizeHTable-1;j<sizeHTable*2;j++)
		  {
		     htable[i][j]=memset(htable[i][j], '\0', sizeHTable*2);
		     htable[i][j]=NULL; 
		  }
	       }
	       else
	       {
		  if(vflag)
		     fprintf(stderr,"Not enough memory\n");
		  exit(EXIT_FAILURE);
	       }
	    }

	    sizeHTable*=HALF;
	 }
	 /*incremention extension so we know where to place the str*/
	 ext++;
      }
      htable[hash][hash+ext]=realloc(htable[hash][hash+ext],len*sizeof(char));
      htable[hash][hash+ext]=memset(htable[hash][hash+ext], '\0', len);
      if(vflag)
	 printf("String being added %s\n",str);
      strncpy(htable[hash][hash+ext],str,len-1);
   }
   /*let the mword struct know where the string is in the hash table*/
   mword[mindex].index1=hash;
   mword[mindex].index2=hash+ext;
}
/* print all strings stored in hashtable by location*/
/*This function is only called when verbosity flag present*/
void printHTable()
{

   /*loop counters, to iterate through entire HTable*/
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
/*free all items in HTable*/
void emptyHTable()
{
   /*loop counters, to iterate through entire HTable*/
   int i,j;
   for(i=0;i<sizeHTable;i++)
   {
      for(j=0;j<sizeHTable;j++)
      {
	 free(htable[i][j]);
      }
      free(htable[i]);
   }
   free(htable);
   if(vflag)
      printf("Table emptied\n");
}
/*free all items in mword*/
void emptyMword()
{
   /*loop counter, to iterate through all structs*/
   int i;
   for(i=0;i<mindex;i++)
   {  
      free(mword[i].apart);
      free(mword[i].loc);

   }

   free(mword);

}


/*print output to Dr. N specified format, 
 * extra space at end of each line*/
void output(FILE* fp1)
{
   /*loop through all letters, A-Z*/
   int i;
   /*loop though all asterixs*/
   int j;
   /*temp ptr used to print current string in HTable*/
   char* temp;
   temp=NULL;
   /*sort alphabetically using qsort*/
   alphasort(mindex);
   /*sort by count of substrings found*/
   insertCount(mindex);
   /*sort by length of strings*/
   insertLength(mindex);
   /*silly stuff that Dr. Nico forces us to print*/
   fprintf(stderr,"SKIPME This is a naive implementation");
   fprintf(stderr," of skipping overlapping\n");
   fprintf(stderr,"SKIPME strings.  It misses the problem");
   fprintf(stderr," of the skipped string\n");
   fprintf(stderr,"SKIPME possibly matching somewhere down the road.\n");

   if(fp1==NULL) 
   {
      printf("Length   Count Word    Location (distance)\n");
      printf("======   ===== ====    =========\n");
   }
   else
   {
      fprintf(fp1,"Length   Count Word    Location (distance)\n");
      fprintf(fp1,"======   ===== ====    =========\n");
   }
   /*print to stdout*/
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
	       /*extra space below*/
	       printf(" (%d) ",mword[i].apart[j]);
	    }
      	    printf("\n");
	 }

      }
   }
   /*print to a file*/
   /*see above for more details*/
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
	    for(j=1;j<mword[i].count;j++)
	    {
	       fprintf(fp1," %d",mword[i].loc[j]);
	       fprintf(fp1," (%d) ",mword[i].apart[j]);
	    }
	    fprintf(fp1,"\n");

	 }
      }
   }
}

/*compare to integers, return which one is greater*/
int compare(int a, int b)
{
   if(a>b)
      return -1;
   else if(a<b)
      return 1;
   else
      return 1;
}

/*order mwords by length
 * insertion sort*/
void insertLength(int size)
{
   /*loop counters*/
   int i,j;
   /*temp structure used for swapping*/
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

/*order mwords by count
 * insertion sort*/
void insertCount(int size)
{
   /*loop counters*/
   int i,j;
   /*temp variable used for swapping*/
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
/*use qsort to alphabetically sort strings in htable*/
/*written entirely by Sunjay*/
void alphasort(int size)
{
   void* base=mword;
   qsort(base, size, sizeof(matchingWord),compareStrings);

}
/*helper function to alphasort*/
/*written entirely by Sunjay*/
int compareStrings(const void *a, const void *b)
{  
   /*must create temporary matchingWords to cast @param a and @param b to*/
   matchingWord *ia = (matchingWord*)a;
   matchingWord  *ib = (matchingWord*)b;
   return strcmp(htable[ia->index1][ia->index2],htable[ib->index1][ib->index2]);
}

