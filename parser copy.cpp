#include <stdexcept>
#include "parser.hpp"
#include "Ast.hpp"
 
AstNode* Parser::parse() {
    curr_tk = lex.getNextToken();
    if (curr_tk == Token::Eof) {
        throw std::runtime_error("Empty input");
    }
    AstNode* root = program();
    if (curr_tk != Token::Eof) {
        throw std::runtime_error("Unexpected tokens after program end");
    }
    return root;
}

// program -> KW_CLASS IDENTIFIER OPEN_CURLY decla* CLOSE_CURLY
AstNode* Parser::program() {
    if (curr_tk != Token::KwClass) {
        throw std::runtime_error("Expected 'class' keyword");
    }
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::Ident) {
        throw std::runtime_error("Expected identifier after 'class'");
    }
    std::string className = lex.getText();
    curr_tk = lex.getNextToken();

    if (curr_tk != Token::OpenCurly) {
        throw std::runtime_error("Expected '{' after class name");
    }
    curr_tk = lex.getNextToken();

    std::vector<AstNode*> declarations;
    while (curr_tk != Token::CloseCurly && curr_tk != Token::Eof) {
        declarations.push_back(decla());
    }

    if (curr_tk != Token::CloseCurly) {
        throw std::runtime_error("Expected '}' at end of class body");
    }
    curr_tk = lex.getNextToken();

    return new ProgramNode(className, declarations);
}

// decla -> method_type IDENTIFIER
//          [ ( COMMA IDENTIFIER )* SEMICOLON
//          | OPEN_PAR opt_param_decl_list CLOSE_PAR
//            OPEN_CURLY ( variable_decl )* ( stmt )* CLOSE_CURLY ]
AstNode* Parser::decla() {
    std::string t;
    method_type(t);

    if (curr_tk != Token::Ident) {
        throw std::runtime_error("Expected identifier after type");
    }
    std::string name = lex.getText();
    curr_tk = lex.getNextToken();

    if (curr_tk == Token::Comma || curr_tk == Token::SemiColon) {
        // This is a variable declaration with possibly multiple variables
        // type ident (COMMA ident)* SEMICOLON
        std::vector<AstNode*> vars;
        vars.push_back(new VarDeclNode(t, name));
        while (curr_tk == Token::Comma) {
            curr_tk = lex.getNextToken();
            if (curr_tk != Token::Ident) {
                throw std::runtime_error("Expected identifier after ','");
            }
            std::string anotherName = lex.getText();
            curr_tk = lex.getNextToken();
            vars.push_back(new VarDeclNode(t, anotherName));
        }
        if (curr_tk != Token::SemiColon) {
            throw std::runtime_error("Expected ';' after variable declarations");
        }
        curr_tk = lex.getNextToken();
        return new BlockNode(vars); // Return them as a block or a single node

    } else if (curr_tk == Token::OpenPar) {
        // method_decl scenario
        // decla is actually front part of method_decl
        // method_decl -> decla OPEN_PAR opt_param_decl_list CLOSE_PAR ...
        // We have type, name, and now '(' is next
        // consume '('
        curr_tk = lex.getNextToken();
        std::vector<AstNode*> params;
        if (curr_tk != Token::ClosePar) {
            opt_param_decl_list(params);
        }
        if (curr_tk != Token::ClosePar) {
            throw std::runtime_error("Expected ')' after parameters");
        }
        curr_tk = lex.getNextToken();

        if (curr_tk != Token::OpenCurly) {
            throw std::runtime_error("Expected '{' after method declaration header");
        }
        curr_tk = lex.getNextToken();

        // (variable_decl)* (stmt)* until '}'
        std::vector<AstNode*> bodyStmts;
        // variable_decl repeated zero or more times:
        while (curr_tk == Token::KwInt) {
            bodyStmts.push_back(variable_decl());
        }

        // (stmt)* until '}'
        while (curr_tk != Token::CloseCurly && curr_tk != Token::Eof) {
            bodyStmts.push_back(stmt());
        }
        if (curr_tk != Token::CloseCurly) throw std::runtime_error("Expected '}' at end of method");
        curr_tk = lex.getNextToken();

        AstNode* bodyBlock = new BlockNode(bodyStmts);
        return new MethodDeclNode(t, name, params, bodyBlock);

    } else {
        throw std::runtime_error("Expected ',' ';' or '(' after type and identifier in decla");
    }
}

