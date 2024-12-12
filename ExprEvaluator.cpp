#include "ExprEvaluator.hpp"
#include <stdexcept>
#include <iostream>


int ExprEvaluator::evaluate(const AstNode* node) {  // Corrected const qualifier
    if (!node) {
        throw std::runtime_error("Null AST node encountered during evaluation");
    }
   std::cout << "[DEBUG] Evaluating node: " << node->toString() << std::endl;
    
    switch (node->getKind()) {

       case NodeKind::Program: {
    const auto* programNode = static_cast<const ProgramNode*>(node);
    int result = 0;
    for (auto& decl : programNode->getDeclarations()) {  // Use the getter method
        result += evaluate(decl);  // Accumulate results or handle each declaration
    }
    return result;
}

        case NodeKind::IntConstNode: {
            const auto* intNode = static_cast<const IntConstNode*>(node);
                std::cout << "[DEBUG] IntConst: " << intNode->getValue() << std::endl;
            return intNode->getValue();
        }

        case NodeKind::StringLiteralNode: {
            const auto* strNode = static_cast<const StringLiteralNode*>(node);
            throw std::runtime_error("Cannot evaluate string literals directly");
        }

        case NodeKind::IdentifierNode: {
            const auto* idNode = static_cast<const IdentifierNode*>(node);
            auto it = variables.find(idNode->getName());
            if (it == variables.end()) {
                throw std::runtime_error("Undefined variable: " + idNode->getName());
            }
             std::cout << "[DEBUG] Identifier: " << idNode->getName() << " = " << it->second << std::endl;
            return it->second;
        }

        case NodeKind::AddOp: {
            const auto* binNode = static_cast<const BinaryExprNode*>(node);
             int leftVal = evaluate(binNode->getLeft());
            int rightVal = evaluate(binNode->getRight());
               std::cout << "[DEBUG] AddOp: " << leftVal << " + " << rightVal << " = " << (leftVal + rightVal) << std::endl;
            return leftVal + rightVal;
        }

        case NodeKind::SubOp: {
            const auto* binNode = static_cast<const BinaryExprNode*>(node);
             return evaluate(binNode->getLeft()) - evaluate(binNode->getRight());
        }

        case NodeKind::MulOp: {
            const auto* binNode = static_cast<const BinaryExprNode*>(node);
            return evaluate(binNode->getLeft()) * evaluate(binNode->getRight());
        }

        case NodeKind::DivOp: {
            const auto* binNode = static_cast<const BinaryExprNode*>(node);
            int rightValue = evaluate(binNode->getRight());
            if (rightValue == 0) {
                throw std::runtime_error("Division by zero encountered during evaluation");
            }
            return evaluate(binNode->getLeft()) / rightValue;
        }

        case NodeKind::ModOp: {
            const auto* binNode = static_cast<const BinaryExprNode*>(node);
            int rightValue = evaluate(binNode->getRight());
            if (rightValue == 0) {
                throw std::runtime_error("Modulo by zero encountered during evaluation");
            }
            return evaluate(binNode->getLeft()) % rightValue;
        }

        case NodeKind::UnaryPlus: {
            const auto* unaryNode = static_cast<const UnaryExprNode*>(node);
            return evaluate(unaryNode->getExpr());
        }

        case NodeKind::UnaryMinus: {
            const auto* unaryNode = static_cast<const UnaryExprNode*>(node);
            return -evaluate(unaryNode->getExpr());
        }

       case NodeKind::IfStmt: {
    const auto* ifNode = static_cast<const IfStmtNode*>(node);  // Ensure correct type
    if (evaluate(ifNode->getCondition())) {
        execute(ifNode->getThenBlock());
    }
    if (ifNode->getElseBlock()) {
        execute(ifNode->getElseBlock());
    }
    break;
}
case NodeKind::Block: {
    const auto* blockNode = static_cast<const BlockNode*>(node);
    int lastValue = 0;  // Default value if block is empty
    for (const auto& stmt : blockNode->getStatements()) {
        lastValue = evaluate(stmt);  // Evaluate each statement and possibly update the last value
    }
    return lastValue;  // Return the value of the last statement
}


        case NodeKind::WhileStmt: {
    const auto* whNode = static_cast<const WhileStmtNode*>(node);  // Correct the class name
    while (evaluate(whNode->getCondition())) {  // Correct method name
     std::cout << "[DEBUG] While loop condition true, executing body..." << std::endl;
        execute(whNode->getBody());  // Correct method name and capitalization
         std::cout << "[DEBUG] While loop finished." << std::endl;
    }
    return 0;
}

case NodeKind::AssignStmt: {
    const auto* assignNode = static_cast<const AssignStmtNode*>(node);
    int value = evaluate(assignNode->getValue()); // Use getter instead of direct access
    const auto* idNode = static_cast<const IdentifierNode*>(assignNode->getTarget()); // Use getter
    variables[idNode->getName()] = value; // Use getName() getter
   std::cout << "[DEBUG] Assign: " << idNode->getName() << " = " << value << std::endl;
    return value;
}

case NodeKind::CallStmt: {
    const auto* callNode = static_cast<const CallStmtNode*>(node);
    execute(callNode);
    return 0;
}

case NodeKind::ReturnStmt: {
    const auto* retNode = static_cast<const ReturnStmtNode*>(node);
    return evaluate(retNode->getExpr()); // Use getter
}

case NodeKind::PrintStmt: {
    const auto* printNode = static_cast<const PrintStmtNode*>(node);
    std::cout << evaluate(printNode->getExpr()) << std::endl; // Use getter
    return 0;
}

case NodeKind::ReadStmt: {
    const auto* readNode = static_cast<const ReadStmtNode*>(node);
    int value;
    std::cin >> value;
    variables[readNode->getIdentName()] = value; // Use getIdentName() getter
    return 0;
}
// Handling for VarDecl - usually, these don't produce a runtime value but may initialize variables.
case NodeKind::VarDecl: {
    const auto* varDeclNode = static_cast<const VarDeclNode*>(node);
    // Assuming a simple system where variables are initialized to 0.
    variables[varDeclNode->getName()] = 0;
    return 0;
}

// Handling for MethodDecl - typically doesn't produce a runtime value.
case NodeKind::MethodDecl: {
    // You might register the method in some kind of function table here if necessary.
    return 0;
}

// Handling for ParamDecl - typically doesn't produce a runtime value.
case NodeKind::ParamDecl: {
    // Parameters are usually handled during function setup, not during general evaluation.
    return 0;
}


// Handling for FunctionCallNode - evaluate a function call.
case NodeKind::FunctionCallNode: {
    const auto* funcCallNode = static_cast<const FunctionCallNode*>(node);
    return evaluateFunctionCall(funcCallNode); // Call the function to handle function call evaluation
}
        case NodeKind::ArrayVariableNode: {
            const auto* arrayVarNode = static_cast<const ArrayVariableNode*>(node);
            return getArrayValue(arrayVarNode->getName(), evaluate(arrayVarNode->getIndex()));
        }

 default:
            std::cerr << "Unsupported AST node type during evaluation: " 
                      << static_cast<int>(node->getKind()) << std::endl;
            throw std::runtime_error("Unsupported AST node type during evaluation");

}

}

void ExprEvaluator::execute(const AstNode* node) {
    if (node) {
        evaluate(node);  // Evaluate the node
    }
}

int ExprEvaluator::evaluateFunctionCall(const FunctionCallNode* funcCallNode) {
    std::vector<int> argValues;
    for (const AstNode* arg : funcCallNode->getArgs()) {
        argValues.push_back(evaluate(arg));  // Evaluate each argument
    }

    auto funcIt = functions.find(funcCallNode->getFunctionName());
    if (funcIt != functions.end()) {
        return funcIt->second(argValues);  // Call the function with arguments
    }

    throw std::runtime_error("Function not defined: " + funcCallNode->getFunctionName());
}
int ExprEvaluator::getArrayValue(const std::string& arrayName, int index) {
    std::string indexedName = arrayName + "[" + std::to_string(index) + "]";

    auto it = variables.find(indexedName);
    if (it != variables.end()) {
        return it->second;
    }

    throw std::runtime_error("Array index out of bounds or array not defined: " + indexedName);
}
