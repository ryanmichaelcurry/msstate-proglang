//**************************************************************************

// Replace with appropriate header comment......

//**************************************************************************

#include <stdio.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"

using namespace std;

extern "C"
{
  extern int yylex();  // the generated lexical analyzer
  extern char *yytext; // text of current lexeme
}

int nextToken = 0;    // next token to process
static int level = 0; // current parse tree level
static int noun_count = 0;
static int verb_count = 0;
static int adjective_count = 0;

// Production functions
bool firstOf_sentence();

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
// Get the next lexeme (word in sentence)
int lex()
{
  char *token_str; // point to the logical name of the token

  nextToken = yylex();
  if (nextToken == TOK_EOF)
  {
    yytext[0] = 'E';
    yytext[1] = 'O';
    yytext[2] = 'F';
    yytext[3] = 0;
  }

  // Determine the logical name of the lexeme
  switch (nextToken)
  {
  case NOUN:
    token_str = (char *)"NOUN";
    break;
  case ADJECTIVE:
    token_str = (char *)"ADJECTIVE";
    break;
  case VERB:
    token_str = (char *)"VERB";
    break;
  case ADVERB:
    token_str = (char *)"ADVERB";
    break;
  case ARTICLE:
    token_str = (char *)"ARTICLE";
    break;
  case POSSESSIVE:
    token_str = (char *)"POSSESSIVE";
    break;

  case TOK_UNKNOWN:
    token_str = (char *)"TOK_UNKNOWN";
    break;

  default:
    token_str = (char *)"=== unmapped token name ===";
  }

  cout << psp() << "next lexeme: |" << yytext << "|, token: " << token_str << endl;
  return nextToken;
}
//*****************************************************************************
// Report what we found
void output(string what)
{
  cout << psp() << "found " << what << ": |" << yytext << "|" << endl;
}
//*****************************************************************************

// <adjective_phrase> -> ( ARTICLE | POSSESSIVE ) ADJECTIVE
void adjective()
{
  if (nextToken != ARTICLE && nextToken != POSSESSIVE)
  {
    if (nextToken != ADJECTIVE)
    {
      throw("<adjective phrase> did not have an adjective.");
    }
    throw("<adjective phrase> did not start with an article or possessive.");
  }

  ++adjective_count;
  cout << psp() << "enter <adjective phrase> " << adjective_count << endl;

  ++level;
  if (nextToken == ARTICLE)
  {
    cout << psp() << "found ARTICLE: |" << yytext << "|" << endl;
    lex();

    if (nextToken == ADJECTIVE)
    {
      cout << psp() << "found ADJECTIVE: |" << yytext << "|" << endl;
      lex();
    }
  }

  else if (nextToken == POSSESSIVE)
  {
    cout << psp() << "found POSSESSIVE: |" << yytext << "|" << endl;
    lex();

    if (nextToken == ADJECTIVE)
    {
      cout << psp() << "found ADJECTIVE: |" << yytext << "|" << endl;
      lex();
    }
  }
  --level;

  cout << psp() << "exit <adjective phrase> " << adjective_count << endl;
}

// <noun_phrase> -> <adjective phrase> NOUN
void noun()
{
  if (nextToken != ARTICLE && nextToken != POSSESSIVE)
  {
    throw("<noun phrase> did not start with an article or possessive.");
  }

  ++noun_count;
  cout << psp() << "enter <noun phrase> " << noun_count << endl;

  ++level;
  adjective();

  if (nextToken == NOUN)
  {
    cout << psp() << "found NOUN: |" << yytext << "|" << endl;
    lex();
  }
  else
  {
    throw("<noun phrase> did not have a noun.");
  }

  --level;
  cout << psp() << "exit <noun phrase> " << noun_count << endl;
}

// <sentence> -> VERB | ADVERB <verb phrase>
void verb()
{
  if (nextToken != VERB && nextToken != ADVERB)
  {
    throw("<verb phrase> did not start with a verb or an adverb.");
  }

  ++verb_count;
  cout << psp() << "enter <verb phrase> " << verb_count << endl;

  ++level;
  if (nextToken == VERB)
  {
    cout << psp() << "found VERB: |" << yytext << "|" << endl;
    lex();
  }

  else if (nextToken == ADVERB)
  {
    cout << psp() << "found ADVERB: |" << yytext << "|" << endl;
    lex();
    verb();
    --verb_count;
  }
  --level;

  cout << psp() << "exit <verb phrase> " << verb_count << endl;
}

// <sentence> -> <noun phrase> <verb phrase> <noun phrase>
void sentence()
{
  if (firstOf_sentence() == false)
    throw("<sentence> did not start with an article or possessive.");

  cout << "enter <sentence>" << endl;
  ++level;

  noun();
  verb();
  noun();

  --level;
  cout << "exit <sentence>" << endl;
}
//*****************************************************************************
bool firstOf_sentence()
{
  if (nextToken == ARTICLE || nextToken == POSSESSIVE)
  {
    return true;
  }
  return false;
}