// method_decl is encompassed by decla in this grammar
// Actually method_decl is given by grammar, but it references decla as a part of it.
// We've effectively merged method_decl logic into decla because method_decl -> decla OPEN_PAR...
// Already handled above. If you strictly want method_decl separate, you'd factor the code differently.
// But the grammar states method_decl -> decla ... 
// We handled that scenario in decla when we saw '('. If you prefer a separate function method_decl(), you'd call it from decla().
// For simplicity, we've fully handled method declarations in decla() according to grammar.

// method_type -> KW_INT [ OPEN_BRACKET CONSTANT CLOSE_BRACKET ]* | KW_VOID
AstNode* Parser::method_type(std::string &outType) {
    if (curr_tk == Token::KwInt) {
        outType = "int";
        curr_tk = lex.getNextToken();
        while (curr_tk == Token::OpenBracket) {
            curr_tk = lex.getNextToken();
            if (curr_tk == Token::IntConst || curr_tk == Token::Hexa || curr_tk == Token::Bin) {
                curr_tk = lex.getNextToken();
            } else {
                throw std::runtime_error("Expected int constant in array size");
            }
            if (curr_tk != Token::CloseBracket) {
                throw std::runtime_error("Expected ']' after array size");
            }
            curr_tk = lex.getNextToken();
            outType += "[]";
        }
        return nullptr; 
    } else if (curr_tk == Token::KwVoid) {
        outType = "void";
        curr_tk = lex.getNextToken();
        return nullptr;
    } else {
        throw std::runtime_error("Expected 'int' or 'void' in method_type");
    }
}

// variable_decl -> type IDENTIFIER
// [ ( COMMA IDENTIFIER )* SEMICOLON
// | OPEN_PAR opt_param_decl_list CLOSE_PAR OPEN_CURLY ( variable_decl )* ( stmt )* CLOSE_CURLY ]
// This looks similar to decla logic, but starts with 'type' instead of 'method_type'.
// difference: variable_decl can also define a function inside (?), per grammar?
AstNode* Parser::variable_decl() {
    std::string varType;
    type(varType);

    if (curr_tk != Token::Ident) {
        throw std::runtime_error("Expected identifier after type in variable_decl");
    }
    std::string name = lex.getText();
    curr_tk = lex.getNextToken();

    if (curr_tk == Token::Comma || curr_tk == Token::SemiColon) {
        // multiple variables
        std::vector<AstNode*> vars;
        vars.push_back(new VarDeclNode(varType, name));
        while (curr_tk == Token::Comma) {
            curr_tk = lex.getNextToken();
            if (curr_tk != Token::Ident) {
                throw std::runtime_error("Expected ident after ',' in var decl");
            }
            std::string vname = lex.getText();
            curr_tk = lex.getNextToken();
            vars.push_back(new VarDeclNode(varType, vname));
        }
        if (curr_tk != Token::SemiColon) throw std::runtime_error("Expected ';' in var decl");
        curr_tk = lex.getNextToken();
        return new BlockNode(vars);

    } else if (curr_tk == Token::OpenPar) {
        // Looks like a function inside variable_decl as per grammar
        curr_tk = lex.getNextToken();
        std::vector<AstNode*> params;
        if (curr_tk != Token::ClosePar) {
            opt_param_decl_list(params);
        }
        if (curr_tk != Token::ClosePar) throw std::runtime_error("Expected ')' in variable_decl function");
        curr_tk = lex.getNextToken();

        if (curr_tk != Token::OpenCurly) throw std::runtime_error("Expected '{' in variable_decl function");
        curr_tk = lex.getNextToken();

        std::vector<AstNode*> bodyStmts;
        while (curr_tk == Token::KwInt) {
            bodyStmts.push_back(variable_decl());
        }
        while (curr_tk != Token::CloseCurly && curr_tk != Token::Eof) {
            bodyStmts.push_back(stmt());
        }

        if (curr_tk != Token::CloseCurly) throw std::runtime_error("Expected '}' at end of var_decl function block");
        curr_tk = lex.getNextToken();

        AstNode* bodyBlock = new BlockNode(bodyStmts);
        // This acts like a method inside var decl: return a MethodDeclNode or similar
        // The grammar isn't super-clear on what node to create. Let's assume a method decl node:
        return new MethodDeclNode(varType, name, params, bodyBlock);

    } else {
        throw std::runtime_error("Expected ',' ';' or '(' after type and ident in variable_decl");
    }
}

