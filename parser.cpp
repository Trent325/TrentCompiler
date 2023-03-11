#include <vector>
#include <stdexcept>

#include "parser.h" 

using namespace std;

// Global variables
vector<Token> tokens;
int currentTokenIndex = 0;

// Forward declarations
void match(string expectedType);
void Program();
void Block();
void StatementList();
void Statement();
void PrintStatement();
void AssignmentStatement();
void VarDecl();
void WhileStatement();
void IfStatement();
void Expr();
void IntExpr();
void StringExpr();
void BooleanExpr();
void Boolval();
void Digit();
void Id();
void CharList();
// method to parse strings
string TokenTypeToStringOne(TokenType type) {
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
// Main function to parse the input
void parse(vector<Token> tokenStream) {
    tokens = tokenStream;
    cout << "PARSER : parse()..." << endl;
    // Start parsing with the Program non-terminal
    Program();
    // Check that all input has been consumed
    if (currentTokenIndex != tokens.size() || tokens.back().type !=  TokenType::TK_EOF) {
        throw runtime_error("Syntax error: unexpected token");
    }
}
// Helper function to match the expected token type
void match(TokenType type) {
    if (currentTokenIndex >= tokens.size()) {
        currentTokenIndex += currentTokenIndex -3;
    }
    if (tokens[currentTokenIndex].type != type) {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : " <<TokenTypeToStringOne(type);
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");   
    }
    // Advance to the next token
    currentTokenIndex++;
}
// Non-terminal functions
void Program() {
    cout << "PARSER : parseProgram()..." << endl;
    Block();
    match(TokenType::TK_EOF);
}
// to parse Block 
void Block() {
    cout << "PARSER : parseBlock()..." << endl;
    match(TokenType::TK_OPEN_BRACE);
    StatementList();
    match(TokenType::TK_CLOSE_BRACE);
}
// to parse Statement List
void StatementList() {
    cout << "PARSER : parseStatementList()..." << endl;
    if (currentTokenIndex >= tokens.size() || tokens[currentTokenIndex].type == TokenType::TK_CLOSE_BRACE) {
        // Empty production
        return;
    }
    Statement();
    StatementList();
}
// to parse statement
void Statement() {
    cout << "PARSER : parseStatement()..." << endl;
    if (tokens[currentTokenIndex].type == TokenType::TK_PRINT) {
        PrintStatement();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE) {
        VarDecl();
    }else if (tokens[currentTokenIndex].type == TokenType::TK_S_TYPE) {
        VarDecl();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_B_TYPE) {
        VarDecl();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_WHILE) {
        WhileStatement();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_IF) {
        IfStatement();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_OPEN_BRACE) {
        Block();
    } else {
        AssignmentStatement();
    }
}
// to parse Print Statement
void PrintStatement() {
    cout << "PARSER : parsePrint()..." << endl;
    match(TokenType::TK_PRINT);
    match(TokenType::TK_OPEN_PAREN);
    Expr();
    match(TokenType::TK_CLOSE_PAREN);
}
// to parse Assignment Statement
void AssignmentStatement() {
    cout << "PARSER : parseAssignment()..." << endl;
    Id();
    match(TokenType::TK_ASSIGN);
    Expr();
}
// probably take in a token Type as the parameter and have logic to do the right one
void VarDecl() {
    cout << "PARSER : parseVarDecl()..." << endl;
    TokenType type = tokens[currentTokenIndex].type;
    match(type);
    // TODO: this is going to take more than a simple match 
    // because we need to match any type and there are a few
    // match("type");
    // TODO: maybe change my lexer to be like the verision I handed in,
    // creating a stream of tokens for inside the quotes would be a good idea
    Id();
}
// to parse a While Statement
void WhileStatement() {
    cout << "PARSER : parseWhile()..." << endl;
    match(TokenType::TK_WHILE);
    BooleanExpr();
    Block();
}
// to parse and IF
void IfStatement() {
    cout << "PARSER : parseIf()..." << endl;
    match(TokenType::TK_IF);
    BooleanExpr();
    Block();
}
// to Parse Expr
void Expr() {
    cout << "PARSER : parseExpr()..." << endl;
    if (tokens[currentTokenIndex].type == TokenType::TK_DIGIT) {
        Digit();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_S_TYPE) {
        StringExpr();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_STRING) {
        StringExpr();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_QUOTE) {
        StringExpr();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_B_TYPE) {
        BooleanExpr();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_TRUE) {
        BooleanExpr();
    } else {
        Id();
    }
}
// to parse IntExpr
void IntExpr() {
    cout << "PARSER : parseIntExpr()..." << endl;
    match(TokenType::TK_DIGIT);
    cout << "PARSER : parseInt()..." << endl;
    if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE) {
        match(TokenType::TK_I_TYPE);
        Expr();
    }
}
// To parse Strings
void StringExpr() {
    cout << "PARSER : parseStringExpr()..." << endl;
    match(TokenType::TK_QUOTE);
    CharList();
    match(TokenType::TK_QUOTE);
}
// to match Bools
void BooleanExpr() {
    cout << "PARSER : parseBooleanExpr()..." << endl;
    if(tokens[currentTokenIndex].type == TokenType::TK_TRUE){
       Boolval();
    }
    if(tokens[currentTokenIndex].type == TokenType::TK_FALSE){
        Boolval();
    }

}
// to Parse boolVals
void Boolval(){
    cout << "PARSER : parseBooleanValue()..." << endl;
    if(tokens[currentTokenIndex].type == TokenType::TK_TRUE){
        match(TokenType::TK_TRUE);
    }
    if(tokens[currentTokenIndex].type == TokenType::TK_FALSE){
        match(TokenType::TK_FALSE);
    }

}
// to match ID
void Id() {
    cout << "PARSER : parseID()..." << endl;
    match(TokenType::TK_ID);
}
// to parse numbers with multiple digits
void Digit(){
    while(tokens[currentTokenIndex].type == TokenType::TK_DIGIT){
        cout << "PARSER : parseDigit()..." << endl;
        match(TokenType::TK_DIGIT);
        currentTokenIndex++;
    }
}
// maybe I do need it?
void CharList() {
    cout << "PARSER : parseChar()..." << endl;
    if (tokens[currentTokenIndex].type == TokenType::TK_CHAR ||
        tokens[currentTokenIndex].type == TokenType::TK_SPACE) {
        match(tokens[currentTokenIndex].type);
        CharList();
    }
    // else: empty production
}