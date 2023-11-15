//**************************************************************************

/*********************************************************************
Name: Ryan Michael Curry NetID: rmc580
Course: CSE 4714 Project 02
Programming Environment: Ubuntu 22.04 in WSL2
Purpose of File: Parsing TIPS
*********************************************************************/

//**************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"

using namespace std;

#define TYPE 10

int nextToken = 0; // hold nextToken returned by lex

// Which tree level are we currently in?
static int level = 0;

// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse
set<string> symbolTable; // Symbol Table

//*****************************************************************************
// Indent to reveal tree structure
string psp(void)
{ // Stands for p-space, but I want the name short
  string str("");
  for (int i = 0; i < level; i++)
    str += "|  ";
  return str;
}
//*****************************************************************************
// Report what we found
void output(string what)
{
  cout << psp() << "found |" << yytext << "| " << what << endl;
}

int lex(bool getNext = true)
{
  char *token_str;

  if (getNext)
  {
    nextToken = yylex();
  }

  switch (nextToken)
  {
  case TOK_BEGIN:
    token_str = (char *)"BEGIN";
    break;
  case TOK_BREAK:
    token_str = (char *)"BREAK";
    break;
  case TOK_CONTINUE:
    token_str = (char *)"CONTINUE";
    break;
  case TOK_DOWNTO:
    token_str = (char *)"DOWNTO";
    break;
  case TOK_ELSE:
    token_str = (char *)"ELSE";
    break;
  case TOK_END:
    token_str = (char *)"END";
    break;
  case TOK_FOR:
    token_str = (char *)"FOR";
    break;
  case TOK_IF:
    token_str = (char *)"IF";
    break;
  case TOK_LET:
    token_str = (char *)"LET";
    break;
  case TOK_PROGRAM:
    token_str = (char *)"PROGRAM";
    break;
  case TOK_READ:
    token_str = (char *)"READ";
    break;
  case TOK_THEN:
    token_str = (char *)"THEN";
    break;
  case TOK_TO:
    token_str = (char *)"TO";
    break;
  case TOK_VAR:
    token_str = (char *)"VAR";
    break;
  case TOK_WHILE:
    token_str = (char *)"WHILE";
    break;
  case TOK_WRITE:
    token_str = (char *)"WRITE";
    break;
  case TOK_INTEGER:
    token_str = (char *)"INTEGER";
    break;
  case TOK_REAL:
    token_str = (char *)"REAL";
    break;
  case TOK_SEMICOLON:
    token_str = (char *)"SEMICOLON";
    break;
  case TOK_COLON:
    token_str = (char *)"COLON";
    break;
  case TOK_OPENPAREN:
    token_str = (char *)"OPENPAREN";
    break;
  case TOK_CLOSEPAREN:
    token_str = (char *)"CLOSEPAREN";
    break;
  case TOK_PLUS:
    token_str = (char *)"PLUS";
    break;
  case TOK_MINUS:
    token_str = (char *)"MINUS";
    break;
  case TOK_MULTIPLY:
    token_str = (char *)"MULTIPLY";
    break;
  case TOK_DIVIDE:
    token_str = (char *)"DIVIDE";
    break;
  case TOK_ASSIGN:
    token_str = (char *)"ASSIGN";
    break;
  case TOK_EQUALTO:
    token_str = (char *)"EQUALTO";
    break;
  case TOK_LESSTHAN:
    token_str = (char *)"LESSTHAN";
    break;
  case TOK_GREATERTHAN:
    token_str = (char *)"GREATERTHAN";
    break;
  case TOK_NOTEQUALTO:
    token_str = (char *)"NOTEQUALTO";
    break;
  case TOK_MOD:
    token_str = (char *)"MOD";
    break;
  case TOK_NOT:
    token_str = (char *)"NOT";
    break;
  case TOK_OR:
    token_str = (char *)"OR";
    break;
  case TOK_AND:
    token_str = (char *)"AND";
    break;
  case TOK_UNKNOWN:
    token_str = (char *)"UNKNOWN";
    break;
  case TOK_IDENT:
    token_str = (char *)"IDENTIFIER";
    break;
  case TOK_FLOATLIT:
    token_str = (char *)"FLOATLIT";
    break;
  case TOK_INTLIT:
    token_str = (char *)"INTLIT";
    break;
  case TOK_STRINGLIT:
    token_str = (char *)"STRINGLIT";
    break;
  default:
    token_str = (char *)"UNKNOWN";
    break;
  }

  output(token_str);
  return nextToken;
}

