/*********************************************************************
Name: Ryan Michael Curry NetID: rmc580
Course: CSE 4714 Project 01
Programming Environment: Ubuntu 22.04 in WSL2
Purpose of File: Rules for the TIPS language
*********************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

// global variable to hold current line number being read
int line_number = 1;
%}

%%



 /* Keyword Lexemes */

"BEGIN" { return TOK_BEGIN; }
"BREAK" { return TOK_BREAK; }
"CONTINUE" { return TOK_CONTINUE; }
"DOWNTO" { return TOK_DOWNTO; }
"ELSE" { return TOK_ELSE; }
"END" { return TOK_END; }
"FOR" { return TOK_FOR; }
"IF" { return TOK_IF; }
"LET" { return TOK_LET; }
"PROGRAM" { return TOK_PROGRAM; }
"READ" { return TOK_READ; }
"THEN" { return TOK_THEN; }
"TO" { return TOK_TO; }
"VAR" { return TOK_VAR; }
"WHILE" { return TOK_WHILE; }
"WRITE" { return TOK_WRITE; }


 /* Datatype Specifier Lexemes */

"INTEGER" { return TOK_INTEGER; }
"REAL" { return TOK_REAL; }


 /* Punctuation Lexemes */

[;] { return TOK_SEMICOLON; }
[:] { return TOK_COLON; }
[(] { return TOK_OPENPAREN; }
[)] { return TOK_CLOSEPAREN; }


 /* Operator Lexemes */

"+" { return TOK_PLUS; }
"-" { return TOK_MINUS; }
"*" { return TOK_MULTIPLY; }
"/" { return TOK_DIVIDE; }
":=" { return TOK_ASSIGN; }
"=" { return TOK_EQUALTO; }
"<" { return TOK_LESSTHAN; }
">" { return TOK_GREATERTHAN; }
"<>" { return TOK_NOTEQUALTO; }
"MOD" { return TOK_MOD; }
"NOT" { return TOK_NOT; }
"OR" { return TOK_OR; }
"AND" { return TOK_AND; }


 /* Useful Abstraction Lexemes */

[A-Z][0-9A-Z]{8,} { return TOK_UNKNOWN; }
[A-Z][0-9A-Z]{0,7} { return TOK_IDENT; }
[0-9][0-9]*"."[0-9][0-9]* { return TOK_FLOATLIT; }
[0-9]* { return TOK_INTLIT; }
['][^'\n']{80,}['] { return TOK_UNKNOWN; }
['][^'\n]*['] { return TOK_STRINGLIT; }

 /* Ignore Comments */
[{][^}]*[}] {  }

[\n]  { line_number++; }

[ \t\r]+             /* nop */

 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }

