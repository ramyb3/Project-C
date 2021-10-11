#include "project.h"

FILE *f;

int address(char *token, int line, char *name)
{
	int i;
	char temp[max_line], s[max_line], *t;
	FILE *fp;
	
	fp=fopen("symbol","r");

	for(i=0;;i++)
	if(isspace(token[i])==0)/*removes white char from beginning of input*/
	{
		t=strchr(token,token[i]);					
		break;
	}	

	for(i=0;i<strlen(token);i++)
	if(isspace(token[i])!=0)/*check if in end of input there's white char*/
	{
		t=strtok(t," ");
		break;
	}
	
	strcpy(s,t);

	while(1)
	{		
		fgets(temp,max_line,fp);/*get label from "symbol" file*/
		
		if(feof(fp))
		break;
				
		t=strtok(temp,"\t");/*get the line number*/		
		i=atoi(t);
			
		t=strtok(NULL,"\n");/*get the label*/		
					
		if(strcmp(t,s)==0)		
		{
			f=fopen(name,"a");
		
			/*adding number and line number and R sign to ".ob" file*/
			/*adding 0 in fprintf for good representation in hexa*/
		
			if(i>255)
			fprintf(f,"%d\t%X\tR\n",line,i);
			
			if(i>15&&i<256)
			fprintf(f,"%d\t0%X\tR\n",line,i);
			
			if(i>0&&i<16)
			fprintf(f,"%d\t00%X\tR\n",line,i);
			
			if(i==0)/*if the address is external*/
			{
				FILE *p;
				
				strcpy(temp,name);
				
				/*changing the extension of the source file with the same name*/
				t=strtok(temp,".");
				strcat(t,".ext");
				
				p=fopen(t,"a");
				
				/*adding extern label and line number to ".ext" file*/
				fprintf(p,"%s\t%d\n",s,line);
				
				/*adding number and line number and E sign to ".ob" file*/
				/*adding 0 in fprintf for good representation in hexa*/				
				fprintf(f,"%d\t00%X\tE\n",line,i);
				
				fclose(p);
			}
			
			fclose(f);
			fclose(fp);
			return 0;/*if input was valid*/
		}			
	}
	
	fclose(fp);
	return -1;/*if input invalid*/
}

int percentage(char *token, int line, int next, char *name)
{
	int i;
	char temp[max_line], s[max_label], *t, *str;
	FILE *fp;
	
	for(i=0;;i++)
	if(isspace(token[i])==0)/*removes white char from beginning of input*/
	{
		t=strchr(token,token[i]);					
		break;
	}	

	for(i=0;i<strlen(token);i++)
	if(isspace(token[i])!=0)/*check if in end of input there's white char*/
	{
		t=strtok(t," ");
		break;
	}
		
	strcpy(s,t);

	fp=fopen("symbol","r");

	str=strtok(s,"%");/*gets the label without '%' in the beginning*/		
	strcpy(s,str);

	while(1)
	{		
		fgets(temp,max_line,fp);/*get label from "symbol" file*/
		
		if(feof(fp))
		break;
			
		t=strtok(temp,"\t");/*get the line number*/		
		i=atoi(t);
				
		t=strtok(NULL,"\n");/*get the label*/		
				
		if(strcmp(t,s)==0)		
		{
			i=i-line;/*get the sum of lines to jump*/

			f=fopen(name,"a");
			
			/*adding number and line number and A sign to ".ob" file*/
			/*adding 0 in fprintf for good representation in hexa*/

			if(i<0)
			{
				i=max_num+i;
				fprintf(f,"%d\t%X\tA\n",next,i);
				
				fclose(f);
				fclose(fp);
				return 0;/*if input valid*/			
			}
				
			if(i<16&&i>-1)
			fprintf(f,"%d\t00%X\tA\n",next,i);
		
			if(i<256&&i>15)
			fprintf(f,"%d\t0%X\tA\n",next,i);
		
			if(i>255)
			fprintf(f,"%d\t%X\tA\n",next,i);
			
			fclose(f);
			fclose(fp);
			return 0;/*if input valid*/
		}
	}				
	
	fclose(fp);
	return -1;/*if input invalid*/
}

int number(char *token, int line, char *name)
{
	int i, j=0, c=0;
	char *t, s[max_label], hash='#', minus='-';

	strcpy(s,token);
	
	for(i=0;i<strlen(token);i++)/*count how many '#' there are in input*/
	if(token[i]==hash)
	j++;

	if(j==1)/*only if there's 1 '#'*/
	{	
		/*get the number without '#'*/				
		t=strchr(s,hash);
		t=strtok(t,"#");
		
		j=0;
		
		for(i=0;i<strlen(t);i++)/*checks validation of input number*/
		if(isspace(t[i])==0)
		{
			if(isdigit(t[i])==0&&t[0]!=minus)
			break;
		
			j=1;
			break;
		}

		for(i=1;i<strlen(t);i++)/*checks if there isn't more minus sign than should be*/
		if(t[i]==minus)
		{
			c=1;
			break;
		}			
		
		if(t==NULL||j==0||c==1)
		return -1;/*if command invalid*/
		
		i=atoi(t);/*get the number*/
		
		if(t[0]==minus&&isdigit(t[1])==0)/*check that the minus next to the number*/
		return -1;/*if command invalid*/
		
		f=fopen(name,"a");

		if(t[0]==minus)/*check if the number is negative*/
		{					
			t=strtok(t,"-");/*gets the number after minus sign*/
				
			c=atoi(t);
				
			if(c>(max_num/2))/*checks if the number valid*/
			{
				fclose(f);
				return -1;/*if command invalid*/				
			}
					
			i=max_num-c;/*get the final number according to two's complement*/
			
			/*adding number and line number and A sign to ".ob" file*/	
			fprintf(f,"%d\t%X\tA\n",line,i);
											
			fclose(f);				
			return 0;/*if command valid*/									
		}
		
		/*adding number and line number and A sign to ".ob" file*/
		/*adding 0 in fprintf for good representation in hexa*/
			
		if(i<16&&i>-1)
		fprintf(f,"%d\t00%X\tA\n",line,i);
		
		if(i<256&&i>15)
		fprintf(f,"%d\t0%X\tA\n",line,i);
		
		if(i>255&&i<(max_num/2))
		fprintf(f,"%d\t%X\tA\n",line,i);			
			
		if(i>((max_num/2)-1))/*checks if the number valid*/
		{
			fclose(f);
			return -1;/*if command invalid*/
		}
			
		fclose(f);
		return 0;/*if command valid*/				
	}

	return -1;/*if command invalid*/		
}
