#include "icHeader.h"
int main(int argc, char* argv[])
{

   /*declarations*/
   /*counter to parse commandline args*/
   int i;
   /*message length*/
   double mlength;
   /*key length*/
   double keylength;
   /*shift amount*/
   int shift;

   /*initializations*/
   mlength=0;
   keylength=0;
   i=1;

   /*if there is stuff at the command line*/
   if(argv[i]!=NULL)
   {
      if(!(argv[i][0]>=LOWER_INT_DEF && argv[i][0]<=HIGHER_INT_DEF))
      {
	 fprintf(stderr,"usage: ic N l [ l2 [...] ] \n");
	 exit(1);
      }
      mlength = atoi(argv[i]);
      i++;
      printf("Key Expected IC"); 
      printf("N=%d)\n---- ------------------\n",(int)mlength);
      
      /*clever way to set shift amount*/
      shift=floor(log10 (abs (mlength))) + 1;
      
      while(argc>i)
      {

	 if(!(argv[i][0]>=LOWER_INT_DEF && argv[i][0]<=HIGHER_INT_DEF))
	 {
	    exit(1);
	 }
	 else
	 {
	    /*read length from command line*/
	    keylength=atoi(argv[i]);
	    /*print in Nico specified format*/
	    printf("%*d ",shift,(int)keylength);
	    printf("%.4f\n",computeIC(mlength,keylength));
	    i++;
	 }
      }
   }
   else
   {
      exit(1);
   }
   return 0;
}
/*Computes and returns the index of coincidence given 
 * message length and key length
 */
double computeIC(double mlen,double klen)
{
   /*temp variable used as Dr.N does not like lines lones than 80 chars*/
   double temp;
   temp=(1/klen)*((mlen-klen)/(mlen-1))*(zeroPZeroSixSix);
   return temp+(((klen-1)/(klen))*((mlen)/(mlen-1))*(zeroPZeroThreeEight));
}
