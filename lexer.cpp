#include "lexer.h"
#include <cctype>
#include <stdexcept>

std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "Numero";
        case TokenType::PLUS: return "Soma";
        case TokenType::MINUS: return "Sub";
        case TokenType::MULTIPLY: return "Mult";
        case TokenType::DIVIDE: return "Div";
        case TokenType::LPAREN: return "ParenEsq";
        case TokenType::RPAREN: return "ParenDir";
        case TokenType::SEMICOLON: return "PontoVirgula";
        case TokenType::LET: return "Let";
        case TokenType::IDENTIFIER: return "Identificador";
        case TokenType::ASSIGN: return "Atribuicao";
        case TokenType::MAIN: return "Main";
        case TokenType::LBRACE: return "ChaveEsq";
        case TokenType::RBRACE: return "ChaveDir";
        case TokenType::EQUAL: return "Igual";
        case TokenType::NOT_EQUAL: return "Diferente";
        case TokenType::LESS: return "Menor";
        case TokenType::GREATER: return "Maior";
        case TokenType::LESS_EQUAL: return "MenorIgual";
        case TokenType::GREATER_EQUAL: return "MaiorIgual";
        case TokenType::AND: return "E";
        case TokenType::OR: return "Ou";
        case TokenType::NOT: return "Nao";
        case TokenType::IF: return "Se";
        case TokenType::ELSE: return "Senao";
        case TokenType::WHILE: return "Enquanto";
        case TokenType::RETURN: return "Retorna";
        case TokenType::TRUE: return "Verdadeiro";
        case TokenType::FALSE: return "Falso";
        case TokenType::FUN: return "Fun";
        case TokenType::COMMA: return "Virgula";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::ILLEGAL: return "ErroLexico";
    }
    return "UNKNOWN";
}

Lexer::Lexer(std::string source)
    : source(std::move(source)), position(0), current_char('\0') {
    if (!this->source.empty()) {
        current_char = this->source[0];
    } else {
        current_char = '\0';
    }
}

void Lexer::advance() {
    position++;
    if (position < source.length()) {
        current_char = source[position];
    } else {
        current_char = '\0';
    }
}

void Lexer::skip_whitespace() {
    while (current_char != '\0' && isspace(current_char)) {
        advance();
    }
}

Token Lexer::make_token(TokenType type, std::string lexeme) {
    return Token{type, lexeme, position};
}

Token Lexer::number() {
    size_t start_pos = position;
    while (current_char != '\0' && isdigit(current_char)) {
        advance();
    }
    std::string lexeme = source.substr(start_pos, position - start_pos);
    return Token{TokenType::NUMBER, lexeme, start_pos};
}

Token Lexer::identifier() {
    size_t start_pos = position;
    while (current_char != '\0' && (isalnum(current_char) || current_char == '_')) {
        advance();
    }
    std::string lexeme = source.substr(start_pos, position - start_pos);
    
    TokenType type = TokenType::IDENTIFIER;
    if (lexeme == "let") {
        type = TokenType::LET;
    } else if (lexeme == "main") {
        type = TokenType::MAIN;
    } else if (lexeme == "fun") {
        type = TokenType::FUN;
    } else if (lexeme == "if") {
        type = TokenType::IF;
    } else if (lexeme == "else") {
        type = TokenType::ELSE;
    } else if (lexeme == "while") {
        type = TokenType::WHILE;
    } else if (lexeme == "return") {
        type = TokenType::RETURN;
    } else if (lexeme == "true") {
        type = TokenType::TRUE;
    } else if (lexeme == "false") {
        type = TokenType::FALSE;
    }
    
    return Token{type, lexeme, start_pos};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (current_char != '\0') {
        skip_whitespace();

        if (current_char == '\0') break;

        size_t start_pos = position;
        
        if (isdigit(current_char)) {
            tokens.push_back(number());
            continue;
        }
        
        if (isalpha(current_char) || current_char == '_') {
            tokens.push_back(identifier());
            continue;
        }

        TokenType type = TokenType::ILLEGAL;
        std::string lexeme;
        
        switch (current_char) {
            case '+': type = TokenType::PLUS; lexeme = "+"; break;
            case '-': type = TokenType::MINUS; lexeme = "-"; break;
            case '*': type = TokenType::MULTIPLY; lexeme = "*"; break;
            case '/': type = TokenType::DIVIDE; lexeme = "/"; break;
            case '(': type = TokenType::LPAREN; lexeme = "("; break;
            case ')': type = TokenType::RPAREN; lexeme = ")"; break;
            case ';': type = TokenType::SEMICOLON; lexeme = ";"; break;
            case ',': type = TokenType::COMMA; lexeme = ","; break;
            case '{': type = TokenType::LBRACE; lexeme = "{"; break;
            case '}': type = TokenType::RBRACE; lexeme = "}"; break;
            
            case '=':
                if (position + 1 < source.length() && source[position + 1] == '=') {
                    type = TokenType::EQUAL;
                    lexeme = "==";
                    advance();
                } else {
                    type = TokenType::ASSIGN;
                    lexeme = "=";
                }
                break;
                
            case '!':
                if (position + 1 < source.length() && source[position + 1] == '=') {
                    type = TokenType::NOT_EQUAL;
                    lexeme = "!=";
                    advance();
                } else {
                    type = TokenType::NOT;
                    lexeme = "!";
                }
                break;
                
            case '<':
                if (position + 1 < source.length() && source[position + 1] == '=') {
                    type = TokenType::LESS_EQUAL;
                    lexeme = "<=";
                    advance();
                } else {
                    type = TokenType::LESS;
                    lexeme = "<";
                }
                break;
                
            case '>':
                if (position + 1 < source.length() && source[position + 1] == '=') {
                    type = TokenType::GREATER_EQUAL;
                    lexeme = ">=";
                    advance();
                } else {
                    type = TokenType::GREATER;
                    lexeme = ">";
                }
                break;
                
            case '&':
                if (position + 1 < source.length() && source[position + 1] == '&') {
                    type = TokenType::AND;
                    lexeme = "&&";
                    advance();
                } else {
                    type = TokenType::ILLEGAL;
                    lexeme = "&";
                }
                break;
                
            case '|':
                if (position + 1 < source.length() && source[position + 1] == '|') {
                    type = TokenType::OR;
                    lexeme = "||";
                    advance();
                } else {
                    type = TokenType::ILLEGAL;
                    lexeme = "|";
                }
                break;
        }

        tokens.push_back(Token{type, lexeme, start_pos});
        
        if (type == TokenType::ILLEGAL) {
            throw std::runtime_error("Erro lexico: caractere ilegal '" + lexeme + "' na posicao " + std::to_string(start_pos));
        }

        advance();
    }

    tokens.push_back(Token{TokenType::END_OF_FILE, "", position});
    return tokens;
}
