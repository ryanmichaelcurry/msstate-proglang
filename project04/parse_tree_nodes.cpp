//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Fall 2023
//  author: Ryan Michael Curry
//*****************************************************************************

#include "parse_tree_nodes.h"

bool printDelete = false;   // shall we print deleting the tree?

#define EPSILON 0.001
static bool truth(float F) {
  return !((EPSILON > F) && (F > -EPSILON));
}

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("|  ");
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
  _level = level - 1;
  id = new string(name);
}
IdNode::~IdNode() {
  if(printDelete) 
    cout << "Deleting FactorNode:IdNode" << endl;
	delete id;
	id = nullptr;
}
float IdNode::interpret() {
  symbolTableT::iterator variable = symbolTable.find((*id));
  return variable->second;
}
void IdNode::printTo(ostream& os) {
	os << "( IDENT: " << *id << " ) ";
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, int value) {
  _level = level - 1;
  int_literal = value;
}
IntLitNode::~IntLitNode() {
  if(printDelete)
    cout << "Deleting FactorNode:IntLitNode" << endl;
	  // Nothing to do since the only members are not pointers
}
float IntLitNode::interpret() {
  return int_literal;
}
void IntLitNode::printTo(ostream& os) {
	os << "( INTLIT: " << int_literal << " ) ";
}
// ---------------------------------------------------------------------
FloatLitNode::FloatLitNode(int level, float value) {
  _level = level - 1;
  float_literal = value;
}
FloatLitNode::~FloatLitNode() {
  if(printDelete)
    cout << "Deleting FactorNode:FloatLitNode" << endl;
	  // Nothing to do since the only members are not pointers
}
float FloatLitNode::interpret() {
  return float_literal;
}
void FloatLitNode::printTo(ostream& os) {
	os << "( FLOATLIT: " << float_literal << " ) ";
}
// ---------------------------------------------------------------------
MinusNode::MinusNode(int level, FactorNode* fn) {
  _level = level - 1;
  factorNode = fn;
}
MinusNode::~MinusNode() {
  if(printDelete)
    cout << "Deleting FactorNode:MinusNode" << endl;
  delete factorNode;
}
float MinusNode::interpret() {
  return -factorNode->interpret();
}
void MinusNode::printTo(ostream& os) {
	os << "(- " << *(factorNode) << ") ";
}
// ---------------------------------------------------------------------
NotNode::NotNode(int level, FactorNode* fn) {
  _level = level - 1;
  factorNode = fn;
}
NotNode::~NotNode() {
  if(printDelete)
    cout << "Deleting FactorNode:NotNode" << endl;
  delete factorNode;
}
float NotNode::interpret() {
  if(truth(factorNode->interpret()))
    return 0.0;
  return 1.0;
}
void NotNode::printTo(ostream& os) {
	os << "(NOT " << *(factorNode) << ") ";
}
// ---------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExprNode* en) {
  _level = level - 1;
	exprPtr = en;
}
void NestedExprNode::printTo(ostream& os) {
	os << "( " << *exprPtr << ")";
}
float NestedExprNode::interpret() {
  return exprPtr->interpret();
}
NestedExprNode::~NestedExprNode() {
  if(printDelete)
    cout << "Deleting FactorNode:NestedExprNode" << endl;
  delete exprPtr;
  exprPtr = nullptr;
}
// ---------------------------------------------------------------------
TermNode::TermNode(int level) {
  _level = level - 1;
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
float TermNode::interpret()
{
  // cout << "termnode" << endl;
  float returnValue = firstFactor->interpret();
  int length = restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    float nextValue = restFactors[i]->interpret();
    switch(restFactorOps[i]) {
      case TOK_MULTIPLY:
        returnValue = returnValue * nextValue;
        break;
      case TOK_DIVIDE:
        returnValue = returnValue / nextValue;
        break;
      case TOK_AND:
        if(truth(returnValue) && truth(nextValue)) 
          returnValue = 1.0;
        returnValue = 0.0;
        break;
    }
  }
  return returnValue;
}
TermNode::~TermNode() {
  if(printDelete)
    cout << "Deleting TermNode" << endl;
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
  _level = level - 1;
}
ostream& operator<<(ostream& os, ExprNode& en) {
  os << endl; indent(en._level); os << "(expression ";
	os << *(en.firstSimpleExp);

	int length = en.restSimpleExpOps.size();
	for (int i = 0; i < length; ++i) {
		int op = en.restSimpleExpOps[i];
    if (op == TOK_PLUS) {
      os << endl; indent(en._level); os << "+ ";
    }  else if(op == TOK_MINUS) {
      os << endl; indent(en._level); os << "- ";
    } else if(op == TOK_LESSTHAN) {
      os << endl; indent(en._level); os << "< ";
    } else if(op == TOK_GREATERTHAN) {
      os << endl; indent(en._level); os << "> ";
    } else if(op == TOK_NOTEQUALTO) {
      os << endl; indent(en._level); os << "<> ";
    } else if(op == TOK_EQUALTO) {
      os << endl; indent(en._level); os << "= ";
    } else {
      os << endl; indent(en._level); os << "UNEXPECTED";
    }
		os << *(en.restSimpleExps[i]);
	}
  os << endl; indent(en._level); os << "expression) ";
	return os;
}
float ExprNode::interpret()
{
  // a Term is a Factor followed by 0 or more Factors separated by * or / operations
  // get the value of the first Factor
  float returnValue = firstSimpleExp->interpret();
  int length = restSimpleExpOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    float nextValue = restSimpleExps[i]->interpret();
    // perform the operation (* or /) that separates the Factors
    switch(restSimpleExpOps[i]) {
      case TOK_LESSTHAN:
        if(returnValue < nextValue)
          return 1.0; // true
        else
          return 0.0; // false
        break;
      case TOK_GREATERTHAN:
        if(returnValue > nextValue)
          return 1.0; // true
        else
          return 0.0; // false
        break;
        break;
      case TOK_EQUALTO:
        // Equality means the values are within EPSILON of each other.
        if(abs(returnValue - nextValue) <= EPSILON)
          return 1.0; // true
        else
          return 0.0; // false
        break;
      case TOK_NOTEQUALTO:
        // Non-equality means the values differ by at least EPSILON.
        if(abs(returnValue - nextValue) > EPSILON)
          return 1.0; // true
        else
          return 0.0; // false
        break;
    }
  }
  return returnValue;
}
ExprNode::~ExprNode() {
  if(printDelete)
    cout << "Deleting ExpressionNode" << endl;
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
  _level = level - 1;
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
float SimpleExpNode::interpret()
{
  // cout << "simpleexp" << endl;
  float returnValue = firstTerm->interpret();
  int length = restTermOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    float nextValue = restTerms[i]->interpret();
    // perform the operation (* or /) that separates the Factors
    switch(restTermOps[i]) {
      case TOK_PLUS:
      returnValue = returnValue + nextValue;
      break;
      case TOK_MINUS:
      returnValue = returnValue - nextValue;
      break;
    }
  }
  return returnValue;
}
SimpleExpNode::~SimpleExpNode() {
  // cout << "DELETING SIMPLE EXP" << endl;
  if(printDelete)
    cout << "Deleting SimpleExpNode" << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}
























