#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer {
public:
    Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position = 0;
    char current_char = '\0';

    void advance();
    Token make_token(TokenType type, std::string lexeme);
    Token number();
    Token identifier();
    void skip_whitespace();
};