// type -> KW_INT [ OPEN_BRACKET CONSTANT CLOSE_BRACKET ]
AstNode* Parser::type(std::string &outType) {
    if (curr_tk == Token::KwInt) {
        outType = "int";
        curr_tk = lex.getNextToken();
        while (curr_tk == Token::OpenBracket) {
            curr_tk = lex.getNextToken();
            if (curr_tk == Token::IntConst || curr_tk == Token::Hexa || curr_tk == Token::Bin) {
                curr_tk = lex.getNextToken();
            } else {
                throw std::runtime_error("Expected int constant in array size for type");
            }
            if (curr_tk != Token::CloseBracket) throw std::runtime_error("Expected ']' after array size");
            curr_tk = lex.getNextToken();
            outType += "[]";
        }
        return nullptr;
    } else {
        throw std::runtime_error("Expected 'int' in type");
    }
}

// opt_param_decl_list -> [ param_decl ( COMMA param_decl ) ]
AstNode* Parser::opt_param_decl_list(std::vector<AstNode*>& params) {
    param_decl(params);
    while (curr_tk == Token::Comma) {
        curr_tk = lex.getNextToken();
        param_decl(params);
    }
    return nullptr;
}

// param_decl -> [ KW_REF ] type IDENTIFIER
AstNode* Parser::param_decl(std::vector<AstNode*>& params) {
    bool isRef = false;
    if (curr_tk == Token::KwRef) {
        isRef = true;
        curr_tk = lex.getNextToken();
    }
    std::string pType;
    type(pType);

    if (curr_tk != Token::Ident) throw std::runtime_error("Expected identifier in param_decl");
    std::string pName = lex.getText();
    curr_tk = lex.getNextToken();

    params.push_back(new ParamDeclNode(pType, pName, isRef));
    return nullptr;
}

// stmt -> assign_stmt | if_stmt | while_stmt | call_stmt | return_stmt | print_stmt | read_stmt
AstNode* Parser::stmt() {
    if (curr_tk == Token::KwRead) return read_stmt();
    if (curr_tk == Token::KwPrint) return print_stmt();
    if (curr_tk == Token::KwReturn) return return_stmt();
    if (curr_tk == Token::KwWhile) return while_stmt();
    if (curr_tk == Token::KwIf) return if_stmt();
    if (curr_tk == Token::Ident) {
        std::string varName = lex.getText();
        curr_tk = lex.getNextToken();
        if (curr_tk == Token::OpenPar) return call_stmt(varName);
        if (curr_tk == Token::OpenBracket || curr_tk == Token::EQUAL) return assign_stmt(varName);
        throw std::runtime_error("Expected statement after identifier");
    }
    throw std::runtime_error("Expected valid statement");
}