int lex_throw(int token, bool getNext = true)
{
  int current_token = lex(getNext);
  switch (token)
  {
  case TOK_IDENT:
    if (current_token != TOK_IDENT)
      throw "2: identifier expected";
    break;
  case TOK_PROGRAM:
    if (current_token != TOK_PROGRAM)
      throw "3: 'PROGRAM' expected";
    break;
  case TOK_CLOSEPAREN:
    if (current_token != TOK_CLOSEPAREN)
      throw "4: ')' expected";
    break;
  case TOK_COLON:
    if (current_token != TOK_COLON)
      throw "5: ':' expected";
    break;
  case TOK_OPENPAREN:
    if (current_token != TOK_OPENPAREN)
      throw "6: '(' expected";
    break;
  case TYPE:
    if (current_token != TOK_INTEGER || current_token != TOK_REAL)
      throw "10: error in type";
    break;
  case TOK_END:
    if (current_token != TOK_END)
      throw "13: 'END' expected";
    break;
  case TOK_SEMICOLON:
    if (current_token != TOK_SEMICOLON)
      throw "14: ';' expected";
    break;
  case TOK_BEGIN:
    if (current_token != TOK_BEGIN)
      throw "17: 'BEGIN' expected";
    break;
  case TOK_ASSIGN:
    if (current_token != TOK_ASSIGN)
      throw "51: ':=' expected";
    break;
  case TOK_THEN:
    if (current_token != TOK_THEN)
      throw "52: 'THEN' expected";
    break;
  default:
    throw "999: an error has occurred";
    break;
  }

  return current_token;
}

//*****************************************************************************
// Forward declarations of FIRST_OF functions.  These check whether the current
// token is in the FIRST set of a production rule.
bool first_of_program(void);
bool first_of_block(void);
bool first_of_statement(void);
bool first_of_expression(void);
bool first_of_statement(void);
bool first_of_simple_exp(void);
bool first_of_term(void);
bool first_of_factor();
//*****************************************************************************
// Parses strings in the language generated by the rule:
void block();
void var();
void statement();

void assignment();
void compound_stmt();
void if_stmt();
void while_stmt();
void read();
void write();

void factor();
void term();
void simple_exp();
void expression();
//*****************************************************************************
// Parses strings in the language generated by the rule:
void factor()
{
  cout << psp() << "enter <factor>" << endl;
  ++level;

  if(!first_of_factor())
    throw "903: illegal type of factor";


  if (nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT)
  {
    lex(false);
    cout << psp() << yytext << endl;
  }

  if(nextToken == TOK_IDENT)
  {
    lex(false);
    cout << psp() << yytext << endl;
    if(symbolTable.find(yytext) == symbolTable.end()) {
      throw "104: identifier not declared";
    }
  }

  if (nextToken == TOK_OPENPAREN)
  {
    lex(false);
    cout << psp() << yytext << endl;
    nextToken = yylex();
    output("EXPRESSION");
    expression();
    lex_throw(TOK_CLOSEPAREN, false);
  }

  if (nextToken == TOK_NOT)
  {
    lex(false);
    nextToken = yylex();
    output("FACTOR");
    factor();
  }

  if(nextToken == TOK_MINUS) {
    lex(false);
    cout << psp() << yytext << endl;
    nextToken = yylex();
    output("FACTOR");
    factor();
  }
  --level;
  // cout << yytext << endl;
  cout << psp() << "exit <factor>" << endl;
}

