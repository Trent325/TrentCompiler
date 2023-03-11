#include <fstream> 
#include <iostream>
#include <sstream> 
#include <vector>

#include "parser.h" 

using namespace std;

// create a to String to print out Tokens
string TokenTypeToString(TokenType type) {
    switch(type) {
        case TokenType::TK_OPEN_PAREN: return "LEFT_PAREN";
        case TokenType::TK_CLOSE_PAREN: return "RIGHT_PAREN";
        case TokenType::TK_OPEN_BRACE: return "LEFT_BRACE";
        case TokenType::TK_CLOSE_BRACE: return "RIGHT_BRACE";
        case TokenType::TK_EOF: return "END OF PROGRAM";
        case TokenType::TK_DIGIT: return "DIGIT";
        case TokenType::TK_EQUAL: return "EQUALS";
        case TokenType::TK_INT: return "INT";
        case TokenType::TK_STRING: return "STRING";
        case TokenType::TK_ID: return "IDENTIFIER";
        case TokenType::TK_BTYPE: return "BASIC TYPE";
        case TokenType::TK_I_TYPE: return "INTEGER TYPE";
        case TokenType::TK_S_TYPE: return "STRING TYPE";
        case TokenType::TK_PRINT: return "PRINT";
        case TokenType::TK_ASSIGN: return "ASSIGN";
        case TokenType::TK_COMMENT: return "COMMENT";
        case TokenType::TK_WHILE: return "WHILE";
        case TokenType::TK_IF: return "IF";
        case TokenType::TK_TRUE: return "BOOLEAN TRUE";
        case TokenType::TK_FALSE: return "BOOLEAN FALSE";
        case TokenType::TK_NOT_EQUAL: return "NOT EQUAL";
        case TokenType::TK_CHAR: return "CHARACTER";
        case TokenType::TK_QUOTE: return "QUOTE";
        case TokenType::TK_PLUS: return "PLUS";
        case TokenType::TK_WARNING: return "WARNING";
        case TokenType::TK_ERROR: return "ERROR";
        case TokenType::TK_MULTLN_STRING: return " ";
        case TokenType::TK_INVALID_STRING_CHAR: return " ";
        case TokenType::TK_SPACE: return "SPACE";
        case TokenType::TK_B_TYPE: return "BOOLEAN";
        default: return "UNKNOWN";
    }
}

// method to split the token stream up into programs to be parsed
vector<Token> createNewVector(int start, int end, vector<Token> originalVector) {
    vector<Token> newVector;
    for (int i = start; i <= end; i++) {
        newVector.push_back(originalVector[i]);
    }
    return newVector;
}

// My method to print out tokens
void printToken(const Token& token) {
    if(token.type == TokenType::TK_ERROR){
        cout << "INFO LEXER - " << TokenTypeToString(token.type) ;
        cout << " [ " << token.lexeme << " ] " ;
        cout << " UNRECOGNIZED TOKEN @  " << token.line ;
        cout << " : " << token.position << endl;
    } 
    else if(token.type == TokenType::TK_MULTLN_STRING){
        cout << "INFO LEXER - " << " ERROR MULTI LINE STRING  ";
        cout << " [ " << token.lexeme << " ] " ;
        cout << TokenTypeToString(token.type)  << token.line ;
        cout << " : " << token.position << endl;
    }
    else if(token.type == TokenType::TK_INVALID_STRING_CHAR){
        cout << "INFO LEXER - " << " ERROR INVALID STRING CHARACTER  ";
        cout << " [ " << token.lexeme << " ] " ;
        cout << TokenTypeToString(token.type)  << token.line ;
        cout << " : " << token.position << endl;
    }else {
        cout << "DEBUG LEXER - " << TokenTypeToString(token.type) ;
        cout << " [ " << token.lexeme << " ] " ;
        cout << " Found at  " << token.line ;
        cout << " : " << token.position << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return 1;
    }
    // Read the entire contents of the file into a string
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string source = buffer.str();
    // Create a lexer object and tokenize the input
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    // Create a Count for Programs
    int programCount = 1;
    // Init message
    cout << "INFO LEXER STARTED ON PROGRAM " << programCount << endl;
    int Warnings = 0;
    int Errors = 0;
    int StartOfProgram = 0;
    // iterate over each element in the vector
    for (int i = 0; i < tokens.size(); i++) {
        // If the token is a warning increase the count 
        if (tokens[i].type == TokenType::TK_WARNING) {
            Warnings++;
        }
        // If the token is an error  increase the count
        if (tokens[i].type == TokenType::TK_ERROR || 
                tokens[i].type == TokenType::TK_MULTLN_STRING || 
                    tokens[i].type == TokenType::TK_INVALID_STRING_CHAR) {
            Errors++;
        }
        // print tokens
        printToken(tokens[i]);
        //If a program ended increment the PC and output
        if (tokens[i].type == TokenType::TK_EOF) {
            // End of lexing the program
            if(Errors != 0){
                cout <<"\n"<< "INFO LEXER FAILED WITH "<< Warnings << " WARNINGS AND " << Errors << " ERRORS";
                cout <<"\n"<< "INFO PARSER SKIPPED DUE TO LEXER ERROR";
            } else {
                cout <<"\n" <<"INFO LEXER FINISHED WITH "<< Warnings << " WARNINGS AND " << Errors << " ERRORS";
                // if there is a good lex than parse
                
                vector<Token> TokensToParse = createNewVector(StartOfProgram, i, tokens);
                StartOfProgram = i;
                cout << "\n" << "INFO PARSER STARTED ON PROGRAM " << programCount << endl;
                parse(TokensToParse);
                programCount++;
                if (i + 1 < tokens.size()) {
                    cout << "\n" << "INFO LEXER STARTED ON PROGRAM " << programCount << endl;
                }
             }
        }
    } 
}
