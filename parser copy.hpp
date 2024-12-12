#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.hpp"
#include "Ast.hpp"
#include <string>
#include <vector>

class Parser {
public:
    Parser(Lexer& lex): lex(lex) {}
    AstNode* parse();

private:
    // Grammar-based methods
    AstNode* program();    
    AstNode* decla();      
    AstNode* method_decl();
    AstNode* method_type(std::string &outType);
    AstNode* variable_decl();
    AstNode* type(std::string &outType);

    AstNode* opt_param_decl_list(std::vector<AstNode*>& params);
    AstNode* param_decl(std::vector<AstNode*>& params);

    AstNode* stmt();
    AstNode* assign_stmt(const std::string& varName);
    AstNode* if_stmt();
    AstNode* while_stmt();
    AstNode* call_stmt(const std::string& funcName);
    AstNode* return_stmt();
    AstNode* print_stmt();
    AstNode* read_stmt();
    AstNode* block();

    AstNode* expression(); // equals boolean_expression by grammar, but we can unify
    AstNode* boolean_expression();
    AstNode* boolean_term();
    AstNode* boolean_factor();
    AstNode* relational_expression();
    AstNode* arithmetic_expression();
    AstNode* term();
    AstNode* factor();
    AstNode* primary();

private:
    Lexer& lex;
    Token curr_tk;
};

#endif
