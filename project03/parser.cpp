//*****************************************************************************
// (part 3)
// purpose: Recursive descent parsing of arithmetic expressions
//          Builds the parse tree while parsing the input
// version: Fall 2023
//  author: Ryan Michael Curry
//*****************************************************************************

#include "parser.h"
#include "parse_tree_nodes.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

// Forward declarations of first_of functions
bool first_of_expr();
bool first_of_term();
bool first_of_factor();
bool first_of_simple_exp();
bool first_of_statement();
bool first_of_block();
bool first_of_program();

// Functions
StatementNode* statement();

int nextToken = 0;            // hold nextToken returned by lex
bool printParse = false;      // shall we print the parse tree?

// Which tree level are we currently in?  Setting this to -1
// means the top-level expression is at level 0.
static int level = 0;

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

// Handle syntax errors
void error() {
  cout << endl << "===========================" << endl;
  cout << "ERROR near: " << yytext;
  cout << endl << "===========================" << endl;
  if (yyin)
    fclose(yyin);
  exit(EXIT_FAILURE);
}
//*****************************************************************************
// Print each level with appropriate indentation
void indent() {
  for (int i = 0; i<level; i++)
    cout << ("  ");
}
//*****************************************************************************
// Announce what the lexical analyzer has found
void output() {
  indent();
  cout << "---> FOUND " << yytext << endl;
}
//*****************************************************************************
int lex() {
  nextToken = yylex();

  if (nextToken == TOK_EOF) {
      // save a "lexeme" into yytext
      yytext[0] = 'E';
      yytext[1] = 'O';
      yytext[2] = 'F';
      yytext[3] = 0;
  }
  if(printParse) {
    // Tell us about the token and lexeme
    indent();
    cout << "Next token is: ";
    switch(nextToken) {
    case TOK_INTLIT:      cout << "TOK_INTLIT";     break;
    case TOK_IDENT:        cout << "TOK_IDENT";       break;
    case TOK_PLUS:       cout << "TOK_PLUS";      break;
    case TOK_MINUS:       cout << "TOK_SUB_OP";      break;
    case TOK_MULTIPLY:      cout << "TOK_MULT_OP";     break;
    case TOK_DIVIDE:       cout << "TOK_DIV_OP";      break;
    case TOK_OPENPAREN:   cout << "TOK_LEFT_PAREN";  break;
    case TOK_CLOSEPAREN:  cout << "TOK_RIGHT_PAREN"; break;
    case TOK_EOF:          cout << "TOK_EOF";         break;
    case TOK_UNKNOWN:      cout << "TOK_UNKNOWN";     break;
    default: error();
    }
    cout << ", Next lexeme is: " << yytext << endl;
  }
  return nextToken;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <factor> -> id || int_constant || ( <expr> )
FactorNode* factor() {
  // Check that the <factor> starts with a valid token
  if(!first_of_factor())
    error();

  if(printParse) {
    cout << psp() << "enter <factor>" << endl;
  }

  level = level + 1;
  FactorNode* newFactorNode = nullptr;

  // Determine which RHS token we have
  switch(nextToken) {

    case TOK_IDENT:
      if(printParse) {
        output("IDENTIFIER");
        cout << psp() << yytext << endl;
      }
      if(symbolTable.find(yytext) == symbolTable.end()) {
        throw "104: identifier not declared";
      }
      newFactorNode = new IdNode(level, string(yytext));
      break;

    case TOK_INTLIT:
      if(printParse) {
        output("INTLIT");
        cout << psp() << yytext << endl;
      }
      newFactorNode = new IntLitNode(level, atoi(yytext));
      break;

    case TOK_FLOATLIT:
      if(printParse) {
        output("FLOATLIT");
        cout << psp() << yytext << endl;
      }
      newFactorNode = new FloatLitNode(level, atof(yytext));
      //nextToken = yylex();
      break;

    case TOK_OPENPAREN:
      // We expect ( <expr> ) parse it
      if(printParse) {
        output("OPENPAREN");
        cout << psp() << yytext << endl;
      }
      nextToken = yylex();
      if (!first_of_expr()) // Check for 'IDENT' | 'INT_LIT' | (
        error();
      if(printParse) output("EXPRESSION");
      newFactorNode = new NestedExprNode(level, expr());
      if(nextToken != TOK_CLOSEPAREN) throw "<expr> does not end with )";
      if(printParse) output("CLOSEPAREN");
      //nextToken = yylex();
      break;

	}

  if (nextToken == TOK_NOT) {
    if(printParse) {
      output("NOT");
      cout << psp() << yytext << endl;
    }
    nextToken = yylex();
    if(printParse) output("FACTOR");
    newFactorNode = new NotNode(level, factor());
  }

  if (nextToken == TOK_MINUS) {
      if(printParse) {
        output("MINUS");
        cout << psp() << yytext << endl;
      }
      nextToken = yylex();
      if(printParse) output("FACTOR");
      newFactorNode = new MinusNode(level, factor());
  }

/*
  else {
    // If we made it to here, syntax error
    error();
  }
*/

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <factor>" << endl;
  }

  return newFactorNode;
}
bool first_of_factor(){
  return nextToken == TOK_OPENPAREN || nextToken == TOK_IDENT || nextToken == TOK_INTLIT || TOK_FLOATLIT;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <term> -> <factor> {{ (( * || /  || AND )) <factor> })
TermNode* term() {
  /* Check that the <term> starts with a valid token */
  if(!first_of_term())
    error();

  if(printParse) {
    cout << psp() << "enter <term>" << endl;
  }
  level = level + 1;
  TermNode* newTermNode = new TermNode(level);

  output("FACTOR");

  /* Parse the first factor */
  newTermNode->firstFactor = factor();
  nextToken = yylex();
  //cout << yytext << endl;

  /* As long as the next token is * or /, get the
     next token and parse the next factor */
  while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND) {
    switch (nextToken)
    {
    case TOK_MULTIPLY:
      if(printParse) output("MULTIPLY");
      cout << psp() << yytext << endl;
      break;
    case TOK_DIVIDE:
      if(printParse) output("DIVIDE");
      cout << psp() << yytext << endl;
      break;
    case TOK_AND:
      if(printParse) output("AND");
      cout << psp() << yytext << endl;
      break;
    }
    newTermNode->restFactorOps.push_back(nextToken);
    nextToken = yylex();
    if(printParse) output("FACTOR");
    newTermNode->restFactors.push_back(factor());
    nextToken = yylex();
  }

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <term>" << endl;
  }

  return newTermNode;
}
bool first_of_term(){
  return nextToken == TOK_OPENPAREN || nextToken == TOK_IDENT || nextToken == TOK_INTLIT || TOK_FLOATLIT;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
ExprNode* expr() {
  // Check that the <expr> starts with a valid token 
  // cout << nextToken << endl;
  if(!first_of_expr()) throw "first of expr";

  if(printParse) {
    cout << psp() << "enter <expression>" << endl;
  }

  level = level + 1;
  if(printParse) output("SIMPLE_EXP");
  ExprNode* newExprNode = new ExprNode(level);

  /* Parse the first term */
  newExprNode->firstSimpleExp = simple_exp();

  // As long as the next token is + or -, get the next token and parse the next term
  while (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO) {
    if(printParse) {
    switch (nextToken)
    {
    case TOK_EQUALTO:
      output("EQUALTO");
      cout << psp() << yytext << endl;
      break;
    case TOK_LESSTHAN:
      output("LESSTHAN");
      cout << psp() << yytext << endl;
      break;
    case TOK_GREATERTHAN:
      output("GREATERTHAN");
      cout << psp() << yytext << endl;
      break;
    case TOK_NOTEQUALTO:
      output("NOTEQUALTO");
      cout << psp() << yytext << endl;
      break;
    default:
      break;
    }
    }
    newExprNode->restSimpleExpOps.push_back(nextToken);
    nextToken = yylex();
    if(printParse) output("SIMPLE_EXP");
    newExprNode->restSimpleExps.push_back(simple_exp());
  }

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <expression>" << endl;
  }
  return newExprNode;
}
bool first_of_expr(){
  return nextToken == TOK_OPENPAREN || nextToken == TOK_IDENT || nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_MINUS || nextToken == TOK_NOT;
}

