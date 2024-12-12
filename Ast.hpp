#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

enum class NodeKind {
    Program,
    VarDecl,
    MethodDecl,
    ParamDecl,
    Block,

    // Statements
    IfStmt,
    WhileStmt,
    PrintStmt,
    ReadStmt,
    ReturnStmt,
    AssignStmt,
    CallStmt,

    // Expression-level function call
    FunctionCallNode,

    // Expressions
    BoolExpr,
    BoolTerm,
    BoolFactor,
    RelationExpr,
    ArithExpr,
    TermNode,
    FactorNode,
    PrimaryNode,

    // Operators for boolean and relational
    OrOp,
    AndOp,
    NotOp,
    LessEqual,
    GreaterEqual,
    LessThan,
    GreaterThan,
    NotEqual,
    Equal,

    // Arithmetic operators
    AddOp,
    SubOp,
    MulOp,
    DivOp,
    ModOp,

    // Unary operators
    UnaryPlus,
    UnaryMinus,

    // Literals and identifiers
    IntConstNode,
    StringLiteralNode,
    IdentifierNode,

    // Array variable
    ArrayVariableNode
};

class AstNode {
public:
    virtual ~AstNode() = default;
    virtual NodeKind getKind() const = 0;  // Ensure all nodes can identify their type
    virtual std::string toString() const = 0;
};
// ------------------ Program Node ------------------
class ProgramNode : public AstNode {
    std::string className;
    std::vector<AstNode*> declarations;
public:
    ProgramNode(const std::string& name, const std::vector<AstNode*>& decls);
    ~ProgramNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::Program; } // Corrected
     const std::vector<AstNode*>& getDeclarations() const { return declarations; }
};

// Variable Declaration Node
class VarDeclNode : public AstNode {
    std::string type, name;
public:
    VarDeclNode(const std::string& t, const std::string& n);
    ~VarDeclNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::VarDecl; } // Corrected
    std::string getName() const { return name; }
    std::string getType() const { return type; }
};


// Method Declaration Node
class MethodDeclNode : public AstNode {
    std::string returnType, methodName;
    std::vector<AstNode*> params;
    AstNode* body;
public:
    MethodDeclNode(const std::string& retType, const std::string& name, const std::vector<AstNode*>& p, AstNode* b);
    ~MethodDeclNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::MethodDecl; }
    AstNode* getBody() const { return body; }

};

// ParamDeclNode
class ParamDeclNode : public AstNode {
    std::string type, name;
    bool isRef;
public:
    ParamDeclNode(const std::string& t, const std::string& n, bool ref);
    ~ParamDeclNode() override;
    std::string toString() const override;
NodeKind getKind() const override { return NodeKind::ParamDecl; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }

};

// BlockNode
class BlockNode : public AstNode {
    std::vector<AstNode*> statements;
public:
    BlockNode(const std::vector<AstNode*>& stmts);
    ~BlockNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::Block; }
std::vector<AstNode*> getStatements() const { return statements; }

};
//********************* */
// IfStmtNode
// IfStmtNode
class IfStmtNode : public AstNode {
    AstNode* condition;
    AstNode* thenBlock;
    AstNode* elseBlock;
public:
    IfStmtNode(AstNode* cond, AstNode* thenB, AstNode* elseB = nullptr);
    ~IfStmtNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::IfStmt; }
    AstNode* getCondition() const { return condition; }
    AstNode* getThenBlock() const { return thenBlock; }
    AstNode* getElseBlock() const { return elseBlock; }
};

// WhileStmtNode
class WhileStmtNode : public AstNode {
    AstNode* condition;
    AstNode* body;
public:
    WhileStmtNode(AstNode* cond, AstNode* b);
    ~WhileStmtNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::WhileStmt; }
    AstNode* getCondition() const { return condition; }
    AstNode* getBody() const { return body; }
};

// PrintStmtNode
class PrintStmtNode : public AstNode {
    AstNode* expr;
public:
    PrintStmtNode(AstNode* e);
    ~PrintStmtNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::PrintStmt; }
    AstNode* getExpr() const { return expr; }
};

// ReadStmtNode
class ReadStmtNode : public AstNode {
    std::string identName;
public:
    ReadStmtNode(const std::string& name);
    ~ReadStmtNode() override = default;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::ReadStmt; }
    std::string getIdentName() const { return identName; }
};

//********************* */
// ReturnStmtNode
class ReturnStmtNode : public AstNode {
    AstNode* expr;
public:
    ReturnStmtNode(AstNode* e);
    ~ReturnStmtNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::ReturnStmt; }
    AstNode* getExpr() const { return expr; }
};

class AssignStmtNode : public AstNode {
    AstNode* target;
    AstNode* value;
public:
    AssignStmtNode(AstNode* tgt, AstNode* val);
    ~AssignStmtNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::AssignStmt; }
    AstNode* getTarget() const { return target; }
    AstNode* getValue() const { return value; }
};

class CallStmtNode : public AstNode {
    std::string functionName;
    std::vector<AstNode*> args;
public:
    CallStmtNode(const std::string& func, const std::vector<AstNode*>& arguments);
    ~CallStmtNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::CallStmt; }
    std::string getFunctionName() const { return functionName; }
    std::vector<AstNode*> getArgs() const { return args; }
};

class FunctionCallNode : public AstNode {
    std::string functionName;
    std::vector<AstNode*> args;
public:
    FunctionCallNode(const std::string& func, const std::vector<AstNode*>& arguments);
    ~FunctionCallNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::FunctionCallNode; }
     const std::vector<AstNode*>& getArgs() const { return args; }
    const std::string& getFunctionName() const { return functionName; }
};

class StringLiteralNode : public AstNode {
    std::string value;
public:
    StringLiteralNode(const std::string& val);
    ~StringLiteralNode() override = default;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::StringLiteralNode; }
    std::string getValue() const { return value; }
};

class ArrayVariableNode : public AstNode {
    std::string name;
    AstNode* index;
public:
    ArrayVariableNode(const std::string& n, AstNode* idx);
    ~ArrayVariableNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::ArrayVariableNode; }
    std::string getName() const { return name; }
    AstNode* getIndex() const { return index; }
};

//************************ */
// IdentifierNode
class IdentifierNode : public AstNode {
    std::string name;
public:
    IdentifierNode(const std::string& n);
    ~IdentifierNode() override = default;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::IdentifierNode; }
    std::string getName() const { return name; }
};

// IntConstNode
class IntConstNode : public AstNode {
    int value;
public:
    IntConstNode(int v);
    ~IntConstNode() override = default;
    std::string toString() const override;
    NodeKind getKind() const override { return NodeKind::IntConstNode; }
    int getValue() const { return value; }
};
// BinaryExprNode
class BinaryExprNode : public AstNode {
    AstNode* left;
    AstNode* right;
    NodeKind op;
public:
    BinaryExprNode(AstNode* l, AstNode* r, NodeKind o);
    ~BinaryExprNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return op; }
    AstNode* getLeft() const { return left; }
    AstNode* getRight() const { return right; }
};
// UnaryExprNode
class UnaryExprNode : public AstNode {
    AstNode* expr;
    NodeKind op;
public:
    UnaryExprNode(AstNode* e, NodeKind o);
    ~UnaryExprNode() override;
    std::string toString() const override;
    NodeKind getKind() const override { return op; }
    AstNode* getExpr() const { return expr; }
};


