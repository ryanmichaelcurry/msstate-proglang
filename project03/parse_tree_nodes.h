//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Fall 2023
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

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
	                                       // the subclass destructors to be called
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
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, int value);
    ~IntLitNode();
    void printTo(ostream & os);
};
// ---------------------------------------------------------------------
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(int level, ExprNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
};
// ---------------------------------------------------------------------
// class FloatLitNode (Float Literal Node)
class FloatLitNode : public FactorNode {
public:
    float float_literal = 0.0;

    FloatLitNode(int level, float value);
    ~FloatLitNode();
    void printTo(ostream & os);
};
// ---------------------------------------------------------------------
// class MinusNode
class MinusNode : public FactorNode {
public:

    FactorNode* factorNode = nullptr;

    MinusNode(int level, FactorNode* fn);
    ~MinusNode();
    void printTo(ostream & os);
};
// ---------------------------------------------------------------------
// class MinusNode
class NotNode : public FactorNode {
public:

    FactorNode* factorNode = nullptr;

    NotNode(int level, FactorNode* fn);
    ~NotNode();
    void printTo(ostream & os);
};







class SimpleExpNode {
public:
  int _level = 0;              // recursion level of this node
  TermNode* firstTerm = nullptr;
  vector<int> restTermOps;   // TOK_MULT_OP or TOK_DIV_OP
  vector<TermNode*> restTerms;

  SimpleExpNode(int level);
  ~SimpleExpNode();

};
ostream& operator<<(ostream&, SimpleExpNode&); // Node print operator


class StatementNode {
public:
  int _level = 0;

  virtual void printTo(ostream & os) = 0;
  virtual ~StatementNode() = 0;
};
ostream& operator<<(ostream&, StatementNode&);

class AssignmentStmtNode : public StatementNode {
public:
  string* ident = nullptr;
  ExprNode* expression = nullptr;
  void printTo(ostream & os);
  AssignmentStmtNode(int level, string ident, ExprNode* expression);
  ~AssignmentStmtNode();
};

class CompoundStmtNode : public StatementNode {
public:
  StatementNode* statement = nullptr;
  void printTo(ostream & os);
  CompoundStmtNode(int level, StatementNode* statement);
  ~CompoundStmtNode();
};

class IfStmtNode : public StatementNode {
public:
  ExprNode* expression = nullptr;
  StatementNode* thenStatement = nullptr;
  StatementNode* elseStatement = nullptr;
  void printTo(ostream & os);
  IfStmtNode(int level, ExprNode* expression, StatementNode* thenStatement, StatementNode* elseStatement);
  ~IfStmtNode();
};

class WhileStmtNode : public StatementNode {
public:
  ExprNode* expression = nullptr;
  StatementNode* statement = nullptr;
  void printTo(ostream & os);
  WhileStmtNode(int level, ExprNode* expression, StatementNode* statement);
  ~WhileStmtNode();
};

class ReadStmtNode : public StatementNode {
public:
  string* ident = nullptr;
  void printTo(ostream & os);
  ReadStmtNode(int level, string ident);
  ~ReadStmtNode();
};

class WriteStmtNode : public StatementNode {
public:
  string* ident = nullptr;
  string* literal = nullptr;
  void printTo(ostream & os);
  WriteStmtNode(int level, string ident, string literal);
  ~WriteStmtNode();
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
};
ostream& operator<<(ostream&, ProgramNode&); // Node print operator




#endif /* PARSE_TREE_NODES_H */