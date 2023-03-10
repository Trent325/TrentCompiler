#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>

// creates a type for all the tokens
// ChatGPT wrote my last version of my Lexer with me now I decided to make a better one
enum class TokenType {
  TK_EOF,
  TK_INT,
  TK_STRING,
  TK_ID,
  // TODO: remove?
  TK_BTYPE,
  TK_I_TYPE,
  TK_S_TYPE,
  TK_PRINT,
  TK_ASSIGN,
  TK_OPEN_BRACE,
  TK_CLOSE_BRACE,
  TK_OPEN_PAREN,
  TK_CLOSE_PAREN,
  TK_COMMENT,
  TK_WHILE,
  TK_IF,
  TK_TRUE,
  TK_FALSE,
  TK_NOT_EQUAL,
  TK_EQUAL,
  TK_CHAR,
  TK_QUOTE,
  TK_PLUS,
  TK_WARNING, 
  TK_ERROR,
  TK_SPACE,
  TK_DIGIT,
  TK_UNTERMINATED_STRING,
  TK_MULTLN_STRING,
  TK_INVALID_STRING_CHAR,
  TK_B_TYPE
};

// structures the type Token
class Token {
public:
    Token(TokenType type, std::string lexeme, int line, int position)
        : type(type), lexeme(lexeme), line(line), position(position) {}

    TokenType type;
    std::string lexeme;
    int line;
    int position;
};

// lexer class to handle lines of code
class Lexer {
public:
    Lexer(std::string source) : source(source), current(0), line(1), position(1) {}
    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;

    int current;
    int start;
    int line;
    int position;
    int line_start;
    bool in_comment = false;

    void scan_token();
    void add_token(TokenType type);
    void add_token(TokenType type, std::string text);
    void add_tokens(TokenType type, std::string text);
    void string();
    void number();
    void identifier();
    bool match(char expected);
    char advance();
    char peek();
    bool is_alpha(char c);
    bool is_alphanumeric(char c);
    bool is_at_end();
    bool is_digit(char c);
    bool is_valid_string(char c);
};

#endif // LEXER_H
