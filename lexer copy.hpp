#ifndef _EXPR_LEXER_H
#define _EXPR_LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include "tokens.hpp"

class Lexer {
public:
    Lexer(std::ifstream& in): in(in), line(1) {}

    Token getNextToken();
    unsigned getLineNo() { return line; }
    std::string getText() { return text; }
    static const char *tokenToString(Token tk);

private:
    
    Token findKw(const std::string& txt) {
     
     
   std::cout << "Checking keyword: " << txt << std::endl;  
    if (txt == "print")    return Token::KwPrint;
    else if (txt == "read") return Token::KwRead;
    else if (txt == "if") return Token::KwIf;
    else if (txt == "while") return Token::KwWhile;
    else if (txt == "return") return Token::KwReturn;
    else if (txt == "ref") return Token::KwRef;
    else if (txt == "int") return Token::KwInt;
    else if (txt == "void") return Token::KwVoid;
    else if (txt == "class")  return Token::KwClass;
    else if (txt == "else") return Token::KwElse;
    else return Token::Ident;
}
    

    int getNextChar() {
        int ch = in.get();
        if (ch == '\n') {
            line++;
        }
        return ch;
    }

    void ungetChar(int ch) {
        if (ch == '\n') {
            line--;
        }
        in.unget();
    }

    void reportError(int ch) {
        std::cerr << "Line " << line << ": Invalid Token '" << (char)ch << "'\n";
    }

private:
    std::string text;
    std::ifstream& in;
    int line;
};
#endif
