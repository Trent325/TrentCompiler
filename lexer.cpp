#include <iostream>
#include <vector>
#include <string>

//creates a type for all the tokens
//ChatGPT wrote lines 7 - 154 originally, I added to the structures of the types
enum TokenType {
  TK_EOF,
  TK_INT,
  TK_STRING,
  TK_ID,
  TK_TYPE,
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
  TK_PLUS,
  TK_ERROR
};

//structures the type
// I added the value word and position to this type, in order to keep track of the position of the elements
struct Token {
  TokenType type;
  std::string value;
  std::string word;
  int position;

};
//lexer class to handle lines of code
class Lexer {
 public:
  explicit Lexer(const std::string& input) : input_(input) {}

//Do I need to go back and track these?
  Token GetNextToken() {
    SkipWhitespace();

    if (input_[pos_] == '{') {
      ++pos_;
      return Token{TK_OPEN_BRACE, "{","OPEN BLOCK", pos_};
    }

    if (input_[pos_] == '}') {
      ++pos_;
      return Token{TK_CLOSE_BRACE, "}","CLOSEBLOCK", pos_};
    }

     if (input_[pos_] == '/') { // I had to add the comment function as chatGPT did not do so correctly ever
      if (input_[pos_ + 1] == '*') {
        ++pos_;
        ++pos_;
        int start = pos_;
        while (!(input_[pos_] == '*' && input_[pos_ + 1] == '/')) {
          ++pos_;
        }
        pos_ += 2;
        return Token{TK_COMMENT, input_.substr(start, pos_ - start - 2), "COMMENT", start};
      }
    }

    if (input_[pos_] == '(') {
      ++pos_;
      return Token{TK_OPEN_PAREN, "(","OPENPAREN", pos_};
    }

    if (input_[pos_] == ')') {
      ++pos_;
      return Token{TK_CLOSE_PAREN, ")","CLOSEPAREN", pos_};
    }

    if (input_[pos_] == '+') {
      ++pos_;
      return Token{TK_PLUS, "+","PLUS SIGN", pos_};
    }

    if (input_[pos_] == '=') {
      ++pos_;
      if (input_[pos_] == '=') {
        ++pos_;
        return Token{TK_EQUAL, "==","EQUALS", pos_};
      }
      return Token{TK_ASSIGN, "=" ,"ASSIGNMENT STATEMENT", pos_};
    }

    if (input_[pos_] == '$') {
        ++pos_;
    return Token{TK_EOF, "$","END OF PROGRAM", pos_};
    }

    if (input_[pos_] == '!') {
      ++pos_;
      if (input_[pos_] == '=') {
        ++pos_;
        return Token{TK_NOT_EQUAL, "!=","NOT EQUALS", pos_};
      } else {
        
      }
    }

    if (isdigit(input_[pos_])) {
      int start = pos_;
      while (isdigit(input_[pos_])) {
        ++pos_;
      }
      return Token{TK_INT, input_.substr(start, pos_ - start),"DIGIT", pos_};
    }

    if (isalpha(input_[pos_])) {
      int start = pos_;
      while (isalpha(input_[pos_])) {
        ++pos_;
      }
      std::string value = input_.substr(start, pos_ - start);
            if (value == "print") {
        return Token{TK_PRINT, "print","PRINT", pos_};
      }

      if (value == "int") {
        return Token{TK_TYPE, "int","TYPE", pos_};
      }

      if (value == "string") {
        return Token{TK_TYPE, "string","TYPE", pos_};
      }

      if (value == "boolean") {
        return Token{TK_TYPE, "boolean","TYPE", pos_};
      }

      if (value == "while") {
        return Token{TK_WHILE, "while","WHILE STATEMENT", pos_};
      }

      if (value == "if") {
        return Token{TK_IF, "if","IF STATEMENT", pos_};
      }

      if (value == "true") {
        return Token{TK_TRUE, "true","TRUE ", pos_};
      }

      if (value == "false") {
        return Token{TK_FALSE, "false","FALSE", pos_};
      }

      return Token{TK_ID, value,"ID", pos_};
    }

    if (input_[pos_] == '"') {
      ++pos_;
      int start = pos_;
      while (input_[pos_] != '"') {
        ++pos_;
      }
      ++pos_;
      return Token{TK_STRING, input_.substr(start, pos_ - start - 1)};
    }

    if (!isdigit(input_[pos_]) && !isalpha(input_[pos_])) { // I had to create this function as ChatGPT could not catch errors of unknown symbols (it knows too much to find unknown)
      int start = pos_;
      ++pos_;
      return Token{TK_ERROR, input_.substr(start, pos_ - start), "ERROR UNRECOGNIZED TOKEN", start};
    }
  }

 private:
  void SkipWhitespace() {
    while (pos_ < input_.size() && isspace(input_[pos_])) {
      ++pos_;
    }
  }

  std::string input_;
  int pos_ = 0;
};
// I created all of this until the end of the file
std::vector<Token> lexer(std::string input, int line) {
std::vector<Token> tokens;
Lexer lexer(input);
Token token;

do {
    token = lexer.GetNextToken();
    tokens.push_back(token);
    std::cout << "DEBUG LEXER - "<< token.word << " [ " << token.value << " ] " << "found at " << line << ":" <<token.position <<std::endl;
    if(token.position >= input.length()){
      break;
      }
    } while (token.type != TK_EOF);
return tokens;
}


