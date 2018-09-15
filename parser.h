#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <iostream>
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

/*
current EBNF (Extended Backus-Naur Form)

expr -> term [ ('+' | '-') term ]*
term -> exponent [ ('*' | '/') exponent ]*
exponent -> factor [ '^' exponent ]*
factor ->  (function)exponent | number | variable | LPAR expr RPAR
*/

typedef double (*PTRFX)(double, double);

double Sum(double op1, double op2) { return (op1 + op2); }
double Subtraction(double op1, double op2) { return (op1 - op2); }
double Multiplication(double op1, double op2) { return (op1 * op2); }
double Division(double op1, double op2) { return (op1 / op2); }
double Pow(double op1, double op2) { return (pow(op1, op2)); }
double UMINUS(double op1, double op2) { return (-op1); }
double UPLUS(double op1, double op2) { return (+op1); }

std::map<std::string, PTRFX> mapfunc = {
    {"+", &::Sum},      {"-", &::Subtraction}, {"*", &::Multiplication},
    {"/", &::Division}, {"^", &::Pow},         {"u-", &::UMINUS},
    {"u+", &::UPLUS}};

std::map<std::string, std::string> vars = {};

class Node {
public:
  virtual std::string getValue() = 0;
  virtual double exeValue() = 0;
};

class ConstNode : public Node {
private:
  std::string value;

public:
  ConstNode(std::string value) : value(value){};
  std::string getValue() { return this->value; }
  double exeValue() { return (std::stod(this->value)); }
};

class VarNode : public Node {
private:
  std::string variable;
  std::string value;

public:
  VarNode(std::string variable) : variable(variable){};
  std::string getValue() { return this->variable; }

  double exeValue() {
    if (::vars.find(this->variable) == ::vars.end()) {

      std::cout << "Enter " << this->variable << " value: ";

      std::cin >> this->value;
      std::cout << std::endl;
      ::vars[variable] = this->value;
    } else {
      this->value = ::vars[variable];
    }
    return (std::stod(this->value));
  }
};

class BNode : public Node {
private:
  std::string op;
  PTRFX pfx;

public:
  Node *left;
  Node *right;

  BNode(Node *left, Node *right, std::string op)
      : op(op), pfx(mapfunc.find(op)->second), left(left), right(right) {}
  std::string getValue() {
    std::ostringstream os;
    os << "(" << this->left->getValue() << " " << op << " "
       << this->right->getValue() << ")";

    return os.str();
  }
  double exeValue() {
    return ((*pfx)(this->left->exeValue(), this->right->exeValue()));
  }
};

class UNode : public Node {
private:
  Node *child;
  std::string function;
  PTRFX pfx;

public:
  UNode(Node *child, std::string function) : child(child), function(function) {

    if (function == "+" || function == "-") {
      std::ostringstream uos;
      uos << "u" << function;
      pfx = mapfunc.find(uos.str())->second;
    }
  }
  std::string getValue() {
    std::ostringstream os;

    os << "(" << function << "(" << child->getValue() << ")"
       << ")";
    return os.str();
  }
  double exeValue() { return ((*pfx)(this->child->exeValue(), 0)); }
};

class Parser {
private:
  Node *AST;

  std::vector<Token> tokenized_text;
  Token current_tkn;
  Interpreter *lexer;

public:
  Parser(std::string text) {
    this->lexer = new Interpreter(text);
    this->current_tkn = lexer->get_next_token();
    this->AST = Parse();
  }
  // void error();
  void eat(std::string token_type) {

    if (this->current_tkn.type == token_type) {
      std::cout << "this->current_tkn.type:  " << this->current_tkn.type
                << "  does match with token_type: " << token_type << std::endl;
      this->current_tkn = this->lexer->get_next_token();
    } else {

      std::cout << "this->current_tkn.type:  " << this->current_tkn.type
                << "  doesn't match with token_type: " << token_type
                << std::endl;
    }
  }

  Node *factor() {

    Token tkn = this->current_tkn;
    if (tkn.type == "OP_+") {
      eat("OP_+");
      return (new UNode(this->exponent(), tkn.value));
    } else if (tkn.type == "OP_-") {
      eat("OP_-");
      return (new UNode(this->exponent(), tkn.value));
    } else if (tkn.type == "NUMBER") {
      eat("NUMBER");
      return (new ConstNode(tkn.value));
    } else if (tkn.type == "VAR") {
      eat("VAR");
      return (new VarNode(tkn.value));
    } else if (tkn.type == "LPAR") {
      eat("LPAR");
      Node *node = this->expr();
      eat("RPAR");
      return node;
    } else {
      std::cout << "error" << std::endl;
      Node *errorNode = nullptr;
      return errorNode;
    }
  }
  Node *exponent() {
    Node *node = factor();
    Token tkn = this->current_tkn;

    while (this->current_tkn.type == "OP_^") {
      Token tkn = this->current_tkn;
      if (tkn.type == "OP_^") {
        eat("OP_^");
      }

      node = new BNode(node, this->exponent(), tkn.value);
    }
    return node;
  }
  Node *term() {
    Node *node = this->exponent();
    while (current_tkn.type == "OP_*" || current_tkn.type == "OP_/") {
      Token tkn = this->current_tkn;
      eat(tkn.type);
      node = new BNode(node, this->exponent(), tkn.value);
    }
    return node;
  }
  Node *expr() {
    Node *node = this->term();
    while (current_tkn.type == "OP_+" || current_tkn.type == "OP_-") {
      Token tkn = this->current_tkn;
      eat(tkn.type);
      node = new BNode(node, this->term(), tkn.value);
    }
    return node;
  }
  Node *Parse() { return this->expr(); }
  void visualize() {
    this->lexer->print();
    std::cout << this->AST->getValue() << std::endl;
  }
  void execute() { std::cout << this->AST->exeValue() << std::endl; }
};

#endif
