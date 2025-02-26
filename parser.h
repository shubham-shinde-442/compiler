#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "lexer.h"
using namespace std;

class ASTNode {
public:
    virtual void generateCode(ostream& out) = 0;  //virtual bcoz method will be different for other methods.
    virtual void generateL(ostream& out) = 0;
    virtual void generateR(ostream& out) = 0;
};

class NumberNode : public ASTNode { // for nums
public:
    int value;
    NumberNode(int value);
    void generateCode(ostream &out);
    void generateL(ostream &out);
    void generateR(ostream &out);
};

class VariableNode : public ASTNode {
public:
    static map<string, int> memLoc;
    static int counter;
    string name;
    int id;

    VariableNode(string name); //Used two types of constructor
    VariableNode() {}
    void generateCode(ostream &out);
    void generateR(ostream &out);
    void generateL(ostream &out);
};

class VariableDec : public VariableNode { // var declaration
public:
    string varN;
    shared_ptr<ASTNode> value;
    VariableDec(string varN, shared_ptr<ASTNode> value);
    void generateCode(ostream &out);
    void generateL(ostream &out) {}
    void generateR(ostream &out) {}
};

class BinaryOpNode : public ASTNode { // for binary operations
public:
    shared_ptr<ASTNode> l, r;
    char op;
    BinaryOpNode(shared_ptr<ASTNode> l, char op, shared_ptr<ASTNode> r);
    void generateCode(ostream &out);
    void generateL(ostream &out) {}
    void generateR(ostream &out) {}
};

class ConditionalNode : public ASTNode { // for conditions e.g if, else
public:
    shared_ptr<ASTNode> cond, then_br, else_br;
    ConditionalNode(shared_ptr<ASTNode> cond, shared_ptr<ASTNode> then_br, shared_ptr<ASTNode> else_br);
    void generateCode(ostream &out);
    void generateL(ostream &out) {}
    void generateR(ostream &out) {}
};

class BlockNode : public ASTNode { // for block of code
public:
    vector<shared_ptr<ASTNode>> statements;
    BlockNode() {}
    void addStat(shared_ptr<ASTNode> statement);
    void generateCode(ostream &out);
    void generateL(ostream &out) {}
    void generateR(ostream &out) {}
};

class Parser
{
public:
    Parser(vector<Token>& tokens);
    shared_ptr<ASTNode> parse();

private:
    vector<Token> tokens;
    size_t pos;

    shared_ptr<ASTNode> parseExp();
    shared_ptr<ASTNode> parseExpStat();
    shared_ptr<ASTNode> parsePrim();
    shared_ptr<ASTNode> parseStat();
    shared_ptr<ASTNode> parseVarDec();
    shared_ptr<ASTNode> parseVarAssign();
    shared_ptr<ASTNode> parseCond();
    shared_ptr<ASTNode> parseBlock();

    bool match(vector<tokenType> types);
    bool isAtEnd();
    bool check(tokenType type);
    void advance();
    Token previous();
    void consume(tokenType type, string);
};

#endif // PARSER_H