void term()
{
  cout << psp() << "enter <term>" << endl;
  ++level;

  if(!first_of_term())
    throw "902: illegal type of term";

  output("FACTOR");
  factor();

  nextToken = yylex();
  while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND)
  {
    lex(false);
    cout << psp() << yytext << endl;
    if (nextToken == TOK_MULTIPLY)
    {
      nextToken = yylex();
      output("FACTOR");
      factor();
      nextToken = yylex();
    }

    if (nextToken == TOK_DIVIDE)
    {
      nextToken = yylex();
      output("FACTOR");
      factor();
      nextToken = yylex();
    }

    if (nextToken == TOK_AND)
    {
      nextToken = yylex();
      output("FACTOR");
      factor();
      nextToken = yylex();
    }
  }

  --level;
  // cout << yytext << endl;
  cout << psp() << "exit <term>" << endl;
}

void simple_exp()
{
  cout << psp() << "enter <simple_exp>" << endl;
  ++level;

  if(!first_of_simple_exp())
    throw "901: illegal type of simple expression";

  output("TERM");
  term();

  if (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR)
  {
    lex(false);
    cout << psp() << yytext << endl;
    nextToken = yylex();
    output("TERM");
    term();
  }

  --level;
  // cout << yytext << endl;
  cout << psp() << "exit <simple_exp>" << endl;
}

void expression()
{
  cout << psp() << "enter <expression>" << endl;
  ++level;
  if(!first_of_expression())
    throw "900: illegal type of statement";
  output("SIMPLE_EXP");
  simple_exp();

  while (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO)
  {
    lex(false);
    cout << psp() << yytext << endl;
    nextToken = yylex();
    output("SIMPLE_EXP");
    simple_exp();
  }

  --level;
  // cout << yytext << endl;
  cout << psp() << "exit <expression>" << endl;
}

void write()
{
  cout << psp() << "enter <write>" << endl;
  ++level;

  lex_throw(TOK_OPENPAREN);

  nextToken = yylex();
  output("WRITE");
  cout << psp() << yytext << endl;

  if(!(nextToken == TOK_IDENT || nextToken == TOK_STRINGLIT))
    throw "134: illegal type of operand(s)";
 

  lex_throw(TOK_CLOSEPAREN);
  nextToken = yylex();

  --level;
  cout << psp() << "exit <write>" << endl;
}

void read()
{
  cout << psp() << "enter <read>" << endl;
  ++level;

  lex_throw(TOK_OPENPAREN);

  nextToken = yylex();
  if (nextToken == TOK_IDENT)
  {
    output("IDENTIFIER");
    cout << psp() << yytext << endl;
  }

  lex_throw(TOK_CLOSEPAREN);
  nextToken = yylex();

  --level;
  cout << psp() << "exit <read>" << endl;
}

void while_stmt()
{
  cout << psp() << "enter <while>" << endl;
  ++level;

  nextToken = yylex();
  output("EXPRESSION");
  expression();

  if(nextToken == TOK_BEGIN) {
    output("STATEMENT");
    output("BEGIN");
  }

  statement();

  --level;
  cout << psp() << "exit <while>" << endl;
}

void if_stmt()
{
  cout << psp() << "enter <if>" << endl;
  ++level;

  nextToken = yylex();
  output("EXPRESSION");
  expression();

  lex_throw(TOK_THEN, false);

  nextToken = yylex();
  output("STATEMENT");
  if(nextToken == TOK_BEGIN)
    output("BEGIN");
  statement();

  if (nextToken == TOK_ELSE)
  {
    --level;

    output("ELSE");
    cout << psp() << "enter <else>" << endl;
    ++level;

    nextToken = yylex();
    output("STATEMENT");
    if(nextToken == TOK_BEGIN)
      output("BEGIN");
    statement();

    --level;
    
    ++level;
  }

  --level;
  cout << psp() << "exit <if>" << endl;
}

