#include "common.h"

namespace ast {
// 基底クラス
class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual std::unique_ptr<Value> Codegen() = nullptr;
};

// 数字
class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double Val) : Val(Val) {}
    virtual std::unique_ptr<Value> Codegen();
};

// 変数
class VariableExprAST : public ExprAST {
    std::string Name;
public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
    virtual std::unique_ptr<Value> Codegen();
};

// 二項演算子
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    virtual std::unique_ptr<Value> Codegen();
};

// 関数呼び出し
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(std::move(Args)) {}
    virtual std::unique_ptr<Value> Codegen();
};

// 関数のプロトタイプ
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(const std::string &Name, const std::vector<std::string> &Args)
        : Name(Name), Args(std::move(Args)) {}
};

// 関数定義それ自身
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
};
std::unique_ptr<ExprAST> LogError(const char *Str) { fprintf(stderr, "Error: %s\n", Str); return nullptr; }
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) { LogError(Str); return nullptr; }
std::unique_ptr<FunctionAST> LogErrorF(const char *Str) { LogError(Str); return nullptr; }
std::unique_ptr<Value> LogErrorV(const char *Str) { LogError(Str); return nullptr; }
} // namespace ast