StatementNode::~StatementNode() {}
ostream& operator<<(ostream& os, StatementNode& node) {
  node.printTo(os);
  return os;
}

// ---------------------------------------------------------------------
AssignmentStmtNode::AssignmentStmtNode(int level, string ident, ExprNode* expression) {
  _level = level - 1;
  this->ident = new string(ident);
  this->expression = expression;
}
AssignmentStmtNode::~AssignmentStmtNode() {
  if(printDelete)
    cout << "Deleting StatementNode:AssignmentStmtNode" << endl;
  delete ident;
  delete expression;
  expression = nullptr;
}
void AssignmentStmtNode::interpret() {
  // cout << "assignmentstmt" << endl;
  symbolTableT::iterator variable = symbolTable.find(*ident);
  if(expression != nullptr) {
    variable->second = expression->interpret();
  }
}
void AssignmentStmtNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(assignment_stmt ( " << *ident << " := )";
	os << *(expression);
  os << endl; indent(_level); os << "assignment_stmt)";
}
// ---------------------------------------------------------------------
CompoundStmtNode::CompoundStmtNode(int level) {
  _level = level - 1;

}
CompoundStmtNode::~CompoundStmtNode() {
  if(printDelete)
    cout << "Deleting StatementNode:CompoundStmtNode" << endl;

    int length = statements.size();
	for (int i = 0; i < length; ++i) {
		delete statements[i];
		statements[i] = nullptr;
	}
}
void CompoundStmtNode::interpret() {
  // cout << "compoundstmt" << endl;
  int length = statements.size();
  for(int i = 0; i < length; ++i) {
    statements[i]->interpret();
  }
}
void CompoundStmtNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(compound_stmt";
  int length = statements.size();
	for (int i = 0; i < length; ++i) {
		os << *(statements[i]);
	}
  os << endl; indent(_level); os << "compound_stmt)";
}
// ---------------------------------------------------------------------
IfStmtNode::IfStmtNode(int level, ExprNode* expression, StatementNode* thenStatement, StatementNode* elseStatement) {
  _level = level - 1;
  this->expression = expression;
  this->thenStatement = thenStatement;
  this->elseStatement = elseStatement;
}
IfStmtNode::~IfStmtNode() {
  if(printDelete)
    cout << "Deleting StatementNode:IfStmtNode" << endl;
	delete expression;
  expression = nullptr;
  delete thenStatement;
  thenStatement = nullptr;
  delete elseStatement;
  elseStatement = nullptr;
}
void IfStmtNode::interpret() {
  if(truth(expression->interpret()))
    thenStatement->interpret();
  else
    if(elseStatement != nullptr)
      elseStatement->interpret();
}
void IfStmtNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(if_stmt ";
	os << *(expression);
  os << endl; indent(_level); os << "(then ";
  os << *(thenStatement);
  os << endl; indent(_level); os << "then) ";
  if(elseStatement != nullptr) {
    os << endl; indent(_level); os << "(else ";
    os << *(elseStatement);
    os << endl; indent(_level); os << "else) ";
  }
  os << endl; indent(_level); os << "if_stmt)";
}
// ---------------------------------------------------------------------
WhileStmtNode::WhileStmtNode(int level, ExprNode* expression, StatementNode* statement) {
  _level = level - 1;
  this->expression = expression;
  this->statement = statement;
}
WhileStmtNode::~WhileStmtNode() {
  if(printDelete)
    cout << "Deleting StatementNode:WhileStmtNode" << endl;
  delete expression;
  expression = nullptr;
  delete statement;
  statement = nullptr;
}
void WhileStmtNode::interpret() {
  while (truth(expression->interpret()))
  {
    statement->interpret();
  }
}
void WhileStmtNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(while_stmt ";
  os << *(expression);
	os << *(statement);
  os << endl; indent(_level); os << "while_stmt)";
}


