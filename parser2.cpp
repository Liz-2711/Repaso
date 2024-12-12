#include <stdexcept>
#include "parser.hpp"
#include "ast.hpp"

// Token-to-string conversion
const char* tokenToString(Token token) {
    switch (token) {
        case Token::Eof: return "Eof";
        case Token::CloseBracket: return "CloseBracket";
        case Token::Ident: return "Ident";
        case Token::OpenPar: return "OpenPar";
        case Token::IntConst: return "IntConst";
        case Token::OpSub: return "OpSub";
        case Token::SemiColon: return "SemiColon";
        case Token::OpenBracket: return "OpenBracket";
        case Token::Comma: return "Comma";
        case Token::OpMod: return "OpMod";
        case Token::ClosePar: return "ClosePar";
        case Token::OpDiv: return "OpDiv";
        case Token::OpMul: return "OpMul";
        case Token::OpAdd: return "OpAdd";
        case Token::GREATER: return "GREATER";
        case Token::GREATER_EQUAL: return "GREATER_EQUAL";
        case Token::LESSTHAN: return "LESSTHAN";
        case Token::NOT: return "NOT";
        case Token::OR: return "OR";
        case Token::LESS_EQUAL: return "LESS_EQUAL";
        case Token::NOTEQUAL: return "NOTEQUAL";
        case Token::EQUAL: return "EQUAL";
        case Token::AND: return "AND";
        case Token::OpenCurly: return "OpenCurly";
        case Token::CloseCurly: return "CloseCurly";
        case Token::KwPrint: return "KwPrint";
        case Token::KwRead: return "KwRead";
        case Token::KwIf: return "KwIf";
        case Token::KwReturn: return "KwReturn";
        case Token::KwRef: return "KwRef";
        case Token::KwInt: return "KwInt";
        case Token::KwVoid: return "KwVoid";
        case Token::KwClass: return "KwClass";
        case Token::KwWhile: return "KwWhile";
        case Token::KwElse: return "KwElse";
        case Token::Comillas: return "Comillas";
        case Token::doubleEqual: return "doubleEqual";
        case Token::Hexa: return "Hexa";
        case Token::Bin: return "Bin";
        default: return "Unknown Token";
    }
}

// Main Parse Function
AstNode* Parser::parse() {
    curr_tk = lex.getNextToken();
    std::vector<AstNode*> classes;

    while (curr_tk != Token::Eof) {
        classes.push_back(program());
    }

    return new ProgramNode(classes);
}

// Parse Program
AstNode* Parser::program() {
    if (curr_tk != Token::KwClass) {
        throw std::runtime_error("Expected 'class' keyword.");
    }

    curr_tk = lex.getNextToken();

    if (curr_tk != Token::Ident) {
        throw std::runtime_error("Expected class identifier.");
    }

    std::string className = lex.getText();
    curr_tk = lex.getNextToken();

    if (curr_tk != Token::OpenCurly) {
        throw std::runtime_error("Expected '{' after class declaration.");
    }
    curr_tk = lex.getNextToken();

    std::vector<AstNode*> members;
    while (curr_tk != Token::CloseCurly && curr_tk != Token::Eof) {
        members.push_back(VarDcl());
    }

    if (curr_tk != Token::CloseCurly) {
        throw std::runtime_error("Expected closing '}' at end of class.");
    }
    curr_tk = lex.getNextToken();

    return new ClassNode(className, members);
}

// Variable Declaration
AstNode* Parser::VarDcl() {
    methodType();

    if (curr_tk != Token::Ident) {
        throw std::runtime_error("Expected identifier.");
    }

    std::string varName = lex.getText();
    curr_tk = lex.getNextToken();

    // If it's a method declaration
    if (curr_tk == Token::OpenPar) {
        curr_tk = lex.getNextToken();
        std::vector<AstNode*> params = paramList();

        if (curr_tk != Token::ClosePar) {
            throw std::runtime_error("Expected ')' after parameters.");
        }
        curr_tk = lex.getNextToken();

        if (curr_tk != Token::OpenCurly) {
            throw std::runtime_error("Expected '{' after method declaration.");
        }
        curr_tk = lex.getNextToken();

        std::vector<AstNode*> methodBody;
        while (curr_tk != Token::CloseCurly && curr_tk != Token::Eof) {
            methodBody.push_back(stmt());
        }

        if (curr_tk != Token::CloseCurly) {
            throw std::runtime_error("Expected '}' at end of method.");
        }
        curr_tk = lex.getNextToken();

        return new MethodNode(varName, "void", params, new BlockNode(methodBody));
    }

    // If it's a variable declaration
    std::vector<std::pair<std::string, AstNode*>> variables;
    variables.emplace_back(varName, nullptr);

    while (curr_tk == Token::Comma) {
        curr_tk = lex.getNextToken();

        if (curr_tk != Token::Ident) {
            throw std::runtime_error("Expected identifier after ','.");
        }

        variables.emplace_back(lex.getText(), nullptr);
        curr_tk = lex.getNextToken();
    }

    if (curr_tk != Token::SemiColon) {
        throw std::runtime_error("Expected ';' after variable declaration.");
    }
    curr_tk = lex.getNextToken();

    return new VarDeclNode("int", variables);
}

// Method Type
void Parser::methodType() {
    if (curr_tk == Token::KwInt) {
        curr_tk = lex.getNextToken();
        while (curr_tk == Token::OpenBracket) {
            curr_tk = lex.getNextToken();
            if (curr_tk != Token::IntConst) {
                throw std::runtime_error("Expected constant size for array.");
            }
            curr_tk = lex.getNextToken();
            if (curr_tk != Token::CloseBracket) {
                throw std::runtime_error("Expected closing ']' for array size.");
            }
            curr_tk = lex.getNextToken();
        }
    } else if (curr_tk == Token::KwVoid) {
        curr_tk = lex.getNextToken();
    } else {
        throw std::runtime_error("Expected 'int' or 'void'.");
    }
}

// Parameter List
std::vector<AstNode*> Parser::paramList() {
    std::vector<AstNode*> params;
    if (curr_tk == Token::ClosePar) {
        return params; // No params
    }

    params.push_back(ParamDecl());

    while (curr_tk == Token::Comma) {
        curr_tk = lex.getNextToken();
        params.push_back(ParamDecl());
    }

    return params;
}

// Parameter Declaration
AstNode* Parser::ParamDecl() {
    if (curr_tk == Token::KwRef) {
        curr_tk = lex.getNextToken();
    }
    methodType();

    if (curr_tk != Token::Ident) {
        throw std::runtime_error("Expected parameter identifier.");
    }

    std::string paramName = lex.getText();
    curr_tk = lex.getNextToken();

    return new VarDeclNode("int", { {paramName, nullptr} });
}

// Statements
AstNode* Parser::stmt() {
    if (curr_tk == Token::KwRead) return read_stmt();
    if (curr_tk == Token::KwPrint) return print_stmt();
    if (curr_tk == Token::KwReturn) return return_stmt();
    if (curr_tk == Token::KwIf) return if_stmt();
    if (curr_tk == Token::KwWhile) return while_stmt();
    if (curr_tk == Token::Ident) return assign_stmt();

    throw std::runtime_error("Unexpected statement.");
}
