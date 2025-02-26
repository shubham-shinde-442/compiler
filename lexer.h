#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
using namespace std;

enum class tokenType {
    INT,
    IDENTIFIER,
    ASSIGN,
    NUM,
    SEMICOLON,
    PLUS,
    MINUS,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    IF,
    ELSE,
    EQUAL,
    DOUBLEEQUAL,  // Actually it is  ==
    END,
};

struct Token {
    tokenType type;
    string value;
};

class Lexer
{
public:
    Lexer(string& source);
    vector<Token> tokenize();

private:
    string& source;
    size_t pos;
};

#endif // LEXER_H