void compound_stmt()
{
  cout << psp() << "enter <compound_stmt>" << endl;
  ++level;
  nextToken = yylex();
  output("STATEMENT");
  if(nextToken == TOK_BEGIN)
    output("BEGIN");
  statement();

  if(nextToken != TOK_END) {
    lex(false);
  }

  while (nextToken == TOK_SEMICOLON)
  {
    nextToken = yylex();
    output("STATEMENT");
    if(nextToken == TOK_BEGIN)
      output("BEGIN");
    statement();
    if(nextToken != TOK_END) {
      lex(false);
    }
  }

  --level;
  lex_throw(TOK_END, false);
  nextToken = yylex();
  cout << psp() << "exit <compound_stmt>" << endl;
}

void assignment()
{
  cout << psp() << "enter <assignment>" << endl;
  ++level;
  lex_throw(TOK_IDENT, false);
  cout << psp() << yytext << endl;
  lex_throw(TOK_ASSIGN);
  nextToken = yylex();
  output("EXPRESSION");
  expression();

  --level;
  cout << psp() << "exit <assignment>" << endl;
}

void statement()
{
  if (!first_of_statement())
    throw "900: illegal type of statement";


  switch (nextToken)
  {
  case TOK_IDENT:
    /* assignment statement */
    assignment();
    break;
  case TOK_BEGIN:
    /* compound statement */
    compound_stmt();
    break;
  case TOK_IF:
    /* if statement */
    if_stmt();
    break;
  case TOK_WHILE:
    /* while statement */
    while_stmt();
    break;
  case TOK_READ:
    /* read statement */
    read();
    break;
  case TOK_WRITE:
    /* write statement */
    write();
    break;
  default:
    break;
  }
}

void var()
{
  nextToken = yylex();
  if (nextToken != TOK_IDENT)
    return;

  string idName;
  lex_throw(TOK_IDENT, false);
  idName = yytext;

  lex_throw(TOK_COLON);

  string idType;
  nextToken = yylex();
  if (nextToken == TOK_REAL || nextToken == TOK_INTEGER)
    output("TYPE");
  else
    throw "10: error in type";
  idType = yytext;

  lex_throw(TOK_SEMICOLON);

  cout << psp() << "-- idName: |" << idName << "| idType: |" << idType << "| --" << endl;

  if(symbolTable.find(idName) != symbolTable.end())
  {
    throw "101: identifier declared twice";
  }
  symbolTable.insert(idName);

  var();
}

// <block> -> [VAR IDENTIFIER : ( INTEGER | REAL ) ; { IDENTIFIER : ( INTEGER | REAL ) ; }] <compound>
void block()
{
  if (!first_of_block())
    throw "18: error in declaration part OR 17: 'BEGIN' expected";
  else
    output("BLOCK");

  cout << psp() << "enter <block>" << endl;
  ++level;

  if (nextToken == TOK_VAR)
    var();

  if (nextToken == TOK_BEGIN)
    lex(false);
  statement();

  --level;
  cout << psp() << "exit <block>" << endl;
}

// <program> → TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
void program()
{
  if (!first_of_program()) // Check for PROGRAM
    throw "3: 'PROGRAM' expected";
  else
    output("PROGRAM");

  cout << psp() << "enter <program>" << endl;
  ++level;
  lex_throw(TOK_IDENT);
  lex_throw(TOK_SEMICOLON);
  block();
  --level;
  cout << psp() << "exit <program>" << endl;
  nextToken = yylex();
}

bool first_of_program(void)
{
  return nextToken == TOK_PROGRAM;
}

bool first_of_block(void)
{
  nextToken = yylex();
  return nextToken == TOK_VAR || nextToken == TOK_BEGIN;
}

bool first_of_expression(void)
{
  return first_of_simple_exp();
}

bool first_of_statement(void)
{
  return nextToken == TOK_IDENT || nextToken == TOK_BEGIN || nextToken == TOK_IF || nextToken == TOK_WHILE || nextToken == TOK_READ || nextToken == TOK_WRITE;
}

bool first_of_simple_exp(void)
{
  return first_of_factor();
  // return first_of_factor() || nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR;
}

bool first_of_term(void)
{
  return first_of_factor();
  // return first_of_factor() || nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND;
}

bool first_of_factor(void)
{
  return nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