// ---------------------------------------------------------------------
WriteStmtNode::WriteStmtNode(int level, string ident, string literal) {
  _level = level - 1;
  this->ident = new string(ident);
  this->literal = new string(literal);
}
WriteStmtNode::~WriteStmtNode() {
  if(printDelete)
    cout << "Deleting StatementNode:WriteStmtNode" << endl;
  delete ident;
  ident = nullptr;
  delete literal;
  literal = nullptr;
}
void WriteStmtNode::interpret() {
  if(!literal->empty()) {  // stringlit
    cout << (*literal).substr(1, (*literal).length() - 2) << endl;
  }
  else {
    symbolTableT::iterator variable = symbolTable.find((*ident));
    cout << variable->second << endl;
  }
}
void WriteStmtNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(write_stmt ";
  if(ident->empty()) os << "( " << *(literal) << " )";
  if(literal->empty()) os << "( " << *(ident) << " )";
  os << endl; indent(_level); os << "write_stmt)";
}

// ---------------------------------------------------------------------
ReadStmtNode::ReadStmtNode(int level, string ident) {
  _level = level - 1;
  this->ident = new string(ident);
}
ReadStmtNode::~ReadStmtNode() {
  if(printDelete)
    cout << "Deleting StatementNode:ReadStmtNode" << endl;
  delete ident;
  ident = nullptr;
}
void ReadStmtNode::interpret() {
  symbolTableT::iterator variable = symbolTable.find(*ident);
  string temp;
  getline(std::cin, temp);
  variable->second = atof(temp.c_str());
}
void ReadStmtNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(read_stmt ( " << *ident << " )";
  os << endl; indent(_level); os << "read_stmt)";
}




























// ---------------------------------------------------------------------
BlockNode::BlockNode(int level) {
  _level = level - 1;
}
ostream& operator<<(ostream& os, BlockNode& bn) {
  os << endl; indent(bn._level); os << "(block ";

	int length = bn.statements.size();
	for (int i = 0; i < length; ++i) {
		os << *(bn.statements[i]);
	}

  os << endl; indent(bn._level); os << "block) ";
	return os;
}
void BlockNode::interpret() {
  int length = statements.size();
  for(int i = 0; i < length; ++i) {
    statements[i]->interpret();
  }
}
BlockNode::~BlockNode() {
  // cout << "DELETING SIMPLE EXP" << endl;
  if(printDelete)
    cout << "Deleting BlockNode" << endl;

	int length = statements.size();
	for (int i = 0; i < length; ++i) {
		delete statements[i];
		statements[i] = nullptr;
	}
}

// ---------------------------------------------------------------------
ProgramNode::ProgramNode(int level) {
  _level = level - 1;
}
ostream& operator<<(ostream& os, ProgramNode& pn) {
  os << endl; indent(pn._level); os << "(program ";
	os << *(pn.block);
  os << endl; indent(pn._level); os << "program) ";
	return os;
}
void ProgramNode::interpret() {
  block->interpret();
}
ProgramNode::~ProgramNode() {
  // cout << "DELETING SIMPLE EXP" << endl;
  if(printDelete)
    cout << "Deleting ProgramNode" << endl;

	delete block;
	block = nullptr;

}