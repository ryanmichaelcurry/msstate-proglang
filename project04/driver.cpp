//*****************************************************************************
// (part 3 example)
// purpose: driver file for parser example - flex generated lexer
//          builds parse tree while parsing the input
// version: Fall 2023
// author: Joe Crumpton / Ed Swan
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"
#include <cstring>

using namespace std;

extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;        // input stream
  extern FILE *yyout;       // output stream
  extern int   yyleng;      // length of current lexeme
  extern char *yytext;      // text of current lexeme
  extern int   yylineno;    // line number for current lexeme
  extern int   yylex();     // the generated lexical analyzer
  extern int   yylex_destroy(); // deletes memory allocated by yylex
}

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[])
{
  // Whether to print these items
  bool printTree = false; 
  bool printSymbolTable = false;

  // Process any command-line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = true;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = true;
    }
    // -s flag: if requested, print symbol table
    if(strcmp(argv[i], "-s") == 0) {
      printSymbolTable = true;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = true;
    }
  }

  // Set the input stream
  if (argc > 1) {
    printf("INFO: Using the %s file for input\n", argv[argc-1]);
    yyin = fopen(argv[argc-1], "r"); 
  }

  // If uncommented, this line allows typing in an expression interactively
  // If so, to end must type "Ctrl-D" (generate EOF character)
  // yyin = stdin;

  // Create the root of the parse tree
  ProgramNode* root = nullptr;

  nextToken = yylex();  // prime the pump (get first token)

  try {
  do {
    root = program(); 
  } while(nextToken != TOK_EOF); 
  } catch(char const *errmsg) {
    cout << endl << "***ERROR:" << endl;
    cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
    cout << errmsg << endl;
    return EXIT_FAILURE;
  }
  if (yyin)
    fclose(yyin);
  

  yylex_destroy();

  if(printParse) {
    // Tell the world about our success!!
    cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
  }

  // Printing and Deleting the tree all result in 
  // the same in-order traversal of the tree as parsing.  All
  // use the call stack.

  if(printTree) {
    cout << endl << "*** Print the Tree ***" << endl;
    cout << *root << endl << endl;
  }
  cout << endl;
  
  cout << "*** Interpret the Tree ***" << endl;
  root->interpret();
  cout << endl;

  if(printSymbolTable)
  {
    cout << "*** Print the Symbol Table ***" << endl;
    symbolTableT::iterator it;
    for(it = symbolTable.begin(); it != symbolTable.end(); ++it )
      cout << setw(8) << it->first << ": " << it->second << endl;
  }
  cout << endl;

  cout << "*** Delete the Tree ***" << endl;
  delete root;
  root = nullptr;

  return(EXIT_SUCCESS);
}
