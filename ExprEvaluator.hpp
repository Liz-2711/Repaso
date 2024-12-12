#pragma once

#include "Ast.hpp"
#include <unordered_map>
#include <functional>  // Include this for std::function
#include <string>
#include <vector>

class ExprEvaluator {
private:
    std::unordered_map<std::string, int> variables;  // Variable storage
    std::unordered_map<std::string, std::function<int(std::vector<int>)>> functions;  // Function storage

public:
    // Evaluate the given AST node and return its computed value
    int evaluate(const AstNode* node);

    // Execute a statement-level AST node
    void execute(const AstNode* node);

    // Evaluate a function call from the AST
    int evaluateFunctionCall(const FunctionCallNode* funcCallNode);

    // Evaluate array access
    int getArrayValue(const std::string& arrayName, int index);
};
