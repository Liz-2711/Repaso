#include "Ast.hpp"

// ProgramNode
ProgramNode::ProgramNode(const std::string& name, const std::vector<AstNode*>& decls)
    : className(name), declarations(decls) {}

ProgramNode::~ProgramNode() {
    for (auto d : declarations) delete d;
}
std::string ProgramNode::toString() const {
    std::ostringstream oss;
    oss << "Program(" << className << ", [";
    for (size_t i = 0; i < declarations.size(); ++i) {
        oss << declarations[i]->toString();
        if (i < declarations.size() - 1) oss << ", ";
    }
    oss << "])";
    return oss.str();
}

// VarDeclNode
VarDeclNode::VarDeclNode(const std::string& t, const std::string& n)
    : type(t), name(n) {}
VarDeclNode::~VarDeclNode() {}
std::string VarDeclNode::toString() const {
    return "VarDecl(" + type + ", " + name + ")";
}

// MethodDeclNode
MethodDeclNode::MethodDeclNode(const std::string& retType, const std::string& name,
                               const std::vector<AstNode*>& p, AstNode* b)
    : returnType(retType), methodName(name), params(p), body(b) {}
MethodDeclNode::~MethodDeclNode() {
    for (auto param : params) delete param;
    delete body;
}
std::string MethodDeclNode::toString() const {
    std::ostringstream oss;
    oss << "MethodDecl(" << returnType << " " << methodName << ", [";
    for (size_t i = 0; i < params.size(); ++i) {
        oss << params[i]->toString();
        if (i < params.size()-1) oss << ", ";
    }
    oss << "], " << body->toString() << ")";
    return oss.str();
}

// ParamDeclNode
ParamDeclNode::ParamDeclNode(const std::string& t, const std::string& n, bool ref)
    : type(t), name(n), isRef(ref) {}
ParamDeclNode::~ParamDeclNode(){}
std::string ParamDeclNode::toString() const {
return std::string("ParamDecl(") + (isRef ? "ref " : "") + type + ", " + name + ")";

}

// BlockNode
BlockNode::BlockNode(const std::vector<AstNode*>& stmts) : statements(stmts) {}
BlockNode::~BlockNode() {
    for (auto s : statements) delete s;
}
std::string BlockNode::toString() const {
    std::ostringstream oss;
    oss << "Block([";
    for (size_t i = 0; i < statements.size(); ++i) {
        oss << statements[i]->toString();
        if (i < statements.size()-1) oss << ", ";
    }
    oss << "])";
    return oss.str();
}

// IfStmtNode
IfStmtNode::IfStmtNode(AstNode* cond, AstNode* thenB, AstNode* elseB)
    : condition(cond), thenBlock(thenB), elseBlock(elseB) {}
IfStmtNode::~IfStmtNode() {
    delete condition;
    delete thenBlock;
    delete elseBlock;
}
std::string IfStmtNode::toString() const {
    std::ostringstream oss;
    oss << "IfStmt(" << condition->toString() << ", " << thenBlock->toString();
    if (elseBlock) oss << ", " << elseBlock->toString();
    oss << ")";
    return oss.str();
}

// WhileStmtNode
WhileStmtNode::WhileStmtNode(AstNode* cond, AstNode* b) : condition(cond), body(b) {}
WhileStmtNode::~WhileStmtNode() {
    delete condition; delete body;
}
std::string WhileStmtNode::toString() const {
    return "WhileStmt(" + condition->toString() + ", " + body->toString() + ")";
}

// PrintStmtNode
PrintStmtNode::PrintStmtNode(AstNode* e) : expr(e) {}
PrintStmtNode::~PrintStmtNode() { delete expr; }
std::string PrintStmtNode::toString() const {
    return "PrintStmt(" + expr->toString() + ")";
}

// ReadStmtNode
ReadStmtNode::ReadStmtNode(const std::string& name) : identName(name) {}
std::string ReadStmtNode::toString() const {
    return "ReadStmt(" + identName + ")";
}

// ReturnStmtNode
ReturnStmtNode::ReturnStmtNode(AstNode* e) : expr(e) {}
ReturnStmtNode::~ReturnStmtNode() { delete expr; }
std::string ReturnStmtNode::toString() const {
    return "ReturnStmt(" + expr->toString() + ")";
}

// AssignStmtNode
AssignStmtNode::AssignStmtNode(AstNode* tgt, AstNode* val) : target(tgt), value(val) {}
AssignStmtNode::~AssignStmtNode() {
    delete target;
    delete value;
}
std::string AssignStmtNode::toString() const {
    return "AssignStmt(" + target->toString() + " = " + value->toString() + ")";
}

// CallStmtNode
CallStmtNode::CallStmtNode(const std::string& func, const std::vector<AstNode*>& arguments)
    : functionName(func), args(arguments) {}
CallStmtNode::~CallStmtNode() {
    for (auto a : args) delete a;
}
std::string CallStmtNode::toString() const {
    std::ostringstream oss;
    oss << "CallStmt(" << functionName << "(";
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << args[i]->toString();
    }
    oss << "))";
    return oss.str();
}

// FunctionCallNode
FunctionCallNode::FunctionCallNode(const std::string& func, const std::vector<AstNode*>& arguments)
    : functionName(func), args(arguments) {}
FunctionCallNode::~FunctionCallNode() {
    for (auto a : args) delete a;
}
std::string FunctionCallNode::toString() const {
    std::ostringstream oss;
    oss << "FunctionCall(" << functionName << "(";
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << args[i]->toString();
    }
    oss << "))";
    return oss.str();
}

// StringLiteralNode
StringLiteralNode::StringLiteralNode(const std::string& val) : value(val) {}
std::string StringLiteralNode::toString() const {
    return "StringLiteral(\"" + value + "\")";
}

// ArrayVariableNode
ArrayVariableNode::ArrayVariableNode(const std::string& n, AstNode* idx)
    : name(n), index(idx) {}
ArrayVariableNode::~ArrayVariableNode() {
    delete index;
}
std::string ArrayVariableNode::toString() const {
    return "ArrayVariable(" + name + "[" + index->toString() + "])";
}

// IdentifierNode
IdentifierNode::IdentifierNode(const std::string& n) : name(n) {}
std::string IdentifierNode::toString() const {
    return "Identifier(" + name + ")";
}

// IntConstNode
IntConstNode::IntConstNode(int v) : value(v) {}
std::string IntConstNode::toString() const {
    return "IntConst(" + std::to_string(value) + ")";
}

// BinaryExprNode
BinaryExprNode::BinaryExprNode(AstNode* l, AstNode* r, NodeKind o)
    : left(l), right(r), op(o) {}
BinaryExprNode::~BinaryExprNode() {
    delete left; delete right;
}
std::string BinaryExprNode::toString() const {
    std::ostringstream oss;
    oss << "BinaryExpr(" << left->toString() << ", " << right->toString()
        << ", " << static_cast<int>(op) << ")";
    return oss.str();
}

// UnaryExprNode
UnaryExprNode::UnaryExprNode(AstNode* e, NodeKind o) : expr(e), op(o) {}
UnaryExprNode::~UnaryExprNode() { delete expr; }
std::string UnaryExprNode::toString() const {
    std::ostringstream oss;
    oss << "UnaryExpr(" << expr->toString() << ", " << static_cast<int>(op) << ")";
    return oss.str();
}
