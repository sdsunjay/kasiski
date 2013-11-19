#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define COUNT_DEFAULT 3
#define LOWER_INT_DEF 48
#define HIGHER_INT_DEF 57
#define ISIZE 1024
#define HALF 2
#define INITIAL_LOCSIZE 50
#define INITIAL_MSIZE 2000
#define INITIAL_STRING_SIZE 500
#define UPPER_TO_LOWER 32
#define LOWER_LCASE_DEF 97
#define UPPER_LCASE_DEF 122
#define READ_300 300

/*see kasiski.c for more information*/
/*hash functions*/
int check(uint32_t hash,char* str, int len);
void addhash(uint32_t hash,char* str, int len);
void createHashTable();
uint32_t hash(char* str);
void printHTable();
void emptyHTable();
/*substring functions*/
void countSubstrings(char*, int);
void filter_alpha(char* s);
void insertCount(int);
int compareStrings(const void *a, const void *b);
void alphasort(int size);
void insertLength(int);
void reader(FILE*,FILE*);
int compare(int, int);
void output(FILE*);
void emptyMword();

/*Stores all the information,
 *  which we need to print about a particular subsequence*/
typedef struct matchingWord 
{
   /*array of locations that the word appears*/
   int* loc;
   /*key in HTable*/
   short int index1;
   /*index into key*/
   short  int index2;
   /*length of subsequence*/
   short int length;
   /*number of subsequence found*/
   short int count;
   /*array of distances*/
   int* apart;
}matchingWord;

/*GLOBALS*/
/*These are probably bad to have, will fix later.*/
matchingWord* mword;
/*number of unique subseqences found*/
int mindex;
/*total number of subsequences found of a particular length*/
int total;
/*verbosity flag*/
int vflag;
/*look for subsequences of this length or greater*/
int mnum;
/*hash table*/
char*** htable;
/*stores the size of htable*/
int sizeHTable;
