#include "irGenerator.h"
#include "ast.h"

std::unique_ptr<Value> ast::NumberExprAST::Codegen() {
    return ConstantFP::get(getGlobalContext(), APFloat(Val));
}

std::unique_ptr<Value> ast::VariableExprAST::Codegen() {
    auto V = NamedValues[Name];
    return V ? std::move(V) : ErrorV("Unknown variable name");
}