SimpleExpNode* simple_exp() {
  if(!first_of_simple_exp())
    error();

  if(printParse) {
    cout << psp() << "enter <simple_exp>" << endl;
  }
  level = level + 1;

  SimpleExpNode* newSimpleExpNode = new SimpleExpNode(level);

  /* Parse the first term */
  if(printParse) output("TERM");
  newSimpleExpNode->firstTerm = term();

  

  if (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR) {
    if(printParse) {
      switch (nextToken)
      {
      case TOK_PLUS:
        output("PLUS");
        cout << psp() << yytext << endl;
        break;
      case TOK_MINUS:
        output("MINUS");
        cout << psp() << yytext << endl;
        break;
      case TOK_OR:
        output("OR");
        cout << psp() << yytext << endl;
        break;
      
      default:
        break;
      }
    }
    newSimpleExpNode->restTermOps.push_back(nextToken);
    nextToken = yylex();
    if(printParse) output("TERM");
    newSimpleExpNode->restTerms.push_back(term());
  }

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <simple_exp>" << endl;
  }
  return newSimpleExpNode;
}
bool first_of_simple_exp() {
  return nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

IfStmtNode* if_stmt() {
  if(printParse) {
    cout << psp() << "enter <if>" << endl;
  }
  level = level + 1;
  
  nextToken = yylex();
  if(printParse) output("EXPRESSION");

  ExprNode* newExprNode = expr();

  if(nextToken != TOK_THEN) throw "52: 'THEN' expected";
  if(printParse) output("THEN");
  nextToken = yylex();
  if(printParse) output("STATEMENT");
  if(nextToken == TOK_BEGIN) output("BEGIN");

  StatementNode* thenStatement = statement();
  // nextToken = yylex();
  StatementNode* elseStatement = nullptr;

  if(nextToken == TOK_ELSE) {
    --level;
    if(printParse){
      output("ELSE");
      cout << psp() << "enter <else>" << endl;
    }
    ++level;
    nextToken = yylex();
    if(printParse) output("STATEMENT");
    if(nextToken == TOK_BEGIN && printParse) output("BEGIN");
    elseStatement = statement();
  }

  IfStmtNode* newIfStmtNode = new IfStmtNode(level, newExprNode, thenStatement, elseStatement);

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <if>" << endl;
  }
  
  return newIfStmtNode;
}

