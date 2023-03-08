#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "lexer.cpp" // assuming the lexer code is in a file called lexer.hpp

using namespace std;

class Parser {
public:
    Parser(std::vector<Token>& tokens) : tokens(tokens), current(0) {}

    void parse() {
        program();
        if (match(TokenType::TK_EOF)) {
            cout << "Parsing complete!\n";
        } else {
            throw std::runtime_error("Expected end of file");
        }
    }

private:
    std::vector<Token>& tokens;
    int current;

    void program() {
        while (!is_at_end()) {
            statement();
        }
    }

    void statement() {
        if (match(TokenType::TK_PRINT)) {
            print_statement();
            match(TokenType::TK_SEMICOLON);
        } else if (match(TokenType::TK_ID)) {
            assignment_statement();
            match(TokenType::TK_SEMICOLON);
        } else if (match(TokenType::TK_IF)) {
            if_statement();
        } else if (match(TokenType::TK_WHILE)) {
            while_statement();
        } else if (match(TokenType::TK_COMMENT)) {
            // ignore comments
        } else {
            throw std::runtime_error("Expected statement");
        }
    }

    void print_statement() {
        expression();
    }

    void assignment_statement() {
        // identifier = expression;
        // parse identifier
        string identifier = previous().lexeme;
        // parse = sign
        match(TokenType::TK_ASSIGN);
        // parse expression
        expression();
        cout << "Assigning value to " << identifier << endl;
    }

    void if_statement() {
        // if (expression) statement (else statement)?
        if (match(TokenType::TK_OPEN_PAREN)) {
            expression();
            if (match(TokenType::TK_CLOSE_PAREN)) {
                statement();
                if (match(TokenType::TK_ELSE)) {
                    statement();
                }
            } else {
                throw std::runtime_error("Expected closing parenthesis");
            }
        } else {
            throw std::runtime_error("Expected opening parenthesis");
        }
    }

    void while_statement() {
        // while (expression) statement
        if (match(TokenType::TK_OPEN_PAREN)) {
            expression();
            if (match(TokenType::TK_CLOSE_PAREN)) {
                statement();
            } else {
                throw std::runtime_error("Expected closing parenthesis");
            }
        } else {
            throw std::runtime_error("Expected opening parenthesis");
        }
    }

    void expression() {
        // term (add_op term)*
        term();
        while (match(TokenType::TK_PLUS)) {
            term();
        }
    }

    void term() {
        // factor (mult_op factor)*
        factor();
        while (match(TokenType::TK_MULT)) {
            factor();
        }
    }

    void factor() {
        if (match(TokenType::TK_INT)) {
            // do nothing, integer literals are already parsed by the lexer
        } else if (match(TokenType::TK_ID)) {
            // do nothing, identifiers are already parsed by the lexer
        } else if (match(TokenType::TK_OPEN_PAREN)) {
            expression();
            if (match(TokenType::TK_CLOSE_PAREN)) {
                // do nothing
            } else {
                throw std::runtime_error("Expected closing parenthesis");
            }
        } else {
            throw std::runtime_error("Expected integer literal, identifier, or opening parenthesis");
        }
    }

    bool match(TokenType type) {
        if (is_at_end()) {
            return false;
        }
        if (tokens[current].type == type) {
            current++;
            return true;
        } else {
            return false;
        }
    }

    const Token& advance() {
        if (!is_at_end()) {
            current++;
        }
        return previous();
    }

    bool is_at_end() {
        return current >= tokens.size();
    }

    const Token& previous() {
        return tokens[current - 1];
    }
};


