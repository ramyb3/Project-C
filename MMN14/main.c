#include "project.h"

int main (int argc, char *argv[])/* get arguments from user */
{	
	int loop;/*counter for ".as" programs to translate*/
	
	if (argc<2)/*checks if there is arguments in command*/
 	{
		printf("ERROR!!\nPUT ARGUMENTS IN COMMAND LINE!\n");
 		exit(0);
 	}
	
	registers();/*declaring all the registers in word type*/
			
	for(loop=1;loop<argc;loop++)/*loop for all arguments in command line*/
	{	
		char argument1[max_line], argument2[max_line], argument3[max_line],
		 	 argument4[max_line];
		
		int error=0;/*flag for errors in the project*/
		
		/*naming all files with all extensions*/	
		strcpy(argument1,argv[loop]);
		strcpy(argument2,argument1);
		strcpy(argument3,argument1);
		strcpy(argument4,argument1);
		
		strcat(argument1,".as");
		strcat(argument2,".ob");
		strcat(argument3,".ext");
		strcat(argument4,".ent");		
	
		if(fopen(argument1,"r")==NULL)/*checks if there's a file with ".as"*/
		{
			printf("\nERROR!!\nTHERE ISN'T SUCH FILE - \"%s\"\n",argument1);
			puts("---------------------------------------------------------------");
	 		error=1;
	 	}
	
		if(error==0)/*if there isn't an error*/
		{	
			char s[max_line], temp[max_line], line[max_line], str[max_line], *t;	
			char colon=':', comma=',';
	
			FILE *fp, *p, *f, *pt, *pp, *ff;
			
			/*IC- line counter, DC- line counter for string and data,
			COUNTER- line counter without string & data & entry & extern */
			int IC=100, DC=0, COUNTER=0, i, j, x; 	
		
			p=fopen(argument1,"r");
			pt=fopen("commands","w");/* commands file + number of line */	
			fp=fopen("symbol","w");/* label file + number of line */
	
			while(1)/*checking 1st time the input file*/
			{
				int ok=0, count1=0, count2=0;
				
				fgets(s,max_line,p);/*gets the line from given file*/
		
				if(feof(p))
				break;
		
				for(j=0;j<strlen(s);j++)/* change '\t' to ' ' in line */
				if(s[j]==9)/* TAB==9 in ascii */
				s[j]=32;/* SPACE==32 in ascii */

				strcpy(str,s);
				
				x=0;
				
				for(i=0;i<strlen(s);i++)
				if(strstr(s,"string")!=NULL)
				if('"'==s[i])
				{					
					x=i;
					break;									
				}
				
				ok=checkempty(s,ok);/*check if line empty*/

				if(ok!=-2)/*if line not empty*/									
				ok=checknote(s,ok,x);/*check if the input is note*/

				if(ok>-1)/*if the line valid and not empty*/
				{	
					x=0;
								
					for(i=0;i<strlen(s);i++)
					{												
						if(strstr(s,"string")!=NULL)/*check if there is string in line*/
						if('"'==s[i])
						x++;
						
						/*making sure i don't check the input in string*/
						if(colon==s[i]&&(x==0||x==2)) /*check colon in line (labels)*/	
						{	
							if(s[0]==colon)/*':' can't be the first char in line*/
							{
								ok=-1;
								break;
							}
							
							ok=-1;
							
							/*check if colon attached to label*/						
							if((isspace(s[i-1])==0)&&(isspace(s[i+1])!=0))
							ok=1; 
						
							count1++;/*count how many colons there in line*/
						}
				
						/*make sure i don't check the input in string*/
						if(comma==s[i]&&(x==0||x==2))  /* check comma in line */
						count2++;/*count how many commas there in line*/ 				     
					}
					
					if(ok==1&&count1==1) /* if there is labels */
					{
						for(j=0;;j++)
						if(isspace(s[j])==0)/*removes white char from beginning of line*/
						{
							t=strchr(s,s[j]);						
							break;
						}			
				
						t=strtok(t,":");/*gets the label*/								
						strcpy(temp,t);
				
						ok=checklabel1(temp,str);/*check if the input label is valid*/
						
						if(ok==-2)/*another type of error*/
						error=1;
												
						if(ok==1)/*if line valid*/
						{
							t=strtok(NULL," ");/*gets the command*/		

							if(strcmp(t,"\n")==0)/*check if line not empty*/
							ok=-1;
								
							if(ok!=-1)
							{
								strcpy(line,t);
				
								t=strtok(NULL,"\n");/*get the arguments in line*/

								/*checks input command line first time*/	
								if(t!=NULL)/*if there is arguments*/
								x= checkcommand(line,count2,t);

								if(t==NULL)/*if there isn't arguments*/
								{									
									t=strtok(line,"\n");
									x= checkcommand(t,count2,NULL);		
								}

							/*puting together the command without label if it was changed*/
								if(strcmp(t,line)!=0)
								{
									strcpy(s," ");
									strcat(line,s);
									strcat(line,t);
								}
					
								/*checks if command valid according to the syntax*/
								if((x==3&&count2!=1)||(x>0&&x<3&&count2>0)||x<0)
								ok=-1;				
				
								if(x>9)/* entry||extern||string||data */
								x=x-10;
							}

							if(ok!=-1)
							{
								if(x>0&&x!=100)/*not entry and not extern*/
								{
									/*adding label and line number to "symbol" file*/		
									fprintf(fp,"%d\t%s\n",IC,temp);	
									
									/*adding command and line number to "commands" file*/
									fprintf(pt,"%d\t%s\n",IC,line);
									
									IC=IC+x;/*calculates the next command line*/
								}																											
							}
						}
					}
		
					if(ok==0&&count1==0)/* when the line isn't note or empty or label */
					{
						strcpy(line,s);
						
						t=strtok(s," ");/*get command*/				
						strcpy(temp,t);				
								
						t=strtok(NULL,"\n");/*get arguments*/
				
						/*checks input command line first time*/
						if(t!=NULL)	/*if there is arguments*/		
						x= checkcommand(temp,count2,t);
							
						if(t==NULL)/*if there isn't arguments*/
						{				
							t=strtok(temp,"\n");					
							x= checkcommand(t,count2,NULL);
						}

						/*checks if command valid according to the syntax*/
						if((x==3&&count2!=1)||(x>0&&x<3&&count2>0)||x<0)
						ok=-1;
								
						if(x>9)/* entry||extern||string||data */
						x=x-10;				
				
						if(ok!=-1)
						{	
							if(x>0&&x!=100)/*not entry and not extern*/
							{
								/*adding command and line number to "commands" file*/
								fprintf(pt,"%d\t%s",IC,str);
								
								IC=IC+x;/*calculates the next command line*/
							}																						
						}							
					}					
					
					if((ok==1&&count1==1)||(ok==0&&count1==0))
					{
						/*check if there is a data or string command in input*/
						if(strstr(str,"data")!=NULL||strstr(str,"string")!=NULL)
						DC=DC+x;/*calculates how many there are string and data lines*/
						
					/*check if there isn't data & string & entry & extern commands in input*/
						if(strstr(str,"data")==NULL&&strstr(str,"string")==NULL&&
						strstr(str,"extern")==NULL&&strstr(str,"entry")==NULL)
						COUNTER=COUNTER+x;/*calculates how many there are command lines*/
						
						/*adding command and line number- 0 to "commands" file*/
						if(x==0||x==100)
						{
							if(count1==1)/*if there's a label*/
							fprintf(pt,"0\t%s\n",line);
							
							if(count1==0)/*if there isn't label*/
							fprintf(pt,"0\t%s",line);							
						}
						
						if(x==0)/*extern command*/
						{
							for(i=0;;i++)
							if(isspace(t[i])==0)/*removes white char from beginning of line*/
							{
								t=strchr(t,t[i]);			
								break;
							}
							
							t=strtok(t," ");/*get label*/	
									
							/*adding label and line number- 0 to "symbol" file*/
							fprintf(fp,"0\t%s\n",t);						
						}		
					}
				}
				
				if(ok==-1||count1>1)/*if the command valid*/
				{
					error=1;			
					printf("\nERROR IN LINE:\n%s\n",str);
				}					 
			}      
			
			/*closing "symbol" & "commands" & ".as" files*/
			fclose(p);
			fclose(pt);
			fclose(fp);

			if(error==0)/*if there isn't errors till now*/
			{	
				/*creating 3 files with the same name but with different extensions*/		
				pp=fopen(argument3,"w");
				fclose(pp);
				
				ff=fopen(argument4,"w");
				fclose(ff);
				
				f=fopen(argument2,"w");
				
				/*if there isn't any commands in ".as" file*/
				if(COUNTER!=0||DC!=0)
				fprintf(f,"%d\t%d\n",COUNTER,DC);/*adding to ".ob" file number of commands*/

				fclose(f);
		
				error=checksyntax(s,str,argument2);/*checks 2nd time the input line*/		
			}	 	 			

			if(error==0)/*if there isn't errors till now*/		
			error=checklabel2(s,temp,line);/*check if there isn't problem with the labels*/
			
			/*removing temp files*/		 	  					
			remove("symbol");
			remove("commands");					
			
			/*this note meant to the 3 if below that*/
			/*if the files empty they will be deleted*/
			if(fopen(argument2,"r")!=NULL)
			{
				f=fopen(argument2,"r");
				i=fgetc(f);

				if(i==-1)
				remove(argument2);
			}
			
			if(fopen(argument3,"r")!=NULL)
			{
				pp=fopen(argument3,"r");
				i=fgetc(pp);

				if(i==-1)
				remove(argument3);
			}
			
			if(fopen(argument4,"r")!=NULL)
			{
				ff=fopen(argument4,"r");
				i=fgetc(ff);
			
				if(i==-1)
				remove(argument4);
			}
				
			if(error==1)/*if there is error somewhere all files will be deleted*/
			{	
				printf("\nALL ERRORS IN \"%s\" ARE ABOVE\n",argument1);
				puts("---------------------------------------------------------------");
			
				if(fopen(argument3,"r")!=NULL)
				remove(argument3);
					
				if(fopen(argument4,"r")!=NULL)
				remove(argument4);	
				
				if(fopen(argument2,"r")!=NULL)	
				remove(argument2);	
			}			
		}
	}
	  
return 0;
}	
