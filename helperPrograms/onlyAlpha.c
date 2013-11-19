#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void filter_alpha(char *s);
int main (int argc, char* argv[])
{
   char* buffer;
   int newsize;
   buffer =(char*)calloc(10000,sizeof(char));
   printf("Enter any string you like (no more than 300 chars long)\n");
   printf("I will remove all not alpha numeric characters!\n");
   while(fgets(buffer,300,stdin)!=EOF)
   {
      newsize++;
   }
   printf("You entered: %s\n",buffer);
   printf("Size of old string: %d\n",strlen(buffer));
   filter_alpha(buffer);
   printf("New string: %s\n",buffer);
   printf("Size of new string: %d\n",strlen(buffer));

   return 0;
}

void filter_alpha(char *s) {
   for (char *p = s; *p; ++p)
      if (isalpha(*p))
	 *s++ = *p;
   *s = '\0';
}


