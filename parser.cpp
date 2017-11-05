#include "ast.h"
#include "lexer.h"
#include "parse.h"

static std::unique_ptr<ast::ExprAST> ParseNumberExpr();
static std::unique_ptr<ast::ExprAST> ParseParenExpr();
static std::unique_ptr<ast::ExprAST> ParseIdentifierExpr();
static std::unique_ptr<ast::ExprAST> ParsePrimary();
static std::unique_ptr<ast::ExprAST> ParseExpression();
static std::unique_ptr<ast::ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ast::ExprAST> LHS);
static std::unique_ptr<ast::PrototypeAST> ParsePrototype();
static std::unique_ptr<ast::FunctionAST> ParseDefinition();
static std::unique_ptr<ast::PrototypeAST> ParseExtern();
static std::unique_ptr<ast::FunctionAST> ParseTopLevelExpr();

// 数字
static std::unique_ptr<ast::ExprAST> ParseNumberExpr() {
    auto Result = llvm::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

// 丸括弧
static std::unique_ptr<ast::ExprAST> ParseParenExpr() {
    getNextToken();
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return ast::LogError("expected ')'");
    getNextToken();
    return V;
}

// 識別子
static std::unique_ptr<ast::ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken();

    // 変数参照
    if (CurTok != '(')
        return llvm::make_unique<ast::VariableExprAST>(IdName);

    // 関数呼び出し
    getNextToken();
    std::vector<std::unique_ptr<ast::ExprAST>> Args;
    if (CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression)
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return ast::LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    getNextToken();

    return llvm::make_unique<ast::CallExprAST>(IdName, std::move(Args));
}

// プライマリ
static std::unique_ptr<ast::ExprAST> ParsePrimary() {
    switch (CurTok) {
    default:
        return ast::LogError("unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    }
}

// 式
static std::unique_ptr<ast::ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

// 二項演算子のペアの解析
static std::unique_ptr<ast::ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ast::ExprAST> LHS) {
    while (true) {
        int TokPrec = GetTokPrecedence();

        if (TokPrec < ExprPrec)
            return LHS;

        int BinOp = CurTok;
        getNextToken();

        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec+1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = llvm::make_unique<ast::BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

// 関数プロトタイプ
static std::unique_ptr<ast::PrototypeAST> ParsePrototype() {
    if (CurTok != tok_identifier)
        return ast::LogErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return ast::LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return ast::LogErrorP("Expected ')' in prototype");

    getNextToken();

    return llvm::make_unique<ast::PrototypeAST>(FnName, std::move(ArgNames));
}

static std::unique_ptr<ast::FunctionAST> ParseDefinition() {
    getNextToken();
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseExpression())
        return llvm::make_unique<ast::FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

static std::unique_ptr<ast::PrototypeAST> ParseExtern() {
    getNextToken();
    return ParsePrototype();
}

static std::unique_ptr<ast::FunctionAST> ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        auto Proto = llvm::make_unique<ast::PrototypeAST>("__anon_expr", std::vector<std::string>());
        return llvm::make_unique<ast::FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

static void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken();
    }
}

static void HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern.\n");
    } else {
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr.\n");
    } else {
        getNextToken();
    }
}

void MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
        case tok_eof:
            return;
        case ';':
            getNextToken();
            break;
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}
