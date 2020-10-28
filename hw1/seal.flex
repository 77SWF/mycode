 /*
  *  The scanner definition for seal.
  */

 /*
  *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
  *  output, so headers and global definitions are placed here to be visible
  * to the code in the file.  Don't remove anything that was here initially
  */
%{

#include <seal-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>

/* The compiler assumes these identifiers. */
#define yylval seal_yylval
#define yylex  seal_yylex

/* Max size of string constants */
#define MAX_STR_CONST 256
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the seal compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE seal_yylval;

 /*
 *  Add Your own definitions here
 */
int string_len;
bool contain_null;
 
%}

 /*
  * Define names for regular expressions here.
  */

%option noyywrap
%option yymore

%x LINE_COMMENT MULTILINE_COMMENT STRING AS_STRING

DIGIT [0-9]

%%

 /*	
 *	Add Rules here. Error function has been given.
 */


 /*================
 *new line/whitespace
 *=================
 */

\n {curr_lineno++;}
[ \f\r\t\v]+ {}


 /*======
 *comment 
 *=======
 */

 /* multiline comment */
<INITIAL>"/*" { BEGIN MULTILINE_COMMENT; }
<MULTILINE_COMMENT>\n { curr_lineno++; }
<MULTILINE_COMMENT><<EOF>> {
  strcpy(seal_yylval.error_msg, "EOF in comment"); 
  BEGIN 0;
  return (ERROR);
}
<MULTILINE_COMMENT>"*/" {BEGIN 0;}

"*/" {
  strcpy(seal_yylval.error_msg, "no matched character"); 
  return (ERROR);
}
<MULTILINE_COMMENT>. {}

 /* single-line comment */
<INITIAL>"//" {BEGIN LINE_COMMENT;}
<LINE_COMMENT>\n {
  curr_lineno++;
  BEGIN 0;
  }
<LINE_COMMENT>. {}


 /*============
 *CONST_STRIING
 *=============
 */

 /* " " */

<INITIAL>\" {
  memset(string_buf,0,MAX_STR_CONST);
  string_len=0;
  contain_null=false;
  BEGIN STRING;
}

<STRING><<EOF>> {
  strcpy(seal_yylval.error_msg,"EOF in srting");
  BEGIN 0;
  return (ERROR);
}

<STRING>\\0 {
  string_buf[string_len++]=0;
  contain_null=true;
}

<STRING>\\. {
  if(string_len>=MAX_STR_CONST){
    strcpy(seal_yylval.error_msg,"string longer than 256.");
    BEGIN 0;
    return (ERROR);
  }

  switch(yytext[1]){
    case '\\':string_buf[string_len++]='\\';break;
    case '\"':string_buf[string_len++]='\"';break;
    case 'n':string_buf[string_len++]='\n';break;
    case 't':string_buf[string_len++]='\t';break;
    case 'b':string_buf[string_len++]='\b';break;
    case 'f':string_buf[string_len++]='\f';break;
    default:string_buf[string_len++]=yytext[1];
  }
}

<STRING>\\\n {
  curr_lineno++;
  string_buf[string_len++]='\n';
}

<STRING>\n {
  curr_lineno++;
  strcpy(seal_yylval.error_msg,"newline in quotation must use a '\\'");
  BEGIN 0;
  return (ERROR);
}

<STRING>\" {
  if(contain_null==true){
    strcpy(seal_yylval.error_msg,"String contain null character '\\0'");
    BEGIN 0;
    return (ERROR);
    }

  if(string_len>=MAX_STR_CONST){
    strcpy(seal_yylval.error_msg,"string longer than 256.");
    BEGIN 0;
    return (ERROR);
  }
  
  seal_yylval.symbol=stringtable.add_string(string_buf);
  BEGIN 0;
  return (CONST_STRING);
}

<STRING>. {
  if(string_len>=MAX_STR_CONST){
    strcpy(seal_yylval.error_msg,"string longer than 256.");
    BEGIN 0;
    return (ERROR);
  }
  string_buf[string_len++]=*yytext;
}

 /* `` */

<INITIAL>\` {
  memset(string_buf,0,MAX_STR_CONST);
  string_len=0;
  contain_null=false;
  BEGIN AS_STRING;
}

<AS_STRING><<EOF>> {
  strcpy(seal_yylval.error_msg,"EOF in srting");
  BEGIN 0;
  return (ERROR);
}

<AS_STRING>\\ {
  string_buf[string_len++]='\\';
}

<AS_STRING>\n {
  string_buf[string_len++]=*yytext;
  curr_lineno++;
}

<AS_STRING>\` {
  if(string_len>=MAX_STR_CONST){
    strcpy(seal_yylval.error_msg,"string longer than 256.");
    BEGIN 0;
    return (ERROR);
  }
  
  seal_yylval.symbol=stringtable.add_string(string_buf);
  BEGIN 0;
  return (CONST_STRING);
}

<AS_STRING>. {
  if(string_len>=MAX_STR_CONST){
    strcpy(seal_yylval.error_msg,"string longer than 256.");
    BEGIN 0;
    return (ERROR);
  }
  string_buf[string_len++]=*yytext;
}


 /*=======
 *keywords
 *========
 */

if {return IF;}
else {return ELSE;} 
while {return WHILE;} 
for {return FOR;} 
break {return BREAK;} 
continue {return CONTINUE;} 
func {return FUNC;} 
return {return RETURN;} 
var {return VAR;} 


 /*========
 *CONST_INT 
 *=========
 */

{DIGIT}+ {
  seal_yylval.symbol=inttable.add_string(yytext);
  return (CONST_INT);
}

0x[0-9a-fA-F]+ {
  int hex_int=strtol(yytext,NULL,16);
  seal_yylval.symbol=inttable.add_int(hex_int);
  return (CONST_INT);
}


 /*========
 *CONST_FLOAT
 *=========
 */

(0|[1-9][0-9]*)"."([0-9]+) {
  seal_yylval.symbol=floattable.add_string(yytext);
  return (CONST_FLOAT);
}


 /*========
 *CONST_BOOL
 *=========
 */
false {
  seal_yylval.boolean=0;
  return (CONST_BOOL);
}
true {
  seal_yylval.boolean=1;
  return (CONST_BOOL);
}


 /*========
 *TYPEID
 *=========
 */

Int|Float|String|Bool|Void {
  seal_yylval.symbol=idtable.add_string(yytext);
  return TYPEID;
}


 /*========
 *OBJECTID
 *=========
 */

[a-z][A-Za-z0-9_]* {
  seal_yylval.symbol=idtable.add_string(yytext);
  return OBJECTID;
}


 /*========
 *operators 
 *=========
 */

"+" {return '+';}
"-" {return '-';}
"*" {return '*';}
"/" {return '/';}
"%" {return '%';}
"&" {return '&';}
"|" {return '|';}
"^" {return '^';}
"~" {return '~';}
"=" {return '=';}
">" {return '>';}
">=" {return GE;}
"==" {return EQUAL;}
"!=" {return NE;}
"<=" {return LE;}
"<" {return '<';}
"&&" {return AND;}
"||" {return OR;}
"!" {return '!';}
"," {return ',';}
";" {return ';';}
":" {return ':';}
")" {return ')';}
"(" {return '(';}
"{" {return '{';}
"}" {return '}';}

 
 /*====
 *ERROR 
 *=====
 */
.	{
	strcpy(seal_yylval.error_msg, yytext); 
	return (ERROR); 
}

%%