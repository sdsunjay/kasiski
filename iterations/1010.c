#include "header.h"
#define COUNT_DEFAULT 3
#define LOWER_INT_DEF 48
#define HIGHER_INT_DEF 57
int main (int argc, char *argv[])
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
/*@function read reads input from keyboard until EOF
 * @param fp specifies where input is read from
 */
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
   countSubstrings(buffer,size);
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
   msize=100;
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
      //printf("%d\n",i);
      //printf("buffer: %s\n",buffer);
      sub1run=&buffer[0];
      //printf("%d substring: %s\n\n",i,sub1);

      /*all substrings of size i*/
      for(k=0;k<=size-i-i;k++)
      {
	 if(strstr(sub1run,sub1)!=NULL)
	    runner=strstr(sub1run,sub1);
	 else
	    {
	       k++;
	       sub1run++;
	    }
	 sub1=memset(sub1, '\0', i);
	 strncpy(sub1,sub1run,i);
	 if(vflag)
	    printf("SUB1  %s\n",sub1);
	  //printf("%s\n",sub1);
	 //printf("FAIL: COunt: %d  Length: %d Loc: %d\n",mword[mindex].count,mword[mindex].length,mword[mindex].loc[mword[mindex].count]);
	 //printf("sub1run: %s\n",sub1run);
	 //runner=sub1run+i;
      	/* if(k>=size-i)
	 {
	    printf("MINUS");
	    runner=sub1run;
	 }*/
	    //printf("SUB1RUN %s",sub1run);
	 locsize=50;
	 //mword[mindex].loc= (int*) calloc (locsize,sizeof(int));
	 //mword[mindex].apart= (int*) calloc (locsize,sizeof(int));
	 mword[mindex].count=0;
	 //printf("Mindex: %d\nK: %d\n",mindex,k);
	 //mword[mindex].loc[mword[mindex].count]=k;
	 //if(k<2 && i<4)
	 //printf("word: %s | loc: %d at index %d\n",sub1,mword[mindex].loc[mword[mindex].count],mword[mindex].count);

	 //printf("RUNNER  %s\n",runner);
	 sub2=memset(sub2, '\0', i+1);

	 strncpy(sub2,runner,i);
	 /*fills sub2 with all substrings of length i 
	  *  which are to the right of sub1 that we have not seen before*/
	    //if check sub1
	    while((c=strstr(runner,sub1))!=NULL) 
	    {
	       sub2=memset(sub2, '\0', i);

	       strncpy(sub2,c,i);
	       runner=c;
	       /*checks to see if they match*/
	       if(strcmp(sub1,sub2)==0)
	       {
		  total++;
		 //add sub1 to hashtable
		  mword[mindex].count++;

		  /* if(mword[mindex].count>=locsize-1)
		     {
		     printf("Count: %d\nlocsize: %d\n",mword[mindex].count,locsize);
		     locsize=locsize*2;
		     mword[mindex].loc=(int*)realloc(mword[mindex].loc,(sizeof(int)*(locsize)));
		     mword[mindex].apart=(int*)realloc(mword[mindex].apart,(sizeof(int)*(locsize)));
		     }*/


		  //mword[mindex].loc[mword[mindex].count]=j+k+i;
		  //if(mword[mindex].count==1)
		  // printf("First location is  %d \n",mword[mindex].loc[0]);

		  //printf("FAIL1: COunt: %d  Length: %d Loc: %d\n",mword[mindex].count,mword[mindex].length,mword[mindex].loc[mword[mindex].count]);
		  //printf("location is  %d \n",mword[mindex].loc[mword[mindex].count]);
		  //if(mword[mindex].apart[mword[mindex].count]==-1)
		  // mword[mindex].count--;

	       }
	       runner++;
	    }
	 if(mword[mindex].count>0)
	 {
	    //printf("COUNT %d\n\n",mword[mindex].count);
	    mword[mindex].length=i;      
	    //spaceApart();

	    //mword[mindex].str= (char*) calloc (strlen(sub1),sizeof(char));
	    //strcpy(mword[mindex].str,sub1);
	    //if(vflag)
	    // printf("%s\n",mword[mindex].str);
	    mindex++;
	 }
      	 sub1run++;
	 //emptyHTable();
	 if(mindex>=msize)
	 {
	    msize*=2;
	    mword=(matchingWord*) realloc(mword,(sizeof(matchingWord)*(msize)));

	 }
      }
      if(total==0)
	 break;
   }
   free(sub1);
   free(sub2);
}

/*check to see if we have already looked for that sub*/
int alreadySeen(char* sub1, char* visited)
{
   if(strstr(visited,sub1)!=NULL)
      return 1;
   return 0;
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

/*@function output prints to stdout or a file
 * @param fp1 specifies a file to output to
 */
void output(FILE* fp1)
{

   //insertAlpha(mindex);
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
	    printf("%d. Count: %d Length: %d\n",i,mword[i].count,mword[i].length);
	    //printf("%6d%6d %+*s   %-d",mword[i].length, mword[i].count,mword[0].length+1,mword[i].str,mword[i].loc[0]);
	    //for(j=1;j<mword[i].count;j++)
	    //{
	    // printf(" %d",mword[i].loc[j]);
	    //printf(" (%d)",mword[i].apart[j]);
	    //}
	    //free(mword[i].apart);
	    //problem here
	    //free(mword[i].loc);
	    //free(mword[i].str);
	    //printf("\n");
	 }

	 //free(mword[i]);
      }
      free(mword);
   }
   else
   {
	 printf("not null fp1\n");
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
   int i, didSwap = 1, limit = size-1;
   matchingWord temp;
   while (didSwap) {
      didSwap = 0;
      for (i = 0; i < limit; i++) {
	 if (strcmp (mword[i].str, mword[i+1].str) > 0) {
	    temp          = mword[i];
	    mword[i]   = mword[i+1];
	    mword[i+1] = temp;
	    didSwap = 1;
	 }
      }
      limit--;
   }
}


