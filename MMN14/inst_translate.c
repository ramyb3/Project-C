#include "project.h"

int checkinst(char *token, int j, int line, char *name)
{
	int i=0, c=0;
	char *t, temp[max_line], s[max_label];	
	
	FILE *f;
	
	strcpy(s,token);

	if(j==18||j==19)/*entry or extern commands*/
	{	
		FILE *fp;
		
		char str[max_line];
	
		for(;;c++)/*removes white char in the beginning of line*/
		if(isspace(s[c])==0)
		{
			t=strchr(s,s[c]);
			strcpy(s,t);							
			break;
		}				

		t=strtok(s," ");
		t=strtok(NULL,"\0");
		
		if(t!=NULL)	/*checks if command is valid*/
		return -1;				
			
		fp=fopen("symbol","r+");
		
		while(1)/*algorithem that checks if there's the same label for extern and entry*/
		{							
			fgets(temp,max_line,fp);/*gets line from "symbol" file*/
			
			if(feof(fp))
			break;
			
			c=0;			
			
			t=strtok(temp,"\t");						
			t=strtok(NULL,"\n");/*gets the label*/		
			
			/*copy the line number to str if there is a label in "symbol" file
			that isn't just "entry label"*/
			if(strcmp(t,s)==0&&j==18)
			strcpy(str,temp);			
			
			if(strcmp(temp,"0")!=0&&j==18)			
			c=1;
								
			if(strcmp(temp,"0")==0&&j==19)
			c=1;
			
			if(c==0)
			if(strcmp(t,s)==0)			
			{  
				printf("\nYOU CAN'T USE THE SAME LABEL FOR ENTRY AND EXTERN!! - %s\n",t);
				fclose(fp);
				return -2;/*another type of error*/	
			}				
		}
		
		while(i!=-1)/*loop to the end of "symbol" file*/		
		i=fgetc(fp);					
		
		if(j==18)/*if entry command*/
		{
			FILE *p;
					
			strcpy(temp,name);
			
			/*changing the extension of the source file with the same name*/	
			t=strtok(temp,".");
			strcat(t,".ent");
				
			p=fopen(t,"a");
			
			c=0;
			
			for(i=0;i<strlen(str);i++)/*checks if str not empty*/
			if(isspace(str[i])==0)
			{
				c=1;
				break;
			}
			
			/*adding entry label and fictional line number 1 to "symbol" file*/
			fprintf(fp,"1\t%s\n",s);
			
			/*adding entry label and line number to ".ent" file*/
			if(c==1&&strcmp(str,"1")!=0)
			fprintf(p,"%s\t%s\n",s,str);

			fclose(p);			
		}
		
		fclose(fp);

		return 0;/*if command valid*/
	}
	
	if(j==16) /*data command*/  
	{	
		int count=0, comma=0;		
		
		char minus='-', plus='+';
		
		if(s!=NULL)				
		for(i=0;i<strlen(s);i++)/*counts how many commas there is in this command*/
		if(s[i]==',')
		comma++;				
							
		t=strtok(s,",");
		
		if(t==NULL)/*checks if command is valid*/
		return -1;

		count++;/*count how many numbers there are in this command*/
		
		f=fopen(name,"a");
							
		while(t!=NULL)/*while i didn't come to end of line*/
		{	
			i=checkspace(t);/*check if there isn't another input except white char*/

			if(i==-1)
			{
				fclose(f);
				return -1;/*if command invalid*/
			}			
			
			c=0;		
			j=0;

			for(i=0;i<strlen(t);i++) /*checks validation of input number*/
			if(isspace(t[i])==0)
			if(isdigit(t[i])==0&&t[i]!=minus&&t[i]!=plus)
			{							
				c=1;
				break;
			}
		
			for(i=0;i<strlen(t);i++)/*checks validation of input number*/
			if(isspace(t[i])==0)
			{							
				if((t[i]==minus||t[i]==plus)&&isdigit(t[i+1])==0)
				break;

				j=1;
				break;
			}

			/*checks if there isn't more minus or plus signs than should be*/
			for(i=i+1;i<strlen(t);i++)
			if(t[i]==minus||t[i]==plus)
			{							
				c=1;
				break;
			}
			
			if(j==0||c==1)
			{
				fclose(f);
				return -1;/*if command invalid*/
			}
												
			i=atoi(t);/*gets the input number*/
			
			if(strchr(t,minus)!=NULL)/*check if the number is negative*/
			{					
				for(i=0;;i++)/*gets the number after minus sign*/
				if(isdigit(t[i])!=0)
				{
					t=strchr(t,t[i]);
					break;
				}
				
				i=atoi(t);
					
				if(i>(max_num/2))/*checks if the number valid*/
				{
					fclose(f);
					return -1;/*if command invalid*/					
				}
				
				c=1;	
				i=max_num-i;/*get the final number according to two's complement*/
				
				/*adding number and line number and A sign to ".ob" file*/	
				fprintf(f,"%d\t%X\tA\n",line,i);
			}
				
			if(c==0)/*if the number is positive*/
			{
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
			}			

			t=strtok(NULL,",");/*gets the next number*/			

			if(t!=NULL)/*if i didn't come to end of line*/
			{
				count++;/*counter of numbers +1*/
				line++;/*line number +1*/
			}						
		}
		
		fclose(f);
		
		/*checks if the number of commas are the same or more than the numbers in line*/
		if(comma>=count)
		return -1;/*if command invalid*/
						
		return 0;/*if command valid*/
	}
	
	if(j==17)/*string command*/		
	{	
		char quotation='"';

		for(i=0;i<strlen(token);i++)/*count how many '"' there are in input*/
		if(token[i]==quotation)
		c++;

		if(c!=2)/*check if there isn't 2 quotation in the line*/
		return -1;/*if command invalid*/

		t=strtok(s,"\"");/*gets the inputed string*/

		if(token[0]!=quotation)/*checks validation of string if first char isn't '"'*/
		{
			for(i=0;i<strlen(t);i++)		
			if(isspace(t[i])==0)
			return -1;/*if command invalid*/
		
			for(i=0;i<strlen(t);i++)
			if(isspace(t[i])!=0)
			{			
				t=strtok(NULL,"\"");
				break;							
			}
		}
		
		if(strtok(NULL,"\0")!=NULL)	/*checks if command is valid*/
		return -1;/*if command invalid*/
		
		f=fopen(name,"a");
		
		for(i=0;i<=strlen(t);i++, line++)
		{
			c=t[i];/*gets the value of char in ascii*/
			
			/*adding number and line number and A sign to ".ob" file*/
			/*adding 0 in fprintf for good representation in hexa*/
			
			if(c<16&&c>-1)
			fprintf(f,"%d\t00%X\tA\n",line,c);
		
			if(c>15)
			fprintf(f,"%d\t0%X\tA\n",line,c);
		}
		
		fclose(f);
		return 0;/*if command valid*/
	}
	
	return -1;/*if command invalid*/	
}
