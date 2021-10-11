#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define max_line 80 /*max length of input command line*/ 
#define max_num 4096 /*max number to be shown in 3 digits in hexa*/
#define max_label 31 /*max length of input label*/

struct word /*decleration of word in 12 bits*/
{		
	unsigned int opcode:4;
	unsigned int funct:4;
	unsigned int from:2;
	unsigned int address:2;							
}r0,r1,r2,r3,r4,r5,r6,r7; /*decleration of registers*/

int checkcommand(char *, int, char *); /*checks input command line first time*/
int getcommand(char *, int, char *); /*checks input command line second time*/

/*calculates how many lines he need to jump to the inputed address*/
int percentage(char *, int, int, char *); 
int number(char *, int, char *); /*gets the number from input*/
int address(char *, int, char *); /*gets the number of line address*/

void registers(); /*declaring the registers in 12 bits*/ 
void getreg(char *, int, char *); /*gets the register from input*/
int method(char *); /*calculates the addressing method of input*/

/*this note meant to the 4 functions below that*/
/*check which command and translating it to object file*/
int arithmetic(char *, int, int, char *); 
int operation(char *token, int, int, char *);
int labels(char *token, int, int, char *);
int program(char *token, int, int, char *);

/*check which data operation and translating it to object file*/
int checkinst(char *, int, int, char *);

/*checks 2nd time the input and calling to all translating functions*/
int checksyntax(char *, char *, char *);
int lastbyte(int, int);/*calculates the last 4 bits in word to 1 byte*/
int checkspace(char *);/*check if there isn't another input except white char*/
int checkempty(char *, int); /*check if the input is empty*/
int checknote(char *, int, int); /*check if the input is note*/
int checklabel1(char *, char *); /*check if the input label is valid*/
int checklabel2(char *, char *, char *); /*check if there isn't problem with all the labels*/

