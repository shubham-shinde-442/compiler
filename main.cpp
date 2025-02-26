#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ifstream inputFile("input.txt");
    if(!inputFile.is_open()) {
        cerr << "Could Not Open Input File "; // just for debugging purpose
    }

    ofstream outputFile("output.asm");
    if(!outputFile.is_open()) {
        cerr << "Could Not Open Output File ";
    }

    string source((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    // for(const auto i : source) {
    //     cout << i << endl;  // just to check if this works
    // }

    inputFile.close();

    //Now lexer part
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    for(const auto i : tokens) {
        cout << "Token val :" << i.value << " " << "Token Num : " << static_cast<int>(i.type) << endl;
    }

    //Parser Part
    Parser parser(tokens); // calls constuctor
    shared_ptr<ASTNode> ast = parser.parse();

    if(ast) {
        cout << "parsing starts" << endl;
        outputFile << ".text" << endl;
        ast->generateCode(outputFile);
    } else {
        cerr << "Parsing Failed" << endl;
    }

    outputFile << "hlt" << endl;
    outputFile.close();

    return a.exec();
}
