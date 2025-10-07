#include "parser.h"
#include "ast.h"
#include <memory>
#include <stdexcept>
#include <string>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

const Token& Parser::peek() const {
    if (position >= tokens.size()) return tokens.back();
    return tokens[position];
}

Token Parser::proximo_token() {
    if (position >= tokens.size()) return tokens.back();
    return tokens[position++];
}

Token Parser::previous() const {
    if (position == 0) return tokens[0];
    return tokens[position - 1];
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        proximo_token();
        return true;
    }
    return false;
}

bool Parser::match(TokenType type1, TokenType type2) {
    if (check(type1) || check(type2)) {
        proximo_token();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

void Parser::verificaProxToken(TokenType expected_type) {
    Token token = proximo_token();
    if (token.type != expected_type) {
        throw std::runtime_error("Erro de sintaxe: token inesperado. Esperava " + token_type_to_string(expected_type) + " mas encontrou " + token_type_to_string(token.type));
    }
}

std::unique_ptr<Program> Parser::parse() {
    return program();
}

std::unique_ptr<Program> Parser::program() {
    auto prog = std::make_unique<Program>();
    
    while (!isAtEnd() && !check(TokenType::MAIN)) {
        auto decl = declaration();
        prog->addGlobalDeclaration(std::move(decl));
    }
    
    if (check(TokenType::MAIN)) {
        auto main = mainFunction();
        prog->setMainFunction(std::move(main));
    } else {
        throw std::runtime_error("Erro de sintaxe: esperava 'main()' function.");
    }
    
    return prog;
}

std::unique_ptr<Statement> Parser::declaration() {
    if (match(TokenType::LET)) {
        return varDeclaration();
    }
    if (match(TokenType::FUN)) {
        return functionDeclaration();
    }
    return statement();
}

std::unique_ptr<Statement> Parser::varDeclaration() {
    if (!check(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Erro de sintaxe: esperava nome da variavel apos 'let'.");
    }
    
    Token nameToken = proximo_token();
    std::string name = nameToken.lexeme;
    
    std::unique_ptr<Exp> initializer = nullptr;
    if (match(TokenType::ASSIGN)) {
        initializer = expression();
    }
    
    verificaProxToken(TokenType::SEMICOLON);
    return std::make_unique<VarDeclaration>(name, std::move(initializer));
}

std::unique_ptr<Statement> Parser::functionDeclaration() {
    if (!check(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Erro de sintaxe: esperava nome da funcao apos 'fun'.");
    }
    
    Token nameToken = proximo_token();
    std::string name = nameToken.lexeme;
    
    verificaProxToken(TokenType::LPAREN);
    
    std::vector<Parameter> parameters;
    if (!check(TokenType::RPAREN)) {
        do {
            if (!check(TokenType::IDENTIFIER)) {
                throw std::runtime_error("Erro de sintaxe: esperava nome do parametro.");
            }
            Token paramToken = proximo_token();
            parameters.emplace_back(paramToken.lexeme);
        } while (match(TokenType::COMMA));
    }
    
    verificaProxToken(TokenType::RPAREN);
    auto body = blockStatement();
    
    return std::make_unique<FunctionDeclaration>(name, std::move(parameters), std::move(body));
}

std::unique_ptr<Statement> Parser::mainFunction() {
    verificaProxToken(TokenType::MAIN);
    verificaProxToken(TokenType::LPAREN);
    verificaProxToken(TokenType::RPAREN);
    
    auto body = blockStatement();
    return std::make_unique<MainFunction>(std::move(body));
}

std::unique_ptr<BlockStatement> Parser::blockStatement() {
    verificaProxToken(TokenType::LBRACE);
    
    auto block = std::make_unique<BlockStatement>();
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        auto stmt = statement();
        block->addStatement(std::move(stmt));
    }
    
    verificaProxToken(TokenType::RBRACE);
    return block;
}

std::unique_ptr<Statement> Parser::statement() {
    if (match(TokenType::IF)) {
        return ifStatement();
    }
    
    if (match(TokenType::WHILE)) {
        return whileStatement();
    }
    
    if (match(TokenType::RETURN)) {
        return returnStatement();
    }
    
    if (match(TokenType::LET)) {
        return varDeclaration();
    }
    
    auto expr = expression();
    verificaProxToken(TokenType::SEMICOLON);
    return std::make_unique<ExpressionStatement>(std::move(expr));
}

std::unique_ptr<Statement> Parser::ifStatement() {
    verificaProxToken(TokenType::LPAREN);
    auto condition = expression();
    verificaProxToken(TokenType::RPAREN);
    
    auto thenBranch = blockStatement();
    std::unique_ptr<Statement> elseBranch = nullptr;
    
    if (match(TokenType::ELSE)) {
        elseBranch = blockStatement();
    }
    
    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Statement> Parser::whileStatement() {
    verificaProxToken(TokenType::LPAREN);
    auto condition = expression();
    verificaProxToken(TokenType::RPAREN);
    
    auto body = blockStatement();
    
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> Parser::returnStatement() {
    auto expr = expression();
    verificaProxToken(TokenType::SEMICOLON);
    return std::make_unique<ReturnStatement>(std::move(expr));
}

std::unique_ptr<Exp> Parser::expression() {
    return assignmentExpression();
}

std::unique_ptr<Exp> Parser::assignmentExpression() {
    std::unique_ptr<Exp> expr = orExpression();
    
    if (match(TokenType::ASSIGN)) {
        Variable* var = dynamic_cast<Variable*>(expr.get());
        if (!var) {
            throw std::runtime_error("Erro de sintaxe: lado esquerdo da atribuicao deve ser uma variavel.");
        }
        std::string varName = var->name;
        expr.release();
        
        std::unique_ptr<Exp> value = assignmentExpression();
        return std::make_unique<AssignmentExpression>(varName, std::move(value));
    }
    
    return expr;
}

std::unique_ptr<Exp> Parser::orExpression() {
    std::unique_ptr<Exp> expr = andExpression();

    while (match(TokenType::OR)) {
        std::unique_ptr<Exp> right = andExpression();
        expr = std::make_unique<LogicalExpression>(std::move(expr), LogicalOperator::OR, std::move(right));
    }

    return expr;
}

std::unique_ptr<Exp> Parser::andExpression() {
    std::unique_ptr<Exp> expr = equality();

    while (match(TokenType::AND)) {
        std::unique_ptr<Exp> right = equality();
        expr = std::make_unique<LogicalExpression>(std::move(expr), LogicalOperator::AND, std::move(right));
    }

    return expr;
}

std::unique_ptr<Exp> Parser::equality() {
    std::unique_ptr<Exp> expr = comparison();

    while (match(TokenType::EQUAL) || match(TokenType::NOT_EQUAL)) {
        Token operatorToken = previous();
        std::unique_ptr<Exp> right = comparison();
        
        ComparisonOperator op;
        switch (operatorToken.type) {
            case TokenType::EQUAL: op = ComparisonOperator::EQUAL; break;
            case TokenType::NOT_EQUAL: op = ComparisonOperator::NOT_EQUAL; break;
            default: throw std::runtime_error("Operador invalido em equality()");
        }
        
        expr = std::make_unique<ComparisonExpression>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Exp> Parser::comparison() {
    std::unique_ptr<Exp> expr = term();

    while (match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) || 
           match(TokenType::LESS) || match(TokenType::LESS_EQUAL)) {
        Token operatorToken = previous();
        std::unique_ptr<Exp> right = term();
        
        ComparisonOperator op;
        switch (operatorToken.type) {
            case TokenType::GREATER: op = ComparisonOperator::GREATER; break;
            case TokenType::GREATER_EQUAL: op = ComparisonOperator::GREATER_EQUAL; break;
            case TokenType::LESS: op = ComparisonOperator::LESS; break;
            case TokenType::LESS_EQUAL: op = ComparisonOperator::LESS_EQUAL; break;
            default: throw std::runtime_error("Operador invalido em comparison()");
        }
        
        expr = std::make_unique<ComparisonExpression>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Exp> Parser::term() {
    std::unique_ptr<Exp> expr = factor();

    while (match(TokenType::MINUS, TokenType::PLUS)) {
        Token operatorToken = previous();
        std::unique_ptr<Exp> right = factor();
        
        Operador op;
        switch (operatorToken.type) {
            case TokenType::PLUS:  op = Operador::SOMA; break;
            case TokenType::MINUS: op = Operador::SUB; break;
            default: throw std::runtime_error("Operador invalido em term()");
        }
        
        expr = std::make_unique<OpBin>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Exp> Parser::factor() {
    std::unique_ptr<Exp> expr = unary();

    while (match(TokenType::DIVIDE, TokenType::MULTIPLY)) {
        Token operatorToken = previous();
        std::unique_ptr<Exp> right = unary();
        
        Operador op;
        switch (operatorToken.type) {
            case TokenType::MULTIPLY: op = Operador::MULT; break;
            case TokenType::DIVIDE:   op = Operador::DIV; break;
            default: throw std::runtime_error("Operador invalido em factor()");
        }
        
        expr = std::make_unique<OpBin>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Exp> Parser::unary() {
    if (match(TokenType::NOT)) {
        std::unique_ptr<Exp> expr = unary();
        return std::make_unique<UnaryExpression>(std::move(expr), true);
    }

    return primary();
}

std::unique_ptr<Exp> Parser::primary() {
    if (match(TokenType::NUMBER)) {
        Token token = previous();
        int valor = std::stoi(token.lexeme);
        return std::make_unique<Const>(valor);
    }

    if (match(TokenType::TRUE)) {
        return std::make_unique<BooleanLiteral>(true);
    }
    
    if (match(TokenType::FALSE)) {
        return std::make_unique<BooleanLiteral>(false);
    }

    if (match(TokenType::IDENTIFIER)) {
        Token token = previous();
        
        if (match(TokenType::LPAREN)) {
            std::vector<std::unique_ptr<Exp>> arguments;
            
            if (!check(TokenType::RPAREN)) {
                do {
                    arguments.push_back(expression());
                } while (match(TokenType::COMMA));
            }
            
            verificaProxToken(TokenType::RPAREN);
            return std::make_unique<FunctionCall>(token.lexeme, std::move(arguments));
        }
        
        return std::make_unique<Variable>(token.lexeme);
    }

    if (match(TokenType::LPAREN)) {
        std::unique_ptr<Exp> expr = expression();
        verificaProxToken(TokenType::RPAREN);
        return expr;
    }

    throw std::runtime_error("Erro de sintaxe: esperava numero, variavel ou '('.");
}