// assign_stmt -> IDENTIFIER [ OPEN_BRACKET expression CLOSE_BRACKET ] OP_ASSIGN expression SEMICOLON
AstNode* Parser::assign_stmt(const std::string& varName) {
    AstNode* target;
    if (curr_tk == Token::OpenBracket) {
        curr_tk = lex.getNextToken();
        AstNode* idx = expression();
        if (curr_tk != Token::CloseBracket) throw std::runtime_error("Expected ']' in assign_stmt");
        curr_tk = lex.getNextToken();
        target = new ArrayVariableNode(varName, idx);
    } else {
        target = new IdentifierNode(varName);
    }

    if (curr_tk != Token::EQUAL) throw std::runtime_error("Expected '=' in assign_stmt");
    curr_tk = lex.getNextToken();

    AstNode* val = expression();
    if (curr_tk != Token::SemiColon) throw std::runtime_error("Expected ';' after assign_stmt");
    curr_tk = lex.getNextToken();
    return new AssignStmtNode(target, val);
}

// return_stmt -> KW_RETURN expression SEMICOLON
AstNode* Parser::return_stmt() {
    curr_tk = lex.getNextToken(); // consume return
    AstNode* expr = expression();
    if (curr_tk != Token::SemiColon) throw std::runtime_error("Expected ';' after return");
    curr_tk = lex.getNextToken();
    return new ReturnStmtNode(expr);
}

// if_stmt -> KW_IF OPEN_PAR expression CLOSE_PAR block [ KW_ELSE block ]
AstNode* Parser::if_stmt() {
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::OpenPar) throw std::runtime_error("Expected '(' after if");
    curr_tk = lex.getNextToken();
    AstNode* cond = expression();
    if (curr_tk != Token::ClosePar) throw std::runtime_error("Expected ')' after if condition");
    curr_tk = lex.getNextToken();
    AstNode* thenBl = block();
    AstNode* elseBl = nullptr;
    if (curr_tk == Token::KwElse) {
        curr_tk = lex.getNextToken();
        elseBl = block();
    }
    return new IfStmtNode(cond, thenBl, elseBl);
}

// block -> OPEN_CURLY ( stmt )* CLOSE_CURLY
AstNode* Parser::block() {
    if (curr_tk != Token::OpenCurly) throw std::runtime_error("Expected '{' at start of block");
    curr_tk = lex.getNextToken();
    std::vector<AstNode*> stmts;
    while (curr_tk != Token::CloseCurly && curr_tk != Token::Eof) {
        stmts.push_back(stmt());
    }
    if (curr_tk != Token::CloseCurly) throw std::runtime_error("Expected '}' at end of block");
    curr_tk = lex.getNextToken();
    return new BlockNode(stmts);
}

// while_stmt -> KW_WHILE OPEN_PAR expression CLOSE_PAR block
AstNode* Parser::while_stmt() {
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::OpenPar) throw std::runtime_error("Expected '(' after while");
    curr_tk = lex.getNextToken();
    AstNode* cond = expression();
    if (curr_tk != Token::ClosePar) throw std::runtime_error("Expected ')' after while condition");
    curr_tk = lex.getNextToken();
    AstNode* body = block();
    return new WhileStmtNode(cond, body);
}

// call_stmt -> IDENTIFIER OPEN_PAR [ expression ( COMMA expression )* ] CLOSE_PAR SEMICOLON
AstNode* Parser::call_stmt(const std::string& funcName) {
    curr_tk = lex.getNextToken(); // consume '('
    std::vector<AstNode*> args;
    if (curr_tk != Token::ClosePar) {
        args.push_back(expression());
        while (curr_tk == Token::Comma) {
            curr_tk = lex.getNextToken();
            args.push_back(expression());
        }
    }
    if (curr_tk != Token::ClosePar) throw std::runtime_error("Expected ')' in call_stmt");
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::SemiColon) throw std::runtime_error("Expected ';' after call_stmt");
    curr_tk = lex.getNextToken();
    return new CallStmtNode(funcName, args);
}

