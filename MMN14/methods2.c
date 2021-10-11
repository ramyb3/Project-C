#include "project.h"
	
char *reg[]={"r0","r1","r2","r3","r4","r5","r6","r7"};/*all registers in project*/

void registers()/*declaring the registers in 12 bits*/
{
	r0.opcode=0;
	r1.opcode=0;
	r2.opcode=0;
	r3.opcode=0;
	r4.opcode=0;
	r5.opcode=0;
	r6.opcode=0;
	r7.opcode=0;

	r0.funct=0;
	r1.funct=0;
	r2.funct=0;
	r3.funct=0;
	r4.funct=1;
	r5.funct=2;
	r6.funct=4;
	r7.funct=8;

	r0.from=0;
	r1.from=0;
	r2.from=1;
	r3.from=2;
	r4.from=0;
	r5.from=0;
	r6.from=0;
	r7.from=0;

	r0.address=1;
	r1.address=2;
	r2.address=0;
	r3.address=0;
	r4.address=0;
	r5.address=0;
	r6.address=0;
	r7.address=0;
}

void getreg(char *token, int line, char *name)
{
	int i=0, x;
	char *str;
	
	FILE *f;

	for(;;i++)
	if(isspace(token[i])==0)/*removes white char from beginning of input*/
	{
		str=strchr(token,token[i]);			
		break;
	}			
	
	for(i=0;i<strlen(str);i++)/*check if in end of input there's white char*/
	if(isspace(str[i])!=0)
	{
		str=strtok(str," ");
		break;
	}
	
	for(i=0;i<8;i++) /* 8- length of reg array*/	
	if(strcmp(str,reg[i])==0)
	break;
	
	f=fopen(name,"a");
	
	if(i==0)
	{
		x=lastbyte(r0.from,r0.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r0.opcode,r0.funct,x);
	}
	
	if(i==1)
	{
		x=lastbyte(r1.from,r1.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r1.opcode,r1.funct,x);
	}
	
	if(i==2)
	{
		x=lastbyte(r2.from,r2.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r2.opcode,r2.funct,x);
	}
	
	if(i==3)
	{
		x=lastbyte(r3.from,r3.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r3.opcode,r3.funct,x);
	}
	
	if(i==4)
	{
		x=lastbyte(r4.from,r4.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r4.opcode,r4.funct,x);
	}
	
	if(i==5)
	{
		x=lastbyte(r5.from,r5.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r5.opcode,r5.funct,x);
	}
	
	if(i==6)
	{
		x=lastbyte(r6.from,r6.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r6.opcode,r6.funct,x);
	}
	
	if(i==7)
	{
		x=lastbyte(r7.from,r7.address);/*calculate last byte in register*/
		/*adding register and line number and A sign to ".ob" file*/
		fprintf(f,"%d\t%X%X%X\tA\n",line,r7.opcode,r7.funct,x);
	}
	
	fclose(f);	
}

int method(char *token)
{
	int i=0, j=0;
	
	char temp[max_line], s[max_label], *str;
	char percentage='%', hash='#';
	
	FILE *fp;	
	
	strcpy(s,token);
	
	for(;;i++)
	if(isspace(token[i])==0)/*removes white char from beginning of input*/
	{
		str=strchr(token,token[i]);			
		strcpy(s,str);
		break;
	}		
	
	if(hash==s[0])
	return 0;	/*if the addressing method is 0*/
		
	for(i=0;i<strlen(s);i++)
	if(isspace(s[i])!=0)/*checks white char in input*/
	break;
	
	/*creating new input whitout white char in from source input*/	
	strncpy(temp,s,i);	
	strcpy(s,temp);
		
	for(i=0;i<8;i++)/* 8- length of reg array*/	
	if(strcmp(s,reg[i])==0)	
	return 3;	/*if the addressing method is 3*/
	
	for(i=0;i<strlen(s);i++)/*count how many '%' there's in input*/
	if(s[i]==percentage)
	j++;
	
	if(j>1)
	return -1;/*if there's more than 1 '%' in input*/
	
	fp=fopen("symbol","r");

	while(1)/*label checking*/
	{		
		fgets(temp,max_line,fp);/*get label from "symbol" file*/

		if(feof(fp))
		break;		
		
		if(percentage==s[0])/*addressing method 2*/
		{														
			str=strtok(s,"%");
			
			if(str==NULL)/*check if input valid*/
			{
				fclose(fp);
				return -1;/*if input invalid*/
			}
			
			if(strstr(temp,str)!=NULL)/*check if the label exist in "symbol" file*/		
			{
				fclose(fp);
				return 2;/*if the addressing method is 2*/
			}			
		}

		if(strstr(temp,s)!=NULL)/*check if the label exist in "symbol" file*/		
		{
			fclose(fp);
			return 1;/*if the addressing method is 1*/
		}
	}
	
	fclose(fp);	
	return -1; /*if input invalid*/
}
