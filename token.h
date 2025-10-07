#pragma once
#include <string>
#include <vector>

enum class TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    SEMICOLON,
    LET,
    IDENTIFIER,
    ASSIGN,
    MAIN,
    LBRACE,
    RBRACE,
    
    EQUAL,
    NOT,
    NOT_EQUAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,

    AND,
    OR,

    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,

    FUN,
    COMMA,
    
    END_OF_FILE,
    ILLEGAL
};


std::string token_type_to_string(TokenType type);

struct Token {
    TokenType type;
    std::string lexeme;
    size_t position;
};
