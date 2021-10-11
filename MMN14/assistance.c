#include "project.h"

char *language[]={
	"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne",
	"jsr","red","prn","rts","stop","data","string","entry","extern",
    "r0","r1","r2","r3","r4","r5","r6","r7"}; /* RESERVED WORDS */

int lastbyte(int j, int k) /*explanation in header file*/
{
	int calc=k;
		
	if(j==0)
	calc+=0;
		
	if(j==1)
	calc+=4;
		
	if(j==2)
	calc+=8;
		
	if(j==3)
	calc+=12;	

	return calc; /*returns the last byte of word*/
}

int checkspace(char *t)
{
	int i=0, ok=0;	

	for(;isspace(t[i])!=0&&i<strlen(t);i++); /*skip white chars*/
		
	for(;isspace(t[i])==0&&i<strlen(t);i++)	/*checks if the chars attached to each other*/	
	ok=0;				
		
	for(;isspace(t[i])!=0&&i<strlen(t);i++); /*skip white chars*/
		
	for(;isspace(t[i])==0&&i<strlen(t);i++) /*checks if there's more non-white chars*/
	ok=1;		

	if(ok==1) /*if there is more than one input, the input invalid*/
	return -1;	
	
	return 0; /*if there isn't more than one input, the input is ok*/	
}

int checkempty(char *t, int ok)
{
	int i;
	
	for(i=0;i<strlen(t);i++) /*checks if the input line is empty*/				
	{
		if(isspace(t[i])==0)		
		{
			ok=0;
			break;				
		}
						
		if(isspace(t[i])!=0)					
		ok=-2;			
	}
	
	return ok;/*ok=-2 line empty, ok=0 line valid*/
}

int checknote(char *t, int ok, int x)
{
	char semicolon=';';
	int i, j;

	if(x==0)/*if there isn't string in line*/				
	for(i=0;i<strlen(t);i++) /*checks if the input line is note*/	
	{				
		if(t[i]==semicolon)/*checks if the semicolon is first char in line*/
		for(j=0;j<=i;j++)
		{
			if(i==j)
			{
				ok=-2;
				break;			
			}
			
			if(isspace(t[j])==0) 
			{
				ok=-1;
				break;			
			}				
		}

		if(ok<0)
		break;
	}

	if(x!=0)/*if there is string in line*/				
	for(i=0;i<x;i++) /*checks if the input line is note*/	
	{				
		if(t[i]==semicolon)/*checks if the semicolon is first char in line*/
		for(j=0;j<=i;j++)
		{
			if(i==j)
			{
				ok=-2;
				break;			
			}
			
			if(isspace(t[j])==0) 
			{
				ok=-1;
				break;			
			}				
		}

		if(ok<0)
		break;
	}
	
	/*ok=-1 the line is invalid, ok=-2 the line is empty or note, ok=0 line valid*/
	return ok;
}

int checklabel1(char *temp, char *str)
{	 
	int ok=1, i;	
   
	if(isalpha(temp[0])==0) /*if the label don't start with a letter, the label invalid*/
	ok=-1;

	i=checkspace(temp); /*check inputed label*/
				
	if(i==-1) /*check if the label invalid*/
	ok=-1;

	for(i=0;i<28;i++) /*28- length of array language*/
	if(strcmp(language[i],temp)==0) /*checks if the label is a reserved word*/
	{
		ok=-2;
		printf("\nYOU USED A RESERVED WORD IN LINE:\n%s\n",str);
		break;
	}
				
	if(strlen(temp)>max_label) /*checks if the label is too long*/
	{
		ok=-2;
		printf("\nTHE LABEL - \"%s\" IS TOO LONG!! IN LINE:\n%s",temp,str);
	}	 
 
	return ok; /*if ok=-2 there's another type of error*/   
}

int checklabel2(char *s, char *temp, char *line)
{	
	int x, i, j, error=0;
	char *t;
	FILE *fp;	
		
	fp=fopen("symbol","r");
	
	i=fgetc(fp);/*gets the first char in the file*/
		
	if(i!=-1) /*checks if the file empty*/
	{	
		rewind(fp);
							
		x=0;
		i=0;	
		j=-1;
			
		while(1)/*algorithm that checks if there is a label that appears more than once*/
		{		
			fgets(s,max_line,fp);/*gets line from "symbol" file*/					
		
			if(j==i)/*if i got to the last line*/
			break;

			if(feof(fp))	
			{
				if(i==0)/*only in first check*/			
				{
					j=x;
					
					if(j==0)/*if there isn't lines in "symbol" file*/
					break;
				}
			
				i++;
				x=i;
				
				rewind(fp);/*returns to file beginning*/
			
				for(i=0;i<x;i++) /*skipping lines that i already checked*/				
				fgets(s,max_line,fp);
		
				i=x;
			}	
																				
			t=strtok(s,"\t");/*gets the line number*/
			strcpy(line,t);

			if(strcmp(t,"1")!=0)/*1- entry label*/
			{						
				t=strtok(NULL,"\n");/*gets the line label*/		
		
				if(x==i)
				strcpy(temp,t);
		
				if(x!=i)
				if(strcmp(temp,t)==0&&strcmp(line,"0")!=0)/*0- extern label*/
				{
					error=1;										
					printf("\nTHE LABEL - \"%s\" APPEARS MORE THAN ONCE!!\n",t);
					break;
				}
		
				x++;/*counts how many lines there are in this file*/
			}															
		}
	}
	
	fclose(fp);
	return error;
}

int checksyntax(char *s, char *str, char *name)
{
	char *t;
	int i, j, x, error=0;
	FILE *pt;

	pt=fopen("commands","r");

	while(1)/*algorithm that checks if the input line is valid*/
	{
		j=0;
		
		fgets(s,max_line,pt);/*gets line from "commands" file*/		

		if(feof(pt))
		break;

		strcpy(str,s);
			 		 	
		t=strtok(str,"\t");
		x=atoi(t); /*gets the line number of the command*/			
			
		t=strtok(NULL,"\n"); /*gets the command*/			
							
		for(i=0;;i++)/*removes white char in the beginning of line*/
		if(isspace(t[i])==0)
		{
			t=strchr(t,t[i]);								
			break;
		}					

		/*if the line valid, calls to function that check which command and translate it*/
		j=getcommand(t,x,name);	 	

		if(j==-1)				
		{		
			error=1;
			printf("\nERROR IN LINE:\n%s\n",s);
		}
		
		/*if function checkinst returns -2 there is another type of error*/	
		if(j==-2)				
		error=1;				
	}
								
	fclose(pt);
	return error;
}
