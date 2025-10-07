#pragma once
#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();

private:
    const std::vector<Token>& tokens;
    size_t position = 0;

    const Token& peek() const;
    Token proximo_token();
    Token previous() const;
    bool match(TokenType type);
    bool match(TokenType type1, TokenType type2);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    void verificaProxToken(TokenType expected_type);

    std::unique_ptr<Program> program();
    std::unique_ptr<Statement> declaration();
    std::unique_ptr<Statement> varDeclaration();
    std::unique_ptr<Statement> functionDeclaration();
    std::unique_ptr<Statement> mainFunction();
    std::unique_ptr<BlockStatement> blockStatement();
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> ifStatement();
    std::unique_ptr<Statement> whileStatement();
    std::unique_ptr<Statement> returnStatement();
    std::unique_ptr<Exp> expression();
    std::unique_ptr<Exp> assignmentExpression();
    std::unique_ptr<Exp> orExpression();
    std::unique_ptr<Exp> andExpression();
    std::unique_ptr<Exp> equality();
    std::unique_ptr<Exp> comparison();
    std::unique_ptr<Exp> term();
    std::unique_ptr<Exp> factor();
    std::unique_ptr<Exp> unary();
    std::unique_ptr<Exp> primary();
};
