#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


#define LOWER_INT_DEF 48
#define HIGHER_INT_DEF 57
#define NUM_ALPHABET 26
#define UPPER_TO_LOWER 32
#define LOWER_LCASE_DEF 97
#define UPPER_LCASE_DEF 122
#define LOWER_UCASE_DEF 65
#define UPPER_UCASE_DEF 90
#define ONEHUNDRED 100



/*GLOBALS*/
/*stores the count of each character*/
int letters[NUM_ALPHABET];
/*stores the total number of characters*/
double total;
/*stores each character from stdin or file*/
char c;
/*stores the number of characters to skip*/
int pnum;
/*stores the number of characters to skip at the beginning*/
int snum;
/*verbosity flag*/
int vflag;

/*FUNCTIONS*/
/*print function*/
void output(FILE* fp1);
/*read input from the keyboard*/
void  read(FILE* fp);
/*count alphabet characters*/
void add();
/*sets letter counts to 0*/
void cleanLetters();
/*compute the IC given each letters frequency*/
double computeIC(double freq);


