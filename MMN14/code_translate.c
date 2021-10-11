#include "project.h"

		/*all commands funct and opcode in order first-mov, last-stop*/
int command[16][2]={{0,0},{1,0},{2,10},{2,11},{4,0},{5,10},{5,11},{5,12},
				{5,13},{9,10},{9,11},{9,12},{12,0},{13,0},{14,0},{15,0}};
FILE *f;

int byte, temp1, temp2;

/*translate commands mov, cmp, add, sub, lea*/					
int arithmetic(char *token, int i, int line, char *name)
{	
	char *t, t1[max_label], t2[max_label], s[max_label];
		
	struct word ar;
	
	/*gets two first bytes according to command array*/	
	ar.opcode=command[i][0];
	ar.funct=command[i][1];

	strcpy(s,token);
	t=strtok(s,",");/*gets first argument*/

	temp1=checkspace(t);/*check if there isn't another input except white char*/

	if(temp1==-1)
	return -1;
	
	strcpy(t1,t);	

	temp1=method(t1);/*gets the number of the addressing method*/
	ar.from=temp1;	

	if(i==4)/* Addressing method that valid- 1 for lea command */
	if(temp1!=1)/*check if Addressing method valid*/
	return -1;

	if(i!=4)/* Addressing method that valid- 0 or 1 or 3  */
	if(temp1==2||temp1==-1)/*check if Addressing method valid*/
	return -1;
	
	t=strtok(NULL,"\0");/*gets second argument*/
	
	if(t==NULL)	/*if argument invalid*/
	return -1;	

	strcpy(t2,t);

	temp1=method(t2);/*gets the number of the addressing method*/
	ar.address=temp1;	

	if(i==1)	/* Addressing method that valid- 0 or 1 or 3 for cmp command */
	if(temp1==-1||temp1==2)/*check if Addressing method valid*/
	return -1;
	
	if(i!=1)	/* Addressing method that valid- 1 or 3  */
	if(temp1<1||temp1==2)/*check if Addressing method valid*/
	return -1;
		
	temp1=checkspace(t);/*check if there isn't another input except white char*/

	if(temp1==-1)
	return -1;

	byte=lastbyte(ar.from,ar.address);/*calculate last byte in word*/
	
	f=fopen(name,"a");
	
	/*adding word and line number and A sign to ".ob" file*/
	fprintf(f,"%d\t%X%X%X\tA\n",line,ar.opcode,ar.funct,byte);
	fclose(f);
	
	/*this note meant to the 6 functions below that*/
	/*calling to functions according to the addressing method*/
	if(ar.from==0)
	temp1=number(t1,line+1,name);

	if(ar.from==1)
	temp1=address(t1,line+1,name);
	
	if(ar.from==3)
	getreg(t1,line+1,name);
	
	if(ar.address==0)
	temp2=number(t2,line+2,name);
	
	if(ar.address==1)
	temp2=address(t2,line+2,name);		
			
	if(ar.address==3)	
	getreg(t2,line+2,name);
	
	if(temp1==-1||temp2==-1)/*if one or both arguments invalid*/
	return -1;
	
	return 0;/*if command valid*/
}

/*translate commands clr, not ,inc ,dec, red, prn*/
int operation(char *token, int i, int line, char *name)
{	
	char t1[max_label];

	struct word op;
	
	/*gets two first bytes according to command array*/
	op.opcode=command[i][0];
	op.funct=command[i][1];
	op.from=0; /*addressing method in 2 first bits of last byte is 0*/
	
	if(token==NULL)/*if argument invalid*/
	return -1;

	temp1=checkspace(token);/*check if there isn't another input except white char*/
	
	if(temp1==-1)
	return -1;
	
	strcpy(t1,token);	

	temp1=method(t1);/*gets the number of the addressing method*/
	op.address=temp1;
	
	if(i==13)/* Addressing method that valid- 0 or 1 or 3 for prn command */
	if(temp1==2||temp1==-1)/*check if Addressing method valid*/
	return -1;
	
	if(i!=13)/* Addressing method that valid- 1 or 3  */
	if(temp1==2||temp1<1)/*check if Addressing method valid*/
	return -1;
	
	byte=lastbyte(op.from,op.address);/*calculate last byte in word*/
	
	f=fopen(name,"a");
	
	/*adding word and line number and A sign to ".ob" file*/
	fprintf(f,"%d\t%X%X%X\tA\n",line,op.opcode,op.funct,byte);
	fclose(f);
	
	/*this note meant to the 3 functions below that*/
	/*calling to functions according to the addressing method*/
	if(op.address==0)
	temp1=number(t1,line+1,name);
	
	if(op.address==1)
	temp1=address(t1,line+1,name);
	
	if(op.address==3)	
	getreg(t1,line+1,name);
	
	if(temp1==-1)/*if argument invalid*/
	return -1;		
	
	return 0;/*if command valid*/	
}

/*translate commands jmp, bne, jsr*/
int labels(char *token, int i, int line, char *name)
{	
	char t1[max_label];

	struct word la;
	
	/*gets two first bytes according to command array*/
	la.opcode=command[i][0];
	la.funct=command[i][1];
	la.from=0;/*addressing method in 2 first bits of last byte is 0*/
	
	if(token==NULL)/*if argument invalid*/
	return -1;

	temp1=checkspace(token);/*check if there isn't another input except white char*/
	
	if(temp1==-1)
	return -1;
	
	strcpy(t1,token);	

	temp1=method(t1);
	la.address=temp1;/* Addressing method that valid- 1 or 2  */

	if(temp1>2||temp1<1)/*check if Addressing method valid*/
	return -1;

	byte=lastbyte(la.from,la.address);/*calculate last byte in word*/

	f=fopen(name,"a");
	
	/*adding word and line number and A sign to ".ob" file*/
	fprintf(f,"%d\t%X%X%X\tA\n",line,la.opcode,la.funct,byte);
	fclose(f);
		
	/*this note meant to the 2 functions below that*/
	/*calling to functions according to the addressing method*/
	if(la.address==1)
	temp1=address(t1,line+1,name);
	
	if(la.address==2)	
	temp1=percentage(t1,line,line+1,name);	
	
	if(temp1==-1)/*if argument invalid*/	
	return -1;		
	
	return 0;/*if command valid*/	
}

/*translate commands rts, stop*/
int program(char *token, int i, int line, char *name)
{					
	struct word pr;
	
	/*gets two first bytes according to command array, last byte is 0*/
	pr.opcode=command[i][0];
	pr.funct=command[i][1];
	pr.from=0;
	pr.address=0;

	byte=lastbyte(pr.from,pr.address);/*calculate last byte in word*/

	f=fopen(name,"a");
	
	/*adding word and line number and A sign to ".ob" file*/	
	fprintf(f,"%d\t%X%X%X\tA\n",line,pr.opcode,pr.funct,byte);
	fclose(f);
	
	return 0;
}				
