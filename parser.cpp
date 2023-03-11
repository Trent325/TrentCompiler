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
void Id();
void CharList();

// Main function to parse the input
void parse(vector<Token> tokenStream) {
    tokens = tokenStream;
    cout << "\nPARSE" << endl;
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
        throw runtime_error("Syntax error: unexpected end of input");
    }
    if (tokens[currentTokenIndex].type != type) {
        throw runtime_error("Syntax error: unexpected token");
    }
    // Advance to the next token
    currentTokenIndex++;
}

// Non-terminal functions
void Program() {
    cout << "PROGRAM" << endl;
    Block();
    match(TokenType::TK_EOF);
}

void Block() {
    cout << "BLOCK" << endl;
    match(TokenType::TK_OPEN_BRACE);
    StatementList();
    match(TokenType::TK_CLOSE_BRACE);
}

void StatementList() {
    cout << "STATEMENTLIST" << endl;
    if (currentTokenIndex >= tokens.size() || tokens[currentTokenIndex].type == TokenType::TK_CLOSE_BRACE) {
        // Empty production
        return;
    }
    Statement();
    StatementList();
}

void Statement() {
    cout << "STATEMENT" << endl;
    if (tokens[currentTokenIndex].type == TokenType::TK_PRINT) {
        PrintStatement();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE) {
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

void PrintStatement() {
    cout << "PrintStatement" << endl;
    match(TokenType::TK_PRINT);
    match(TokenType::TK_OPEN_PAREN);
    Expr();
    match(TokenType::TK_CLOSE_PAREN);
}

void AssignmentStatement() {
    cout << "ASSIGNMENT STATEMENT" << endl;
    Id();
    match(TokenType::TK_ASSIGN);
    Expr();
}
// probably take in a token Type as the parameter and have logic to do the right one
void VarDecl() {
    cout << "VARDEC" << endl;
    TokenType type = tokens[currentTokenIndex].type;
    match(type);
    // TODO: this is going to take more than a simple match 
    // because we need to match any type and there are a few
    // match("type");
    Id();
}

void WhileStatement() {
    cout << "WHILE STATEMENT" << endl;
    match(TokenType::TK_WHILE);
    BooleanExpr();
    Block();
}

void IfStatement() {
    cout << "IF STATEMENT" << endl;
    match(TokenType::TK_IF);
    BooleanExpr();
    Block();
}

void Expr() {
    cout << "EXPR" << endl;
    if (tokens[currentTokenIndex].type == TokenType::TK_DIGIT) {
        IntExpr();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_S_TYPE) {
        StringExpr();
    } else if (tokens[currentTokenIndex].type == TokenType::TK_B_TYPE) {
        BooleanExpr();
    } else {
        Id();
    }
}

void IntExpr() {
    cout << "INT EXPR" << endl;
    match(TokenType::TK_DIGIT);
    if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE) {
        match(TokenType::TK_I_TYPE);
        Expr();
    }
}

void StringExpr() {
    cout << "STRING EXPR" << endl;
    match(TokenType::TK_QUOTE);
    CharList();
    match(TokenType::TK_QUOTE);
}

void BooleanExpr() {
    cout << "BOOLEAN EXPR" << endl;
    match(TokenType::TK_OPEN_PAREN);
    Expr();
    //match() still gonna be hard 
    Expr();
    match(TokenType::TK_CLOSE_PAREN);
}
// to match ID
void Id() {
    cout << "ID" << endl;
    match(TokenType::TK_ID);
}

// maybe I do need it?
void CharList() {
    if (tokens[currentTokenIndex].type == TokenType::TK_CHAR ||
        tokens[currentTokenIndex].type == TokenType::TK_SPACE) {
        match(tokens[currentTokenIndex].type);
        CharList();
    }
    // else: empty production
}
