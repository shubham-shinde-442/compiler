#include "lexer.h"
#include <iostream>
using namespace std;

Lexer::Lexer(string& source) : source(source), pos(0) {}  // just to start from pos 0 when constructor called

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;

    while(pos < source.length()) {
        char cur = source[pos];
        if(isspace(cur)) {   // Ignoring spaces
            cout << "This is space" << endl;  // just for debugging
            pos++;
            continue;
        }

        if(isalpha(cur)) {
            string identifier;

            while(isalnum(cur)) { // while loop continue till identifier name ends and identifer may be include nums too
                cout << "Identifier continues" << endl;
                identifier += cur;
                pos++;
                if(pos >= source.length()) {
                    break;
                }

                cur = source[pos];
            }

            if(identifier == "int") {  // Might looks good with switch, but will do this later
                tokens.push_back({tokenType::INT, "int"});
            } else if(identifier == "if") {
                tokens.push_back({tokenType::IF, "if"});
            } else if(identifier == "else") {
                tokens.push_back({tokenType::ELSE, "else"});
            } else {
                tokens.push_back({tokenType::IDENTIFIER, identifier});
            }

        } else if (isdigit(cur)) {
            string num;
            while(isdigit(cur)) { // continue to check for nums because e.g 1310
                num += cur;
                pos++;

                if(pos >= source.length()) {
                    break;
                }

                cur = source[pos];
            }
            tokens.push_back({tokenType::NUM, num});

        } else {
            switch(cur) {
            case '=':
                pos++;
                if(pos < source.length() && source[pos] == '=') {  // checking if there is double equal i.e ==
                    tokens.push_back({tokenType::DOUBLEEQUAL, "#"});
                    pos++;
                } else {
                    tokens.push_back({tokenType::EQUAL, "="});
                    pos--;
                }
                break;
            case '+':
                tokens.push_back({tokenType::PLUS, "+"});
                break;
            case '-':
                tokens.push_back({tokenType::MINUS, "-"});
                break;
            case '(':
                tokens.push_back({tokenType::LPAREN, "("});
                break;
            case ')':
                tokens.push_back({tokenType::RPAREN, ")"});
                break;
            case '{':
                tokens.push_back({tokenType::LBRACE, "{"});
                break;
            case '}':
                tokens.push_back({tokenType::RBRACE, "}"});
                break;
            case ';':
                tokens.push_back({tokenType::SEMICOLON, ";"});
                break;
            default:
                cerr << "Not Known Char :" << cur << endl; // for other char show error
            }
            pos++;
        }
    }

    tokens.push_back({tokenType::END, ""});
    return tokens;
}