// TODO: more stuff here
AssignmentStmtNode* assignment() {
  if(printParse) {
    cout << psp() << "enter <assignment>" << endl;
  }
  level = level + 1;

  if(nextToken != TOK_IDENT) { 
    error();
  }

  if(printParse) {
    output("IDENTIFIER");
    cout << psp() << yytext << endl;
  }
  string ident = yytext;

  nextToken = yylex();
  if(nextToken != TOK_ASSIGN) {
    throw "type 104: identifier not declared";
  }

  if(printParse) output("ASSIGN");
  nextToken = yylex();
  if(printParse) output("EXPRESSION");

  AssignmentStmtNode* newAssignmentStmtNode = new AssignmentStmtNode(level, ident, expr());

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <assignment>" << endl;
  }

  return newAssignmentStmtNode;
}

CompoundStmtNode* compound_stmt() {
  if(printParse) {
    cout << psp() << "enter <compound_stmt>" << endl;
  }
  level = level + 1;

  nextToken = yylex();
  if(printParse) output("STATEMENT");
  if(nextToken == TOK_BEGIN)
    if(printParse) ("BEGIN");
  
  CompoundStmtNode* newCompoundStmtNode = new CompoundStmtNode(level);
  newCompoundStmtNode->statements.push_back(statement());

  if(nextToken != TOK_END) {
    // TODO: lex(false)
  }

  while(nextToken == TOK_SEMICOLON) {
    if(printParse) output("SEMICOLON");
    nextToken = yylex();
    if(printParse) output("STATEMENT");
    if(nextToken == TOK_BEGIN)
      if(printParse) output("BEGIN");
    newCompoundStmtNode->statements.push_back(statement());
    if(nextToken != TOK_END) {
      //if(printParse) {output("SEMICOLON");} // TODO: do we need to print semicolons? do we check for end?
    }
  }

  level = level - 1;
  //nextToken = yylex();
  if(nextToken != TOK_END) throw "13: 'END' expected";
  if(printParse) {
    output("END");
    cout << psp() << "exit <compound_stmt>" << endl;
  }
  nextToken = yylex();

  return newCompoundStmtNode;
}

ReadStmtNode* read() {
  if(printParse) {
    cout << psp() << "enter <read>" << endl;
  }
  level = level + 1;

  ReadStmtNode* newReadStmtNode;

  nextToken = yylex();
  // (
  if(nextToken != TOK_OPENPAREN) throw "6: '(' expected";
  if(printParse) output("OPENPAREN");
  // 'Hello World'
  nextToken = yylex();

  if(nextToken != TOK_IDENT) throw "2: identifier expected";
  if(printParse) {
    output("IDENTIFIER");
    cout << psp() << yytext << endl;
  }
  
  string ident = yytext;
  newReadStmtNode = new ReadStmtNode(level, ident);
  nextToken = yylex();

  if(nextToken != TOK_CLOSEPAREN) throw "4: ')' expected";
  if(printParse) output("CLOSEPAREN");
  nextToken = yylex();

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <read>" << endl;
  }

  return newReadStmtNode;
}

WriteStmtNode* write() {
  if(printParse) {
    cout << psp() << "enter <write>" << endl;
  }
  level = level + 1;

  WriteStmtNode* newWriteStmtNode;

  nextToken = yylex();
  // (
  if(nextToken != TOK_OPENPAREN) throw "6: '(' expected";
  if(printParse) output("OPENPAREN");
  // 'Hello World'
  nextToken = yylex();
  
  if(!(nextToken == TOK_IDENT || nextToken == TOK_STRINGLIT))
    throw "134: illegal type of operand(s)";
  if(nextToken == TOK_IDENT)
    newWriteStmtNode = new WriteStmtNode(level, yytext, "");
  else
    newWriteStmtNode = new WriteStmtNode(level, "", yytext);
  
  if(printParse) {
    output("WRITE");
    cout << psp() << yytext << endl;
  }
  nextToken = yylex();
  if(nextToken != TOK_CLOSEPAREN) throw "4: ')' expected";
  if(printParse) output("CLOSEPAREN");
  nextToken = yylex();

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <write>" << endl;
  }

  return newWriteStmtNode;
}

