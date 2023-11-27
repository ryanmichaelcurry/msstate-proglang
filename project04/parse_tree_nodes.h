//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Fall 2023
//  author: Ryan Michael Curry
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "lexer.h"

using namespace std;

typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable; // Holds the symbols in the interpreted program


extern bool printDelete;      // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Forward declaration of node types
class ExprNode; 
class TermNode;
class FactorNode;
class SimpleExpNode;

class StatementNode;
class AssignmentStmtNode;
class IfStmtNode;
class WhileStmtNode;
class ReadStmtNode;
class WriteStmtNode;

class ProgramNode;
class BlockNode;








// ---------------------------------------------------------------------
// <expr> -> <term> {{ (( + || - )) <term> }}
class ExprNode {
public:
  int _level = 0;          // recursion level of this node
  SimpleExpNode* firstSimpleExp = nullptr;
  vector<int> restSimpleExpOps;
  vector<SimpleExpNode*> restSimpleExps;

  float interpret();

  ExprNode(int level);
  ~ExprNode();
};
ostream& operator<<(ostream&, ExprNode&); // Node print operator














// ---------------------------------------------------------------------
// <term> -> <factor> {{ (( * || / )) <factor> }}
class TermNode {
public:
  int _level = 0;              // recursion level of this node
  FactorNode* firstFactor = nullptr;
  vector<int> restFactorOps;   // TOK_MULT_OP or TOK_DIV_OP
  vector<FactorNode*> restFactors;

  float interpret();

  TermNode(int level);
  ~TermNode();
};
ostream& operator<<(ostream&, TermNode&); // Node print operator




























// ---------------------------------------------------------------------
// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
// <factor> -> ID || INTLIT || ( <expr> )
class FactorNode {
public:
  int _level = 0;                        // recursion level of this node

  virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
  virtual ~FactorNode();                 // labeling the destructor as virtual allows 
	virtual float interpret() = 0;              // the subclass destructors to be called
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator
// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, int value);
    ~IntLitNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(int level, ExprNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
    float interpret();
};
// ---------------------------------------------------------------------
// class FloatLitNode (Float Literal Node)
class FloatLitNode : public FactorNode {
public:
    float float_literal = 0.0;

    FloatLitNode(int level, float value);
    ~FloatLitNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class MinusNode
class MinusNode : public FactorNode {
public:

    FactorNode* factorNode = nullptr;

    MinusNode(int level, FactorNode* fn);
    ~MinusNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class MinusNode
class NotNode : public FactorNode {
public:

    FactorNode* factorNode = nullptr;

    NotNode(int level, FactorNode* fn);
    ~NotNode();
    void printTo(ostream & os);
    float interpret();
};







class SimpleExpNode {
public:
  int _level = 0;              // recursion level of this node
  TermNode* firstTerm = nullptr;
  vector<int> restTermOps;
  vector<TermNode*> restTerms;

  SimpleExpNode(int level);
  ~SimpleExpNode();

  float interpret();

};
ostream& operator<<(ostream&, SimpleExpNode&); // Node print operator


class StatementNode {
public:
  int _level = 0;

  virtual void printTo(ostream & os) = 0;
  virtual ~StatementNode() = 0;
  virtual void interpret() = 0;
};
ostream& operator<<(ostream&, StatementNode&);

class AssignmentStmtNode : public StatementNode {
public:
  string* ident = nullptr;
  ExprNode* expression = nullptr;
  void printTo(ostream & os);
  AssignmentStmtNode(int level, string ident, ExprNode* expression);
  ~AssignmentStmtNode();
  void interpret();
};

class CompoundStmtNode : public StatementNode {
public:
  vector<StatementNode*> statements;
  void printTo(ostream & os);
  CompoundStmtNode(int level);
  ~CompoundStmtNode();
  void interpret();
};

class IfStmtNode : public StatementNode {
public:
  ExprNode* expression = nullptr;
  StatementNode* thenStatement = nullptr;
  StatementNode* elseStatement = nullptr;
  void printTo(ostream & os);
  IfStmtNode(int level, ExprNode* expression, StatementNode* thenStatement, StatementNode* elseStatement);
  ~IfStmtNode();
  void interpret();
};

class WhileStmtNode : public StatementNode {
public:
  ExprNode* expression = nullptr;
  StatementNode* statement = nullptr;
  void printTo(ostream & os);
  WhileStmtNode(int level, ExprNode* expression, StatementNode* statement);
  ~WhileStmtNode();
  void interpret();
};

class ReadStmtNode : public StatementNode {
public:
  string* ident = nullptr;
  void printTo(ostream & os);
  ReadStmtNode(int level, string ident);
  ~ReadStmtNode();
  void interpret();
};

class WriteStmtNode : public StatementNode {
public:
  string* ident = nullptr;
  string* literal = nullptr;
  void printTo(ostream & os);
  WriteStmtNode(int level, string ident, string literal);
  ~WriteStmtNode();
  void interpret();
};


// ---------------------------------------------------------------------
// Abstract class.
// <block> -> PROGRAM IDENTIFIER ; <block>
class BlockNode {
public:
  int _level = 0;

  vector<StatementNode*> statements;

  BlockNode(int level);
  ~BlockNode();
  void interpret();
};
ostream& operator<<(ostream&, BlockNode&);

// ---------------------------------------------------------------------
// Abstract class.
// <program> -> PROGRAM IDENTIFIER ; <block>
class ProgramNode {
public:
  int _level = 0;
  BlockNode* block;

  ProgramNode(int level);
  ~ProgramNode();
  void interpret();    
};
ostream& operator<<(ostream&, ProgramNode&); // Node print operator




#endif /* PARSE_TREE_NODES_H */