// print_stmt -> KW_PRINT OPEN_PAR (expression | STRING_LITERAL) CLOSE_PAR SEMICOLON
AstNode* Parser::print_stmt() {
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::OpenPar) throw std::runtime_error("Expected '(' after print");
    curr_tk = lex.getNextToken();
    AstNode* expr = nullptr;
    if (curr_tk == Token::Comillas) {
        curr_tk = lex.getNextToken();
        std::string text;
        while (curr_tk != Token::Comillas && curr_tk != Token::Eof) {
            text += lex.getText();
            curr_tk = lex.getNextToken();
        }
        if (curr_tk != Token::Comillas) throw std::runtime_error("Expected '\"' closing string in print");
        curr_tk = lex.getNextToken();
        expr = new StringLiteralNode(text);
    } else {
        expr = expression();
    }
    if (curr_tk != Token::ClosePar) throw std::runtime_error("Expected ')' after print expr");
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::SemiColon) throw std::runtime_error("Expected ';' after print");
    curr_tk = lex.getNextToken();
    return new PrintStmtNode(expr);
}

// read_stmt -> KW_READ OPEN_PAR IDENTIFIER CLOSE_PAR SEMICOLON
AstNode* Parser::read_stmt() {
    curr_tk = lex.getNextToken(); // after read
    if (curr_tk != Token::OpenPar) throw std::runtime_error("Expected '(' after read");
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::Ident) throw std::runtime_error("Expected ident in read");
    std::string varName = lex.getText();
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::ClosePar) throw std::runtime_error("Expected ')' in read");
    curr_tk = lex.getNextToken();
    if (curr_tk != Token::SemiColon) throw std::runtime_error("Expected ';' after read");
    curr_tk = lex.getNextToken();
    return new ReadStmtNode(varName);
}

// expression -> boolean_expression
// We'll let expression() call boolean_expression() directly.
AstNode* Parser::expression() {
    return boolean_expression();
}

// boolean_expression -> boolean_term (OP_BOOL_OR boolean_term)*
// OP_BOOL_OR = Token::OR
AstNode* Parser::boolean_expression() {
    AstNode* left = boolean_term();
    while (curr_tk == Token::OR) {
        curr_tk = lex.getNextToken();
        AstNode* right = boolean_term();
        left = new BinaryExprNode(left, right, NodeKind::OrOp);
    }
    return left;
}

// boolean_term -> boolean_factor (OP_BOOL_AND boolean_factor)*
// OP_BOOL_AND = Token::AND
AstNode* Parser::boolean_term() {
    AstNode* left = boolean_factor();
    while (curr_tk == Token::AND) {
        curr_tk = lex.getNextToken();
        AstNode* right = boolean_factor();
        left = new BinaryExprNode(left, right, NodeKind::AndOp);
    }
    return left;
}

// boolean_factor -> relational_expression | OP_BOOL_NOT boolean_factor
// OP_BOOL_NOT = Token::NOT
AstNode* Parser::boolean_factor() {
    if (curr_tk == Token::NOT) {
        curr_tk = lex.getNextToken();
        AstNode* operand = boolean_factor();
        return new UnaryExprNode(operand, NodeKind::NotOp);
    }
    return relational_expression();
}

// relational_expression -> arithmetic_expression OP_REL relational_expression | arithmetic_expression
// OP_REL includes: doubleEqual, NOTEQUAL, LESSTHAN, GREATER, LESS_EQUAL, GREATER_EQUAL
AstNode* Parser::relational_expression() {
    AstNode* left = arithmetic_expression();
    while (curr_tk == Token::doubleEqual || curr_tk == Token::NOTEQUAL || curr_tk == Token::LESSTHAN ||
           curr_tk == Token::GREATER || curr_tk == Token::LESS_EQUAL || curr_tk == Token::GREATER_EQUAL) {
        Token op = curr_tk;
        curr_tk = lex.getNextToken();
        AstNode* right = arithmetic_expression();
        NodeKind kind;
        if (op == Token::LESS_EQUAL) kind = NodeKind::LessEqual;
        else if (op == Token::GREATER_EQUAL) kind = NodeKind::GreaterEqual;
        else if (op == Token::LESSTHAN) kind = NodeKind::LessThan;
        else if (op == Token::GREATER) kind = NodeKind::GreaterThan;
        else if (op == Token::NOTEQUAL) kind = NodeKind::NotEqual;
        else kind = NodeKind::Equal;
        left = new BinaryExprNode(left, right, kind);
    }
    return left;
}

