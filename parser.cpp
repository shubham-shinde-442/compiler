#include "parser.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "lexer.h"
using namespace std;

map<string, int> VariableNode::memLoc;
int VariableNode::counter = 1;

NumberNode::NumberNode(int value) : value(value) {}

void NumberNode::generateCode(ostream &out) {
    out << "ldi A " << value << endl;
}

void NumberNode::generateL(ostream &out) {
    out << "ldi A " << value << endl;
}

void NumberNode::generateR(ostream &out) {
    out << "ldi B " << value << endl;
}

VariableNode::VariableNode(string name) : name(name) {
    if(memLoc.find(name) == memLoc.end()) {
        memLoc[name] = counter++;
    }
    id = memLoc[name];
}

void VariableNode::generateCode(ostream &out) {
    out << "mov M A " << memLoc[name] << endl;
}

void VariableNode::generateL(ostream &out) {
    out << "mov A M " << memLoc[name] << endl;
}

void VariableNode::generateR(ostream &out) {
    out << "mov B M " << memLoc[name] << endl;
}

BinaryOpNode::BinaryOpNode(shared_ptr<ASTNode> l, char op, shared_ptr<ASTNode> r) : l(l), op(op), r(r) {}

void BinaryOpNode::generateCode(ostream &out) {
    l->generateL(out);
    r->generateR(out);

    switch(op) {
    case '+':
        out << "add" << endl;
    case '-':
        out << "sub" << endl;
    case '#':
        out << "cmp" << endl;
    default:
        throw runtime_error("Unsupported Binary Op");
    }
}

VariableDec::VariableDec(string varN, shared_ptr<ASTNode> value) : varN(varN), value(value) {
    if(memLoc.find(varN) == memLoc.end()) {
        VariableNode::memLoc[varN] = VariableNode::counter++;
    }
}

void VariableDec::generateCode(ostream &out) {
    if(memLoc.find(varN) == memLoc.end()) {
        memLoc[varN] = counter++;
    }

    value->generateCode(out);
    out << "mov M A " << memLoc[varN] << endl;
}

ConditionalNode::ConditionalNode(shared_ptr<ASTNode> cond, shared_ptr<ASTNode> then_br, shared_ptr<ASTNode> else_br)
    : cond(cond), then_br(then_br), else_br(else_br) {}

void ConditionalNode::generateCode(ostream &out) {
    cond->generateCode(out);
    out << "jnz %ELSEBR" << endl;

    then_br->generateCode(out);
    out << "jmp &OUTOFIF" << endl;

    out << "ELSEBR:" << endl;

    if(else_br) {
        else_br->generateCode(out);
    }

    out << "OUTOFIF:" << endl;
}

void BlockNode::addStat(shared_ptr<ASTNode> statement) {
    statements.push_back(statement);
}

void BlockNode::generateCode(ostream &out) {
    for(auto each : statements) {
        each->generateCode(out);
    }
}

Parser::Parser(vector<Token>& tokens) : tokens(tokens), pos(0) {} // start pos from 0 and put passed source to our source when constructor called

shared_ptr<ASTNode> Parser::parse() {
    shared_ptr<BlockNode> bNode = make_shared<BlockNode>();
    while(!isAtEnd()) { // until it not ends
        bNode->addStat(parseStat());
    }

    return bNode;
}

shared_ptr<ASTNode> Parser::parseStat() {
    if(match({tokenType::INT})) {
        return parseVarDec();
    } else if(match({tokenType::IF})) {
        return parseCond();
    } else if(match({tokenType::IDENTIFIER})) {
        return parseVarAssign();
    } else {
        return parseExpStat();
    }
}

shared_ptr<ASTNode> Parser::parseVarAssign() {
    string varN = previous().value;

    // Checks
    if(VariableNode::memLoc.find(varN) == VariableNode::memLoc.end()) {
        throw runtime_error("Semantic Error : variable " + varN + "not declared");
    }

    consume({tokenType::EQUAL}, "expect = ");
    shared_ptr<ASTNode> value = parseExp();
    consume({tokenType::SEMICOLON}, "expect ; ");
    return make_shared<VariableDec>(varN, value);
}

