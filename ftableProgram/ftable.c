#include "ftableHeader.h"
int main (int argc, char *argv[])
{
   /*declarations*/
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
   pnum=1;
   snum=0;
   /*skip 'ftable' */
   i=1;
   /*total number of characters entered*/
   total=0;
   /*while last argument has not been parsed, parse*/
   while(argc>i)
   {
      if(strcmp(argv[i],"-s")==0)
      {
	 i++;
	 if(!(argv[i][0]>=LOWER_INT_DEF && argv[i][0]<= HIGHER_INT_DEF))
	 {
	    printf("usage: ftable [ -v ] [ -s num ]");
	    printf(" [ -p num ] [ infile [ outfile ] ]\n");
	    exit(1);
	 }
	 snum= atoi(argv[i]);
	 if(vflag)
	    printf("Skipping first %d characters of input\n",snum);
      }
      else if(strcmp(argv[i],"-v")==0)
      {
	 vflag=1;
	 printf("Verbose mode: Engaged!\n");
      }
      else if(strcmp(argv[i],"-p")==0)
      {
	 i++;
	 /*check if period option is specified with number*/
	 if(!(argv[i][0]>= LOWER_INT_DEF && argv[i][0]<=HIGHER_INT_DEF) )
	 {
	    printf("usage: ftable [ -v ] [ -s num ]");
	    printf(" [ -p num ] [ infile [ outfile ] ]\n");
	    exit(1);
	 }
	 /*make string into int*/
	 pnum= atoi(argv[i]);
	 if(vflag)
	    printf("Skipping every %d characters of input\n",pnum);
      }
      else
      {
	 fp=fopen(argv[i],"r");
	 if(vflag)
	    printf("Reading from %s\n",argv[i]);
	 if(fp==NULL)
	 {
	    printf("%s: invalid option -- '%c'\n",argv[0],argv[i][1]);
	    printf("usage: ftable [ -v ] [ -s num ]");
	    printf(" [ -p num ] [ infile [ outfile ] ]\n");
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
	       printf("usage: ftable [ -v ] [ -s num ]");
	       printf(" [ -p num ] [ infile [ outfile ] ]\n");
	       exit(1);
	    }

	 }
	 else if(vflag)
	    printf("Outputting to to stdout\n");
      }
      i++;
   }
   cleanLetters();
   read(fp);
   output(fp1);
   /*no input or outfiles specified*/
   return 0;
}
/*@function read reads input from keyboard until EOF
 * @param fp specifies where input is read from
 */
void read(FILE* fp)
{
   int i;
   int pcount;
   pcount=0;
   i=0;
   c=0;

   /*read from stdin*/
   if(fp==NULL)
   {
      while((c=getchar())!=EOF)
      {
	 if(isalpha(c))
	    i++;
	 if(i>snum)
	 {

	    if(pnum!=1)
	    {
	       if(isalpha(c))
		  pcount++;
	       if(pcount%pnum==1) 
		  add();
	    }
	    else
	       add();
	 }
      }
   } 
   /*read from file*/
   else
   {
      while((c=fgetc(fp))!=EOF)
      {
	 if(isalpha(c))
	    i++;

	 if(i>snum)
	 {
	    if(isalpha(c))
	       pcount++;
	    if(pnum!=1)
	    {
	       if(pcount%pnum==1 )
	       {
		  add();	  
	       }
	    }
	    else
	    {
	       add(); 
	    }
	 }
      }
   }
}

/*@function add increases the count of an alphabet letter*/
void add()
{

   /*if lowercase, make uppercase*/
   if(c>=LOWER_LCASE_DEF && c<=UPPER_LCASE_DEF)
   {
      c-=UPPER_TO_LOWER;
   }
   /*uppercase letters*/
   if(c>= LOWER_UCASE_DEF && c<=UPPER_UCASE_DEF)
   {
      /*increment count of a letter, 
       * using int representation of lettter as index*/
      letters[c-LOWER_UCASE_DEF]=letters[c-LOWER_UCASE_DEF]+1;
      if(vflag)
      {
	 printf("%c at index %d ",c,c-LOWER_UCASE_DEF);
	 printf("increased to count %d\n",letters[c-LOWER_UCASE_DEF]);
      }
      total++;
   }
}

/*set all counts to 0*/
void cleanLetters()
{
   int i;
   for(i=0;i<NUM_ALPHABET;i++)
      letters[i]=0;


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
   /*stores the index of coincidence counter*/
   double ic;

   temp=0;
   ic=0;

   if(fp1==NULL) 
      printf("Total chars: %d\n",(int)total);
   else
      fprintf(fp1,"Total chars: %d\n",(int)total);
   /*for all the numbers in the alphabet*/
   for(i=0;i<NUM_ALPHABET;i++)
   {

	 if(total!=0)
	 {
	    /*only computer the IC if total>0
	     * sum up all letters A-Z*/
	    ic+=computeIC((double)letters[i]);
	    
	    temp=((double)letters[i])/total;
	    temp*=ONEHUNDRED;
	    temp=round(temp*ONEHUNDRED)/ONEHUNDRED;
	 }
	 if(fp1==NULL)
	 {
	    printf("%c:%10d ( %5.2f",i+LOWER_UCASE_DEF,letters[i],temp);
	    printf("%%) ");
	    if(letters[i])
	    for(j=0;j<ceilf(temp);j++)
	       printf("*");
	    printf("\n");
	 }
	 else
	 {
	    fprintf(fp1,"%c:%10d ( %5.2f",i+LOWER_UCASE_DEF,letters[i],temp);
	    fprintf(fp1,"%%) ");
	    if(letters[i])
	    for(j=0;j<=temp;j++)
	       fprintf(fp1,"*");
	    fprintf(fp1,"\n");
	 }
   }
/*new stuff added to original ftable.c*/
   if(total)
   {
      /*get the final IC if total chars is greaters than zero*/
      ic*= 1/((total)*(total-1));
      if(fp1==NULL)
      {
	 printf("\nIndex of Coincidence:");
	 printf(" %.4f\n",ic);

      }
      else
      {
	 fprintf(fp1,"\nIndex of Coincidence:");
	 fprintf(fp1," %.4f\n",ic);
      }
   }
   else
   {
      fprintf(stderr,"Index of coincidence requires");
      fprintf(stderr," at least two characters.\n");
   }
}

double computeIC(double freq)
{
   if(freq!=0)
      return freq*(freq-1);
   return 0;
}
