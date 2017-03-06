%option noyywrap
%{
	#include <string.h>
	#include <stdlib.h>
	#include "a5.tab.h"
	int SynErr;
	FILE* write_file1;
%}
%s sin
%s n1
%s n2
%%
<INITIAL>([a-hj-uw-z]|[A-HJ-UW-Z])([0-9]|[a-z]|[A-Z]|[_])* {
 	yylval.stri = strdup(yytext);
 //	fprintf(write_file1,"%s \n",yylval.stri);
 		BEGIN n1;
		return NAME;
	}		
<INITIAL>([1-9]([0-9])*(F|P|N|U|M|K|"MEG")?"H") {
		yylval.stri = strdup(yytext);
	//	fprintf(write_file1,"%s \n",yylval.stri);
		return IND;
	}

<INITIAL>([1-9]([0-9])*(F|P|N|U|M|K|"MEG")?"F") {
	yylval.stri = strdup(yytext);
//	fprintf(write_file1,"%s \n",yylval.stri);
	return CAP;
	}
<INITIAL>([1-9]([0-9])*(P|N|U|M|K|"MEG")?) {
	yylval.stri = strdup(yytext);
	//fprintf(write_file1,"%s \n",yylval.stri);
	return RES;
	}	
<n1>([a-zA-Z])([0-9]|[a-z]|[A-Z]|[_])* {
	yylval.stri = strdup(yytext);
//	fprintf(write_file1,"%s \n",yylval.stri);
	BEGIN n2;
	return NET ;
	  }
<n1>"0" {
	yylval.stri = strdup(yytext);
//	fprintf(write_file1,"%s \n",yylval.stri);
	BEGIN n2;
	return NET;
	}
<n2>([a-zA-Z])([0-9]|[a-z]|[A-Z]|[_])* {
	yylval.stri = strdup(yytext);
//	fprintf(write_file1,"%s \n",yylval.stri);
	BEGIN INITIAL;
	return NET ;
	  }
<n2>"0" {
	yylval.stri = strdup(yytext);
//	fprintf(write_file1,"%s \n",yylval.stri);
	BEGIN INITIAL;
	return NET;
	}	

<sin>[0-9]+(\.[0-9]+)? {
	char* numb = strdup(yytext);	
    yylval.stri = numb;	
 //   fprintf(write_file1,"%s \n",yylval.stri);
	return NUMBER;
	}
<INITIAL>(V|v)([0-9]|[a-z]|[A-Z]|[_])* {
    yylval.stri = strdup(yytext);	
  //  fprintf(write_file1,"%s \n",yylval.stri);
  	BEGIN n1;
	return VOLT;
}
<INITIAL>(I|i)([0-9]|[a-z]|[A-Z]|[_])* {
    yylval.stri = strdup(yytext);
  //  fprintf(write_file1,"%s \n",yylval.stri);
  	BEGIN n1;	
	return CURRENT;
}
<INITIAL>"SINE (" {
	BEGIN sin;	
//	fprintf(write_file1,"%s \n",yytext);
	return SINE;
}
<sin>[0-9]+(\.[0-9]+)?"S" {
	yylval.stri = strdup(yytext);
//	fprintf(write_file1,"%s \n",yylval.stri);
	return DELAY;
	}
<sin>[0-9]+((\.[0-9]+)?)((P|N|U|M|K|"MEG")?)"hz" {
	char* numb = strdup(yytext);	
    yylval.stri = numb;	
 //   fprintf(write_file1,"%s \n",yylval.stri);
	return FREQ;
	}	
<sin>")" {
		BEGIN INITIAL;
	//	fprintf(write_file1,")\n");		
		return CP;
	}
<INITIAL>\n 	{//	fprintf(write_file1," NEWLINE \n");
	return EOL;
	}
<INITIAL>[ \t\r]+ {;//fprintf(write_file1,"SPACE \n");
}	
<INITIAL>.	{
		SynErr = 1;
		fprintf(write_file1,"%s error \n",yytext);
	throw_err1();
	}
<sin>[ \t\r]+ {//fprintf(write_file1,"space \n");
}	
<sin>.	{
		SynErr = 1;
		fprintf(write_file1,"%s error \n",yytext);
	throw_err1();
		}	
<n1>[ \t\r]+ {//fprintf(write_file1,"space \n");
}	
<n1>.	{
		SynErr = 1;
		fprintf(write_file1,"%s error \n",yytext);
	throw_err1();
	}
<n2>[ \t\r]+ {//fprintf(write_file1,"space \n");
}	
<n2>.	{
		SynErr = 1;
		fprintf(write_file1,"%s error \n",yytext);
	throw_err1();
	}		

%%;
