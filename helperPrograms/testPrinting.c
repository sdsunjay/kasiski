#include <stdlib.h>
#include <stdio.h>
int main(void)
{
   char e;
   e='t';
   printf("val is %d\n",(e-'v'));
   printf("val is %d\n",(e-'r'));
   printf("val is %d\n",(e-'k'));
   printf("val is %d\n",(e-'i'));
   printf("val is %d\n",(e-'f'));
   e='e';
   printf("val is %d\t%d\n",('v'-e),('v'-e)+65);
   printf("val is %d\t%d\n",('r'-e),('r'-e)+65);
   printf("val is %d\t%d\n",('k'-e),('k'-e)+65);
   printf("val is %d\t%d\n",('i'-e),('i'-e)+65);
   printf("val is %d\t%d\n",('f'-e),('f'-e)+65);


 e='a';
   printf("val is %d\t%d\n",('v'-e),('v'-e)+65);
   printf("val is %d\t%d\n",('r'-e),('r'-e)+65);
   printf("val is %d\t%d\n",('k'-e),('k'-e)+65);
   printf("val is %d\t%d\n",('i'-e),('i'-e)+65);
   printf("val is %d\t%d\n",(e-'f'),(e-'f')+65);


 e='o';
   printf("val is %d\t%d\n",('v'-e),('v'-e)+65);
   printf("val is %d\t%d\n",('r'-e),('r'-e)+65);
   printf("val is %d\t%d\n",('k'-e),('k'-e)+65);
   printf("val is %d\t%d\n",('i'-e),('i'-e)+65);
   printf("val is %d\t%d\n",(e-'f'),(e-'f')+65);
	 
   return 0;
}
