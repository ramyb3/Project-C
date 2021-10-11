#include "project.h"		
						/*all commands in project*/	
char *s[]={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne",
		 "jsr","red","prn","rts","stop",".data",".string",".entry",".extern"};		   

int checkcommand(char *t, int commacount, char *token) 
{
	int i=0, c=0, count=0, count1=0, count2=0;
	
	char *str, quotation='"', comma=',';

	for(;;c++)
	if(isspace(t[c])==0)/*removes white char from beginning of command*/
	{
		str=strchr(t,t[c]);			
		strcpy(t,str);
		break;
	}

	for(;i<20;i++)/*20-length of array s*/
	if(strcmp(s[i],t)==0)
	break;		
						
	if(i>-1&&i<5)/*addressing to arithmetic commands*/
	{	
		if(token!=NULL)	/*check syntax of inputed line*/
		for(c=0;c<strlen(token);c++)
		{			
			if(count==0&&token[c]!=comma)
			if(isspace(token[c])==0)
			count1=1;
			
			if(count==1&&token[c]!=comma)
			if(isspace(token[c])==0)
			count2=1;
			
			if(token[c]==comma)
			count=1;									
		}
		
		if(count1==1&&count2==1)/*check if there's 2 arguments*/
		count=0;	
				
		if(token==NULL)/*check if command invalid*/
		count=1;
		
		if(count==1)
		return -1;/*if command invalid*/

		return 3;/*returns how many lines the command will use*/	
	}
	
	if(i>4&&i<14)/*addressing to operation and labels commands*/
	{
		if(token!=NULL)	/*check syntax of inputed line*/
		for(c=0;c<strlen(token);c++)
		{
			if(isspace(token[c])!=0)
			count=1;
			
			if(isspace(token[c])==0)
			{
				count=0;
				break;
			}			
		}
		
		if(token==NULL)/*check if command invalid*/
		count=1;
		
		if(count==1)
		return -1;/*if command invalid*/
		
		return 2;/*returns how many lines the command will use*/
	}

	if(i>13&&i<16)/*addressing to program commands*/	
	{
		if(token!=NULL)	
		for(c=0;c<strlen(token);c++)/*check syntax of inputed line*/
		{
			if(isspace(token[c])!=0)
			count=0;
			
			if(isspace(token[c])==0)
			{
				count=1;
				break;
			}			
		}	
		
		if(count==1)
		return -1;/*if command invalid*/

		return 1;/*returns how many lines the command will use*/
	}
	 
	c=0;
	
	if(token!=NULL)/*check if line not empty*/
	c=checkempty(token,c);
	
	if(token==NULL)/*if there isn't arguments*/
	c=-1;
		
	if((i==18||i==19)&&token!=NULL&&c==0)/*addressing to entry and extern commands*/
	{					
		if(i==18)/*entry*/													
		c=110;/*made up number that i wiil use in main*/
			
		if(i==19)/*extern*/													
		c=10;/*made up number that i wiil use in main*/			
	}							

	if(i==16&&token!=NULL&&c==0)/*addressing to data command*/
	c=commacount+11;/*number of commas + made up number that i wiil use in main*/

	if(i==17&&token!=NULL&&c==0)/*addressing to string command*/
	{	
		if(commacount>0)/*if there isn't any comma in line*/
		return -1;/*if command invalid*/
		
		for(count1=0;;count1++)
		if(isspace(token[count1])==0)/*removes white chars from beginning of string*/
		{
			str=strchr(token,token[count1]);			
			strcpy(token,str);
			break;
		}
			
		if(token[0]!=quotation)/*check if first char is '"'*/
		return -1;/*if command invalid*/
				
		for(count=0;count<strlen(token);count++)
		if(token[count]==quotation)/*checks how many chars in string*/
		{
			count++;
			for(;token[count]!=quotation&&count<strlen(token);count++,c++);
		}			
			
		if(c==0)/*if the string empty */
		return -1;/*if command invalid*/
			
		c=c+11;/*number of chars in string + made up number that i wiil use in main*/
	}
				
	return c;/*returns how many lines the command will use or below zero if invalid*/
}
		   
int getcommand(char *t, int line, char *name)
{
	int i=0, c=0;
	
	char *str, temp[max_line];
	
	for(c=strlen(t)-1;;c--)
	if(isspace(t[c])==0)/*removes white char from end of line*/
	{
		strcpy(temp,"");
		strncat(temp,t,c+1);
		break;
	}		

	str=strtok(temp," ");/*get the name of the command*/	

	c=-1;
	
	for(;i<20;i++)/*20-length of array s*/
	if(strcmp(s[i],str)==0)
	break;		

	if(i==20)/*if the command invalid*/			
	return c;
	
	if(i<14||i>15)/*all commands except rts and stop have arguments */
	str=strtok(NULL,"\n");
	
	/*this note meant to the 5 functions below that*/
	/*calling to functions according to inputed command*/			
	if(i>-1&&i<5)
	c=arithmetic(str,i,line,name);
	
	if((i>4&&i<9)||i==12||i==13)
	c=operation(str,i,line,name);

	if(i>8&&i<12)
	c=labels(str,i,line,name);
	
	if(i>13&&i<16)
	c=program(str,i,line,name);	
	
	if(i>15&&i<20)
	c=checkinst(str,i,line,name);
	
	return c;/*returns if the command valid or not*/
}
