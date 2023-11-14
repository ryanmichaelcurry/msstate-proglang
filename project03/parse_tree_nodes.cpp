//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Fall 2023
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#include "parse_tree_nodes.h"

bool printDelete = false;   // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("| ");
}
// ---------------------------------------------------------------------
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) {
  os << endl; indent(fn._level); os << "(factor ";
  fn.printTo(os);
  os << endl; indent(fn._level); os << "factor) ";
	return os;
}
// ---------------------------------------------------------------------
IdNode::IdNode(int level, string name) {
  _level = level;
  id = new string(name);
}
IdNode::~IdNode() {
  if(printDelete) 
    cout << "Deleting FactorNode:IdNode " << endl;
	delete id;
	id = nullptr;
}
void IdNode::printTo(ostream& os) {
	os << "(IDENT: " << *id << ") ";
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, int value) {
  _level = level;
  int_literal = value;
}
IntLitNode::~IntLitNode() {
  if(printDelete)
    cout << "Deleting FactorNode:IntLitNode " << endl;
	  // Nothing to do since the only members are not pointers
}
void IntLitNode::printTo(ostream& os) {
	os << "(INTLIT: " << int_literal << ") ";
}
// ---------------------------------------------------------------------
FloatLitNode::FloatLitNode(int level, float value) {
  _level = level;
  float_literal = value;
}
FloatLitNode::~FloatLitNode() {
  if(printDelete)
    cout << "Deleting FactorNode:FloatLitNode " << endl;
	  // Nothing to do since the only members are not pointers
}
void FloatLitNode::printTo(ostream& os) {
	os << "(FLOATLIT: " << float_literal << ") ";
}
// ---------------------------------------------------------------------
MinusNode::MinusNode(int level, FactorNode* fn) {
  _level = level;
  factorNode = fn;
}
MinusNode::~MinusNode() {
  if(printDelete)
    cout << "Deleting FactorNode:MinusNode " << endl;
	  // Nothing to do since the only members are not pointers
}
void MinusNode::printTo(ostream& os) {
	os << "(MINUSNODE:  ) ";
}
// ---------------------------------------------------------------------
NotNode::NotNode(int level, FactorNode* fn) {
  _level = level;
  factorNode = fn;
}
NotNode::~NotNode() {
  if(printDelete)
    cout << "Deleting FactorNode:NotNode " << endl;
	  // Nothing to do since the only members are not pointers
}
void NotNode::printTo(ostream& os) {
	os << "(NOTNODE:  ) ";
}
// ---------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExprNode* en) {
  _level = level;
	exprPtr = en;
}
void NestedExprNode::printTo(ostream& os) {
	os << *exprPtr;
}
NestedExprNode::~NestedExprNode() {
  if(printDelete)
    cout << "Deleting FactorNode:NestedExprNode " << endl;
  delete exprPtr;
  exprPtr = nullptr;
}
// ---------------------------------------------------------------------
TermNode::TermNode(int level) {
  _level = level;
}
ostream& operator<<(ostream& os, TermNode& tn) {
  os << endl; indent(tn._level); os << "(term ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
    if (op == TOK_MULTIPLY) {
      os << endl; indent(tn._level); os << "* ";
    } else {
      os << endl; indent(tn._level); os << "/ ";
    }
		os << *(tn.restFactors[i]);
	}
  os << endl; indent(tn._level); os << "term) ";
	return os;
}
TermNode::~TermNode() {
  if(printDelete)
    cout << "Deleting TermNode " << endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}
// ---------------------------------------------------------------------
ExprNode::ExprNode(int level) {
  _level = level;
}
ostream& operator<<(ostream& os, ExprNode& en) {
  os << endl; indent(en._level); os << "(expr ";
	os << *(en.firstSimpleExp);

	int length = en.restSimpleExpOps.size();
	for (int i = 0; i < length; ++i) {
		int op = en.restSimpleExpOps[i];
    if (op == TOK_PLUS) {
      os << endl; indent(en._level); os << "+ ";
    } else {
      os << endl; indent(en._level); os << "- ";
    }
		os << *(en.restSimpleExps[i]);
	}
  os << endl; indent(en._level); os << "expr) ";
	return os;
}
ExprNode::~ExprNode() {
  if(printDelete)
    cout << "Deleting ExprNode " << endl;
	delete firstSimpleExp;
	firstSimpleExp = nullptr;

	int length = restSimpleExpOps.size();
	for (int i = 0; i < length; ++i) {
		delete restSimpleExps[i];
		restSimpleExps[i] = nullptr;
	}
}

// ---------------------------------------------------------------------
SimpleExpNode::SimpleExpNode(int level) {
  _level = level;
}
ostream& operator<<(ostream& os, SimpleExpNode& sen) {
  os << endl; indent(sen._level); os << "(simple_exp ";
	os << *(sen.firstTerm);

	int length = sen.restTermOps.size();
	for (int i = 0; i < length; ++i) {
		int op = sen.restTermOps[i];
    if (op == TOK_PLUS) {
      os << endl; indent(sen._level); os << "+ ";
    } else if (op == TOK_MINUS) {
      os << endl; indent(sen._level); os << "- ";
    } else {
      os << endl; indent(sen._level); os << "OR ";
    }
		os << *(sen.restTerms[i]);
	}
  os << endl; indent(sen._level); os << "simple_exp) ";
	return os;
}
SimpleExpNode::~SimpleExpNode() {
  // cout << "DELETING SIMPLE EXP" << endl;
  if(printDelete)
    cout << "Deleting SimpleExpNode " << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}