// arithmetic_expression -> term ((OP_ADD | OP_SUB) term)*
AstNode* Parser::arithmetic_expression() {
    AstNode* left = term();
    while (curr_tk == Token::OpAdd || curr_tk == Token::OpSub) {
        Token op = curr_tk;
        curr_tk = lex.getNextToken();
        AstNode* right = term();
        NodeKind kind = (op == Token::OpAdd) ? NodeKind::AddOp : NodeKind::SubOp;
        left = new BinaryExprNode(left, right, kind);
    }
    return left;
}

// term -> factor ((OP_MUL | OP_DIV | OP_MOD) factor)*
AstNode* Parser::term() {
    AstNode* left = factor();
    while (curr_tk == Token::OpMul || curr_tk == Token::OpDiv || curr_tk == Token::OpMod) {
        Token op = curr_tk;
        curr_tk = lex.getNextToken();
        AstNode* right = factor();
        NodeKind kind;
        if (op == Token::OpMul) kind = NodeKind::MulOp;
        else if (op == Token::OpDiv) kind = NodeKind::DivOp;
        else kind = NodeKind::ModOp;
        left = new BinaryExprNode(left, right, kind);
    }
    return left;
}

// factor -> [OP_ADD | OP_SUB] primary
AstNode* Parser::factor() {
    if (curr_tk == Token::OpAdd || curr_tk == Token::OpSub) {
        Token op = curr_tk;
        curr_tk = lex.getNextToken();
        AstNode* val = primary();
        NodeKind kind = (op == Token::OpAdd) ? NodeKind::UnaryPlus : NodeKind::UnaryMinus;
        return new UnaryExprNode(val, kind);
    }
    return primary();
}

// primary -> INT_CONST | IDENTIFIER | IDENTIFIER OPEN_BRACKET expression CLOSE_BRACKET 
//          | IDENTIFIER OPEN_PAR [expression (COMMA expression)*] CLOSE_PAR | OPEN_PAR expression CLOSE_PAR
AstNode* Parser::primary() {
    if (curr_tk == Token::IntConst || curr_tk == Token::Hexa || curr_tk == Token::Bin) {
        int value = std::stoi(lex.getText());
        curr_tk = lex.getNextToken();
        return new IntConstNode(value);
    } else if (curr_tk == Token::Ident) {
        std::string name = lex.getText();
        curr_tk = lex.getNextToken();
        if (curr_tk == Token::OpenBracket) {
            curr_tk = lex.getNextToken();
            AstNode* idx = expression();
            if (curr_tk != Token::CloseBracket) throw std::runtime_error("Missing ']' in primary");
            curr_tk = lex.getNextToken();
            return new ArrayVariableNode(name, idx);
        } else if (curr_tk == Token::OpenPar) {
            // function call in primary
            curr_tk = lex.getNextToken();
            std::vector<AstNode*> args;
            if (curr_tk != Token::ClosePar) {
                args.push_back(expression());
                while (curr_tk == Token::Comma) {
                    curr_tk = lex.getNextToken();
                    args.push_back(expression());
                }
            }
            if (curr_tk != Token::ClosePar) throw std::runtime_error("Missing ')' in function call");
            curr_tk = lex.getNextToken();
            // function calls inside expressions return a node representing a function call
            return new FunctionCallNode(name, args);
        }
        return new IdentifierNode(name);
    } else if (curr_tk == Token::OpenPar) {
        curr_tk = lex.getNextToken();
        AstNode* expr = expression();
        if (curr_tk != Token::ClosePar) throw std::runtime_error("Missing ')' in primary");
        curr_tk = lex.getNextToken();
        return expr;
    } else {
        throw std::runtime_error("Expected int const, identifier or '(' in primary");
    }
}
