#include "header1.h"
void countSubstrings(char* buffer, int size);
int main (int argc, char *argv[]){
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
   mnum=1;
   /*skip 'ftable' */
   i=1;
   /*while last argument has not been parsed, parse*/
   while(argc>i)
   {
      if(strcmp(argv[i],"-m")==0)
      {
	 i++;
	 if(!(argv[i][0]>=48 && argv[i][0]<=57))
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
	       printf("usage: ftable [ -v ] [ -s num ] [ -p num ] [ infile [ outfile ] ]\n");
	       exit(1);
	    }

	 }
	 else if(vflag)
	    printf("Outputting to to stdout\n");
      }
      i++;
   }
   reader(fp);
   //output(fp1);
   /*no input or outfiles specified*/
   return 0;
}
/*@function read reads input from keyboard until EOF
 * @param fp specifies where input is read from
 */
void reader(FILE* fp){
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
	 /*copy entire wout file*/
	 strcat(buffer, line);


      }
      size=strlen(buffer);
   } 
   /*read from file*/
   else
   {
      /*obtain file size:*/
      fseek (fp , 0 , SEEK_END);
      size = ftell (fp);
      rewind (fp);

      /*allocate memory for the whole file*/
      buffer = (char*) malloc (sizeof(char)*size);
      if (buffer == NULL) 
	 exit (1);
      /*copy the file into the buffer*/
      result = fread (buffer,1,size,fp);
      if (result != size) {
	 fprintf (stderr,"Reading error\n"); 
	 exit (1);
      }
      fclose (fp);
   }
   //printf("%s",buffer);
   countSubstrings(buffer,size);
   free (buffer);

   /*******/

   // if(isalpha(c))
   //  printf("%c",c);
}

void countSubstrings(char* buffer, int size){
   int i;
   int j;
   int k;
   int count;
   char* sub1;
   char* sub2;
   char* runner;
   char* sub1run;
   sub1 = (char*) malloc (sizeof(char)*(size));
   sub2 = (char*) malloc (sizeof(char)*(size));
   count=0; 

   for(i=3;i<size/2;i++)
   {
      //printf("buffer: %s\n",buffer);
      sub1=memset(sub1, '\0', size);
      strncpy(sub1,buffer,i);
      sub1run=&buffer[0];

      //sub1[i]='\0';
      printf("%d First substring: %s\n\n",i,sub1);
      //printf("runner: %s\n",runner);

      for(k=0;k<=size-i-1;k++)
      {
	 runner=&buffer[0];
	 sub2=memset(sub2, '\0', size);
	 strncpy(sub2,runner,i);
	 //sub2[i]='\0';
	 //printf("%d second substring: %s\n\n",i,sub2);
	 /*all substrings size 3 to total file size /2*/

	 for(j=0;j<=size-i-1;j++)
	 {
	    if(strcmp(sub1,sub2)==0)
	    {
	       //printf("AHAAHHA!\n\n");
	       count++;
	    }
	    //printf(" %d %s",j,sub2);
	    //else
	    // printf("F %s\nSecond substring %s\n",subs,runner);
	    runner++;
	    //printf("%s",runner);
	    //printf("%c",runner[strlen(runner)-1]);
	    strncpy(sub2,runner,i);
	 }
	 printf("There are %d %s in this file (length %d)\n",count,sub1,i);
	 count=0;
	 sub1run++;
	 sub1=memset(sub1, '\0', size);
	 strncpy(sub1,sub1run,i);

      }
      //printf("sub2, final: %s\n",sub2);
      //printf("runner: %s\n",runner);
   }
   free(sub1);
   free(sub2);
}

/*@function output prints to stdout or a file
 * @param fp1 specifies a file to output to
 */
void output(FILE* fp1)
{
   /*loop through all letters, A-Z*/
   int i;
   /*loop though all asterixs*/
   int j;
   /*temporarily stores final percentage of a given letter*/
   double temp;
   if(fp1==NULL) 
      printf("Total chars: \n");
   else
      fprintf(fp1,"Total chars: %d\n");
   if(fp1==NULL)
   {
      printf("%) ");
      for(j=0;j<=temp;j++)
	 printf("*");
      printf("\n");
   }
   else
   {
      fprintf(fp1,"%) ");
      for(j=0;j<=temp;j++)
	 fprintf(fp1,"*");
      fprintf(fp1,"\n");
   }
}
