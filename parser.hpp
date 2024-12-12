#ifndef _PARSER_H
#define _PARSER_H

#include "lexer2.hpp"

class Parser{ 
public:
    Parser(Lexer& lex): lex(lex) {}

    void parse();

    private:

    
    void boolExpr();
    void boolTerm();
    void boolFactor();
    void relationExpr();
    void arithExpr();
    void term();
    void factor();
    void primary();
    void program();
    void VarDcl();
    void methodType();
    void methodDecl();
    void stmt();
    void read_stmt();
    void print_stmt();
    void return_stmt();
    void while_stmt();
    void if_stmt();
    void call_stmt();
    void assign_stmt();
    void ParamDecl();
    void paramList();
    void block();
    void decl();
    



   
   
    private:
    Lexer& lex;
    Token curr_tk;

};
#endif

