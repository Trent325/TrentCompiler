#include <iostream>
#include <vector>
#include <string>

using namespace std;

// creates a type for all the tokens
// ChatGPT wrote my last version of my Lexer with me now I decided to make a better one
enum class TokenType {
  TK_EOF,
  TK_INT,
  TK_STRING,
  TK_ID,
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
    std::vector<Token> tokenize() {
        // init position
        position = 1;
        line_start = 0;
        while (!is_at_end()) {
            start = current;
            position = current - line_start + 1; 
            scan_token();
        }
        // Check if the last token is not a $ 
        if (!tokens.empty() && tokens.back().type != TokenType::TK_EOF) {
            // Emit a warning indicating the missing $ character
            tokens.push_back(Token(TokenType::TK_WARNING, "NO $ to end the program was found", line, position));
        }
        return tokens;
    }
private:

    std::string source;
    std::vector<Token> tokens;

    int current;
    int start;
    int line;
    int position;
    int line_start;
    bool in_comment = false;

    // method assign tokens to input
    void scan_token() {
        char c = advance();
        // handles inside comments
        if (in_comment) {
            if (c == '*' && peek() == '/') {
                advance();
                in_comment = false;
            }
            // Ignore everything in the comment
            return;
        }
        //switch to handle user input
        switch (c) {
            // Single-character tokens
            case '(': add_token(TokenType::TK_OPEN_PAREN); break;
            case ')': add_token(TokenType::TK_CLOSE_PAREN); break;
            case '{': add_token(TokenType::TK_OPEN_BRACE); break;
            case '}': add_token(TokenType::TK_CLOSE_BRACE); break;
            case '+': add_token(TokenType::TK_PLUS); break;
            case '$': add_token(TokenType::TK_EOF); break;
            // One or two character tokens
            case '!': add_token(match('=') ? TokenType::TK_NOT_EQUAL : TokenType::TK_NOT_EQUAL); break;
            case '=': add_token(match('=') ? TokenType::TK_EQUAL : TokenType::TK_EQUAL); break;
            // Whitespace
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace
                break;
            case '\n':
                line++;
                position = 1;
                line_start = current;
                break;
            // Strings
            case '"': string(); break;
            // Digits
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                number(); break;
            //comments
            case '/':
            if (peek() == '*') {
                advance();
                in_comment = true;
            } else {
                add_token(TokenType::TK_ERROR);
            }
            break;
            // Idenifiers and keywords
            default:
                if (is_alpha(c)) {
                    identifier();
                } else {
                    add_token(TokenType::TK_ERROR);
                }
                break;
        }
    }
    // method to handle any input that is alphabetic
    void identifier() {
        //pointer for what char I am at
        int pos_ = current;
        while (is_alpha(peek())){
            advance(); 
        } 

        std::string text = source.substr(start, current - start);
    
        TokenType type;
        if (text == "if") type = TokenType::TK_IF;
        else if (text == "false") type = TokenType::TK_FALSE;
        else if (text == "print") type = TokenType::TK_PRINT;
        else if (text == "true") type = TokenType::TK_TRUE;
        else if (text == "while") type = TokenType::TK_WHILE;
        else if (text == "int") type = TokenType::TK_I_TYPE;
        else if (text == "string") type = TokenType::TK_S_TYPE;
        else if (text == "boolean") type = TokenType::TK_B_TYPE;
        else {
            if (text.length() > 1) {
                // not a valid identifier, check for other possible token types
                if (text.substr(0, 5) == "false") {
                    type = TokenType::TK_FALSE;
                    text = text.substr(0, 5);
                    current = pos_ +4;
                } else if (text.substr(0, 5) == "print") {
                    type = TokenType::TK_PRINT;
                    text = text.substr(0, 5);
                    current = pos_ +4;
                } else if (text.substr(0, 7) == "boolean") {
                    type = TokenType::TK_B_TYPE;
                    text = text.substr(0, 7);
                    current = pos_ +6;
                }else if (text.substr(0, 4) == "true") {
                    type = TokenType::TK_TRUE;
                    text = text.substr(0, 4);
                    current = pos_ +3;
                } else if (text.substr(0, 5) == "while") {
                    type = TokenType::TK_WHILE;
                    text = text.substr(0, 5);
                    current = pos_ +4;
                } else if (text.substr(0, 3) == "int") {
                    type = TokenType::TK_I_TYPE;
                    text = text.substr(0, 3);
                    current = pos_ +2;
                } else if (text.substr(0, 6) == "string") {
                    type = TokenType::TK_S_TYPE;
                    text = text.substr(0, 6);
                    current = pos_ +5;
                }
                else if (text.substr(0, 2) == "if") {
                    type = TokenType::TK_IF;
                    text = text.substr(0, 2);
                    current = pos_ +1;
                }else {
                    //chop up and make it a 1 character ID
                    type = TokenType::TK_ID;
                    text = text.substr(0,1);
                    current = pos_ ;
                }
            } else {
                type = TokenType::TK_ID;
                }
            // created a new tokens method to add based on a text string rather than a lexeme    
            add_tokens(type,text);
            return;
        }
        add_token(type, text);
    }
    // handles numeric input
    void number() {
        add_token(TokenType::TK_DIGIT, source.substr(start, current - start));
    }

    // method to check if a string is vlaid
    bool isValidString(char c){
        if(is_alpha(c) || isspace(c)){
            if (std::isupper(c)) {
                return false;
            }
            return true;
         }
        return false;
    }
    // method that processes strings
    void string() {
        while (peek() != '"' && !is_at_end()) {
            if (peek() == '\n'){
                //If it is a new line character
                std::string ErrorMessage = "";
                std::string nextChar(1, peek()); ;
                ErrorMessage.append(nextChar);
                add_token(TokenType::TK_MULTLN_STRING, ErrorMessage);
                line++;
            } 
            else if(!isValidString(peek())){
                //If it is an uppercase
                std::string ErrorMessage = "";
                std::string nextChar(1, peek()); ;
                ErrorMessage.append(nextChar);
                add_tokens(TokenType::TK_INVALID_STRING_CHAR, ErrorMessage);
            }
            advance();  
        }
        if (is_at_end()) {
            tokens.push_back(Token(TokenType::TK_UNTERMINATED_STRING, "UNTERMINATED STRING", line, position));
            current = source.length()-1;
            return;
        }
        // Consume the closing "
        advance();
        // Trim the surrounding quotes
        std::string value = source.substr(start + 1, current - start - 2);
        add_token(TokenType::TK_STRING, value);
    }

    // method to check if the next character is what is expected
    bool match(char expected) {
        if (is_at_end()) return false;
        if (source[current] != expected) return false;
        current++;
        return true;
    }
    // method to go to the next character of input
    char advance() {
        current++;
        return source[current - 1];
    }
    // method to view the next character of input
    char peek() {
        if (is_at_end()) return '\0';
        return source[current];
    }
    // method to ensure a character is alphabetic
    bool is_alpha(char c) {
        return (c >= 'a' && c <= 'z');
    }
    // method to see if something is a digit
    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }
    // method to find end of file
    bool is_at_end() {
        return current >= source.length();
    }
    // method to add a token 
    void add_token(TokenType type) {
        add_token(type, "");
    }
    // method to add a token and pass through text as the lexeme
    void add_tokens(TokenType type, std::string text){
        tokens.push_back(Token(type, text, line, position));
    }
    // method to add a token and pass through text to be used as nothing and have a determined lexeme
    void add_token(TokenType type, std::string text) {
        std::string lexeme = source.substr(start, current - start);
        tokens.push_back(Token(type, lexeme, line, position)); 
    }
};
            