WhileStmtNode* while_stmt() {
  if(printParse) {
    cout << psp() << "enter <while>" << endl;
  }
  level = level + 1;

  WhileStmtNode* newWhileStmtNode;

  nextToken = yylex();
  if(printParse) {
    output("EXPRESSION");
  }

  ExprNode* newExprNode = expr();
  
  if(nextToken == TOK_BEGIN && printParse) {
    output("STATEMENT");
    output("BEGIN");
  } else if(printParse) {
    output("STATEMENT");
  }

  newWhileStmtNode = new WhileStmtNode(level, newExprNode, statement());

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <while>" << endl;
  }

  return newWhileStmtNode;
}

StatementNode* statement() {
  if(!first_of_statement())
    error();
    //throw "900: illegal type of statement";

  switch (nextToken)
  {
  case TOK_IDENT:
    /* assignment statement */
    return assignment();
    break;
  case TOK_BEGIN:
    /* compound statement */
    return compound_stmt();
    break;
  case TOK_IF:
    /* if statement */
    return if_stmt();
    break;
  case TOK_WHILE:
    /* while statement */
    return while_stmt();
    break;
  case TOK_READ:
    /* read statement */
    return read();
    break;
  case TOK_WRITE:
    /* write statement */
    return write();
    break;
  default:
    throw "999: an error has occurred";
    break;
  }
}
bool first_of_statement(void) {
  return nextToken == TOK_IDENT || nextToken == TOK_BEGIN || nextToken == TOK_IF || nextToken == TOK_WHILE || nextToken == TOK_READ || nextToken == TOK_WRITE;
}
void var() {
  nextToken = yylex();

  if(nextToken == TOK_BEGIN) return;
  if(nextToken != TOK_IDENT) throw "2: identifier expected";
  if(printParse) output("IDENTIFIER");
  
  string idName;
  idName = yytext;

  nextToken = yylex();
  if(nextToken != TOK_COLON) throw "5: ':' expected";
  if(printParse) output("COLON");

  string idType;
  nextToken = yylex();
  if (nextToken == TOK_REAL || nextToken == TOK_INTEGER)
    if(printParse) output("TYPE");
  else
    throw "10: error in type";
  idType = yytext;

  nextToken = yylex();
  if(nextToken != TOK_SEMICOLON) throw "14: ';' expected";
  if(printParse) output("SEMICOLON");

  if(printParse)
    cout << psp() << "-- idName: |" << idName << "| idType: |" << idType << "| --" << endl;

  if(symbolTable.find(idName) != symbolTable.end())
    throw "101: identifier declared twice";
  
  symbolTable.insert(idName);

  var();
}

BlockNode* block() {
  if(!first_of_block())
    error();

  if(printParse) {
    cout << psp() << "enter <block>" << endl;
  }
  level = level + 1;

  BlockNode* newBlockNode = new BlockNode(level);

  if(nextToken == TOK_VAR) {
    var();
  }

  if(nextToken == TOK_BEGIN) {
    // TODO: equivalent of lex(false)
    if(printParse) output("BEGIN");
    newBlockNode->statements.push_back(statement());
  }

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <block>" << endl;
  }

  return newBlockNode;
}
bool first_of_block() {
  return nextToken == TOK_VAR || nextToken == TOK_BEGIN;
}

ProgramNode* program() {
  if(!first_of_program())
    error();
  
  if(printParse) {
    output("PROGRAM");
    indent();
    cout << "enter <program>" << endl;
  }
  level = level + 1;
  ProgramNode* newProgramNode = new ProgramNode(level);

  nextToken = yylex();
  if(nextToken != TOK_IDENT) throw "2: identifier expected";
  if(printParse) output("IDENTIFIER");
  nextToken = yylex();
  if(nextToken != TOK_SEMICOLON) throw "14: ';' expected";
  if(printParse) output("SEMICOLON");
  nextToken = yylex();
  if(printParse) output("BLOCK");

  newProgramNode->block = block();

  level = level - 1;
  if(printParse) {
    cout << psp() << "exit <program>" << endl;
  }

  while(nextToken == TOK_END) nextToken = yylex(); // We are hoping for EOF
  return newProgramNode;
}
bool first_of_program() {
  return nextToken == TOK_PROGRAM;
}


