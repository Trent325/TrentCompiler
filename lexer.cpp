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
  TK_BTYPE,
  TK_ITYPE,
  TK_STYPE,
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
  TK_SPACE
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
  
  int OpenQuoteFlag = 2;

  Token GetNextToken() {
    
    

    if(input_[pos_] == ' '){
      if(OpenQuoteFlag %2 == 0){
        SkipWhitespace();
      }
      else{
        std::string value = input_.substr(pos_,pos_+1);
        char first_char = value[0];
        std::string s(1, first_char);
        ++pos_;
        return Token{TK_SPACE, s,"SPACE",pos_};

      }
    }

    if (input_[pos_] == '{') {
      ++pos_;
      return Token{TK_OPEN_BRACE, "{","OPEN BLOCK", pos_};
    }
    if (input_[pos_] == '}') {
      ++pos_;
      return Token{TK_CLOSE_BRACE, "}","CLOSEBLOCK", pos_};
    }
  // I had to write this as GPT was useless here
     if (input_[pos_] == '/') {
      if (input_[pos_ + 1] == '*') {
        ++pos_;
        ++pos_;
        int start = pos_;
        while (!(input_[pos_] == '*' && input_[pos_ + 1] == '/') && pos_ < input_.length()) {
          ++pos_;
        }
        if (pos_ >= input_.length()) {
          pos_ = input_.length()-1; //checks for terminating character after bad comment block
          return Token{TK_WARNING, "Unterminated comment block","WARNING", pos_};
        }
        pos_ += 2;
        return Token{TK_COMMENT, " ","COMMENT", pos_};
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
      if(OpenQuoteFlag % 2 == 0){
        ++pos_;
        return Token{TK_PLUS, "+","PLUS SIGN", pos_};
      }
      else{
        ++pos_;
        return Token{TK_ERROR, "Invalid Character inside quotes","ERROR", pos_};
      }
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
      std::string value = input_.substr(pos_,pos_+1);
      char first_char = value[0];
      std::string s(1, first_char);
      if(OpenQuoteFlag % 2 == 0){
          ++pos_;
          return Token{TK_INT, s,"DIGIT", pos_};
      }
      else{
        ++pos_;
        return Token{TK_CHAR, s,"CHAR",pos_};
      } 
    }

    if (isalpha(input_[pos_])) {
      int start = pos_;
      while (isalpha(input_[pos_])) {
        ++pos_;
      }
      int duration = pos_ - start;
      std::string value = input_.substr(start, pos_ - start);

      if(OpenQuoteFlag %2 == 0){
        if (value == "print") {
        return Token{TK_PRINT, "print","PRINT", pos_};
        }

        if (value == "int") {
          return Token{TK_ITYPE, "int","TYPE", pos_};
        }

        if (value == "string") {
          return Token{TK_STYPE, "string","TYPE", pos_};
        }

        if (value == "boolean") {
          return Token{TK_BTYPE, "boolean","TYPE", pos_};
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
        else{
          pos_ = start;
          char first_char = value[0]; // add a check to see if value contains any of the accepted strings
          std::string s(1, first_char);
          if(OpenQuoteFlag % 2 == 0){
            if(duration >= 2){
              if(value.substr(0,2) == "if"){
                pos_++;
                pos_++;
                return Token{TK_IF, "if","IF STATEMENT", pos_};
              }
            }
            if(duration >= 3){
              if(value.substr(0,3) == "int"){
                pos_++;
                pos_++;
                pos_++;
                return Token{TK_ITYPE, "int","TYPE", pos_};
              }
            }
            if(duration >= 4){
              if(value.substr(0,4) == "true"){
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                return Token{TK_TRUE, "true","TRUE ", pos_};
              }
            }
            if(duration >= 5){
              if(value.substr(0,5) == "false"){
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                return Token{TK_FALSE, "false","FALSE", pos_};
              }
              if(value.substr(0,5) == "print"){
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                return Token{TK_PRINT, "print","PRINT", pos_};
              }
              if(value.substr(0,5) == "while"){
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                return Token{TK_WHILE, "while","WHILE STATEMENT", pos_};
              }
            }
            if(duration >= 6){
              if(value.substr(0,6) == "string"){
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                return Token{TK_STYPE, "string","TYPE", pos_};
              }
            }
            if(duration >= 7){
              if(value.substr(0,7) == "boolean"){
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                pos_++;
                return Token{TK_BTYPE, "boolean","TYPE", pos_};
              }
            }
            
            ++pos_;
            return Token{TK_ID, s,"ID", pos_};
            }
          } 

      }
      else{
          pos_ = start;
          char first_char = value[0]; // add a check to see if value contains any of the accepted strings
          std::string s(1, first_char);
          ++pos_;
          return Token{TK_CHAR,s,"CHAR",pos_};
        } 
      
    }

    if (input_[pos_] == '"') {
      OpenQuoteFlag++;
      ++pos_;
      return Token{TK_QUOTE, "\"", "QUOTE", pos_};
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
    if(token.type != TK_COMMENT){
      std::cout << "DEBUG LEXER - "<< token.word << " [ " << token.value << " ] " << "found at " << line << ":" <<token.position <<std::endl;
    }
    if(token.position >= input.length()){
      break;
      }
    } while (token.type != TK_EOF);
return tokens;
}


