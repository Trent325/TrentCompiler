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
    match(TokenType::TK_PRINT);
    match(TokenType::TK_OPEN_PAREN);
    Expr();
    match(TokenType::TK_CLOSE_PAREN);
}

void AssignmentStatement() {
    Id();
    match(TokenType::TK_EQUAL);
    Expr();
}
// probably take in a token Type as the parameter and have logic to do the right one
void VarDecl() {
    // TODO: this is going to take more than a simple match 
    // because we need to match any type and there are a few
    // match("type");
    Id();
}

void WhileStatement() {
    match(TokenType::TK_WHILE);
    BooleanExpr();
    Block();
}

void IfStatement() {
    match(TokenType::TK_IF);
    BooleanExpr();
    Block();
}

void Expr() {
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
    match(TokenType::TK_DIGIT);
    if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE) {
        match(TokenType::TK_I_TYPE);
        Expr();
    }
}

void StringExpr() {
    match(TokenType::TK_QUOTE);
    CharList();
    match(TokenType::TK_QUOTE);
}

void BooleanExpr() {
    if (tokens[currentTokenIndex].type == TokenType::TK_B_TYPE) {
        match(TokenType::TK_OPEN_PAREN);
        Expr();
        // TODO: this is going to require more than a simple match
        // because we need to match EQ, NEQ, etc.
        // match("boolop");
        Expr();
        match(TokenType::TK_CLOSE_PAREN);
    } else {
        match(TokenType::TK_B_TYPE);
    }
}
// to match ID
void Id() {
    match(TokenType::TK_ID);
}

// do not think I need this
void CharList() {
        if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE) {
            // Empty production
            return;
        }
        // TODO: dupe?
        if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE) {
            // Escape sequence
            match(TokenType::TK_QUOTE);
            // TODO: unsure if this is right type?
            match(TokenType::TK_CHAR);
            CharList();
        } else {
            // Normal character
            // TODO: unsure if this is right type?
            match(TokenType::TK_CHAR);
            CharList();
        }
}
