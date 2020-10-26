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

 
%}

 /*
  * Define names for regular expressions here.
  */

%option noyywrap
%option yymore

%x LINE_COMMENT MULTILINE_COMMENT STRING

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

<INITIAL>\" {
  BEGIN STRING;
  yymore();
}

<STRING><<EOF>> {
  strcpy(seal_yylval.error_msg,"EOF in srting");
  BEGIN 0;
  return (ERROR);
}

<STRING>\\\n {
  curr_lineno++;
  yymore();
}

<STRING>\n {
  curr_lineno++;
  strcpy(seal_yylval.error_msg,"start new line without '\'");
  BEGIN 0;
  return (ERROR);
}
 
 /*zai kan */
<STRING>[^\"] {yymore();}

<STRING>\\0 {
  strcpy(seal_yylval.error_msg,"string contains ASCII 0x00");
  return (ERROR);
}

 /* escape character */
<STRING>\\[^0] {yymore();}

<STRING>\" {

  
  seal_yylval.symbol=stringtable.add_string(yytext);
  BEGIN 0;
  return (CONST_STRING);
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

[A-Z][A-Za-z0-9_]* {
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