shared_ptr<ASTNode> Parser::parseVarDec() {
    consume({tokenType::IDENTIFIER}, "expect var");

    //Checks
    if(VariableNode::memLoc.find(previous().value) != VariableNode::memLoc.end()) {
        throw runtime_error("Semantic Error: Variable" + previous().value + "already declared");
    }

    string varN = previous().value;
    shared_ptr<ASTNode> value = nullptr;

    if(match({tokenType::EQUAL})) {
        value = parseExp();
    } else {
        consume({tokenType::SEMICOLON}, "expect ;");
        return make_shared<VariableNode>(varN);
    }

    consume({tokenType::SEMICOLON}, "expect ;");
    return make_shared<VariableDec>(varN, value);
}

shared_ptr<ASTNode> Parser::parseCond() {
    consume({tokenType::LPAREN}, "expect (");
    shared_ptr<ASTNode> cond1 = parseExp();
    consume({tokenType::RPAREN}, "expect )");

    shared_ptr<ASTNode> then_br1 = parseBlock();
    shared_ptr<ASTNode> else_br1 = nullptr;

    if(match({tokenType::ELSE})) {
        else_br1 = parseBlock();
    }

    return make_shared<ConditionalNode>(cond1, then_br1, else_br1);
}

shared_ptr<ASTNode> Parser::parseBlock() {
    if(match({tokenType::LBRACE})) {
        shared_ptr<BlockNode> b = make_shared<BlockNode>();

        while(!check(tokenType::RBRACE) && !isAtEnd()) {
            b->addStat(parseStat());
        }

        consume({tokenType::RBRACE}, "expect }");
        return b;
    } else {
        return parseStat();
    }
}

shared_ptr<ASTNode> Parser::parseExpStat() {
    shared_ptr<ASTNode> exp = parseExp();
    consume({tokenType::SEMICOLON}, "expect ;");
    return exp;
}

shared_ptr<ASTNode> Parser::parseExp() {
    shared_ptr<ASTNode> l = parsePrim();

    while(match({tokenType::PLUS, tokenType::MINUS, tokenType::DOUBLEEQUAL})) {
        char op = previous().value[0];
        shared_ptr<ASTNode> r = parsePrim();
        l = make_shared<BinaryOpNode>(l, op, r);
    }
    return l;
}

shared_ptr<ASTNode> Parser::parsePrim() {
    if(match({tokenType::NUM})) {
        return make_shared<NumberNode>(stoi(previous().value));
    } else if(match({tokenType::IDENTIFIER})) {
        return make_shared<VariableNode>(previous().value);
    } else if(match({tokenType::LPAREN})) {
        shared_ptr<ASTNode> exp = parseExp();
        consume({tokenType::RPAREN}, "expect )");
        return exp;
    }

    cout << tokens[pos].value << endl;
    throw runtime_error("unexpected token ");
}

bool Parser::match(vector<tokenType> types) {
    auto it = find_if(types.begin(), types.end(), [&](tokenType type) {
        return type == tokens[pos].type;
    });

    if (it != types.end()) {
        pos++;
        return true;
    }

    return false;
}


bool Parser::isAtEnd() {
    return pos >= tokens.size() || tokens[pos].type == tokenType::END;
}

bool Parser::check(tokenType type) {
    if(isAtEnd()) {
        return false;
    }
    return tokens[pos].type == type;
}

Token Parser::previous() {
    return  tokens[pos - 1];
}

void Parser::advance() {
    if(!isAtEnd()) {
        pos++;
        return;
    }
}

void Parser::consume(tokenType type, string errMessage) {
    if(check(type)) {
        advance();
    } else {
        throw runtime_error("Wrong token present");
    }
}
