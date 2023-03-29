#include <vector>
#include <stdexcept>

#include "parser.h" 
#include "ast.h"

using namespace std;

// Global variables
vector<Token> tokens;
int currentTokenIndex = 0;

// Forward declarations
void match(string expectedType);
void Program(Tree* tree);
void Block(Tree* tree);
void StatementList(Tree* tree);
void Statement(Tree* tree);
void PrintStatement(Tree* tree);
void AssignmentStatement(Tree* tree);
void VarDecl(Tree* tree);
void WhileStatement(Tree* tree);
void IfStatement(Tree* tree);
void Expr(Tree* tree);
void IntExpr(Tree* tree);
void StringExpr(Tree* tree);
void BooleanExpr(Tree* tree);
void BoolVal(Tree* tree);
void Digit(Tree* tree);
void Id(Tree* tree);
void IntOp(Tree* tree);
void CharList(Tree* tree);
void BooleanOp(Tree* tree);
void Char(Tree* tree);
void Space(Tree* tree);

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
        case TokenType::TK_BOOLOP: return "BOOLEAN OPERATION";
        default: return "UNKNOWN";
    }
}

// Helper function to match the expected token type
void match(TokenType type, Tree* tree) {
    if (currentTokenIndex >= tokens.size()) {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : [ $ ]" ;
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");   
    
    }
    else if (tokens[currentTokenIndex].type != type) {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : " <<TokenTypeToStringOne(type);
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");   
    } else{
        tree->addNode(tokens[currentTokenIndex].lexeme, "leaf");
        currentTokenIndex++;
    }
    
}
// Main function to parse the input
Tree* parse(vector<Token> tokenStream) {
    tokens = tokenStream;
    currentTokenIndex = 0;
    cout << "PARSER : parse()..." << endl;

    Tree* tree = new Tree();
    Program(tree);
    // Check that all input has been consumed
    if (currentTokenIndex != tokens.size() || tokens.back().type !=  TokenType::TK_EOF) {
        throw runtime_error("Syntax error: unexpected token");
    }
    return tree;
}
//Non Terminals
void Program(Tree* tree) {
    cout << "PARSER : parseProgram()..." << endl;
    tree->addNode("Program", "branch");
    Block(tree);
    match(TokenType::TK_EOF, tree);
    tree->endChildren();
}

// to parse Block 
void Block(Tree* tree) {
    cout << "PARSER : parseBlock()..." << endl;

    tree->addNode("Block", "branch");
    match(TokenType::TK_OPEN_BRACE, tree);
    
    StatementList(tree);

    match(TokenType::TK_CLOSE_BRACE, tree);
    
    tree->endChildren();
}

// to parse Statement List
void StatementList(Tree* tree) {
    cout << "PARSER : parseStatementList()..." << endl;
    if (currentTokenIndex >= tokens.size() || tokens[currentTokenIndex].type == TokenType::TK_CLOSE_BRACE) {
        // Empty production
        return;
    }
    tree->addNode("StatementList", "branch");
    Statement(tree);
    StatementList(tree);
    tree->endChildren();
}

// to parse statement
void Statement(Tree* tree) {
    cout << "PARSER : parseStatement()..." << endl;
    tree->addNode("Statement", "branch");
    if (tokens[currentTokenIndex].type == TokenType::TK_PRINT) {
        PrintStatement(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_ID){
        AssignmentStatement(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_I_TYPE
                || tokens[currentTokenIndex].type == TokenType::TK_S_TYPE
                    || tokens[currentTokenIndex].type == TokenType::TK_B_TYPE){
        VarDecl(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_WHILE){
        WhileStatement(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_IF){
        IfStatement(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_OPEN_BRACE){
        Block(tree);
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : statement " ;
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
    tree->endChildren();
}

// to parse Print Statement
void PrintStatement(Tree* tree) {
    cout << "PARSER : parsePrint()..." << endl;
    tree->addNode("PrintStatement", "branch");
    match(TokenType::TK_PRINT, tree);
    match(TokenType::TK_OPEN_PAREN, tree);
    Expr(tree);
    match(TokenType::TK_CLOSE_PAREN, tree);
    
    tree->endChildren();
}

// to parse Assignment Statement
void AssignmentStatement(Tree* tree) {
    cout << "PARSER : parseAssignment()..." << endl;
    tree->addNode("AssignmentStatement", "branch");
    Id(tree);
    match(TokenType::TK_ASSIGN, tree);
    Expr(tree);
    tree->endChildren();
}

// to parse a variable decl
void VarDecl(Tree* tree) {
    cout << "PARSER : parseVarDecl()..." << endl;
    tree->addNode("VarDecl", "branch");
    TokenType type = tokens[currentTokenIndex].type;
    match(type, tree);
    Id(tree);
    tree->endChildren();
}

// to parse a While Statement
void WhileStatement(Tree* tree) {
    cout << "PARSER : parseWhile()..." << endl;

    tree->addNode("WhileStatement", "branch");
    match(TokenType::TK_WHILE, tree);

    BooleanExpr(tree);
    Block(tree);

    tree->endChildren();
}

// to parse and IF
void IfStatement(Tree* tree) {
    cout << "PARSER : parseIf()..." << endl;
    tree->addNode("IfStatement", "branch");
    match(TokenType::TK_IF, tree);
    BooleanExpr(tree);
    Block(tree);
    tree->endChildren();
}

// to Parse Expr
void Expr(Tree* tree) {
    cout << "PARSER : parseExpr()..." << endl;
    tree->addNode("Expr", "branch");
    if (tokens[currentTokenIndex].type == TokenType::TK_DIGIT) {
        IntExpr(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_QUOTE) {
        StringExpr(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_OPEN_PAREN
                || tokens[currentTokenIndex].type == TokenType::TK_TRUE 
                    || tokens[currentTokenIndex].type == TokenType::TK_FALSE) {
        BooleanExpr(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_ID){
        Id(tree);
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : digit || quote || ( || true || false || Id " ;
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
    tree->endChildren();
}

// to parse IntExpr
void IntExpr(Tree* tree) {
    cout << "PARSER : parseIntExpr()..." << endl;
    tree->addNode("IntExpr", "branch");

    Digit(tree);
    if(tokens[currentTokenIndex].type != TokenType::TK_PLUS){
        return; //Empty Production
    } else {
        IntOp(tree);
        Expr(tree);
    }
    tree->endChildren(); 
}

// To parse Strings
void StringExpr(Tree* tree) {
    cout << "PARSER : parseStringExpr()..." << endl;
    tree->addNode("StringExpr", "branch");
    match(TokenType::TK_QUOTE, tree);
    CharList(tree);
    match(TokenType::TK_QUOTE, tree);
    tree->endChildren();
}

// to match Bools
void BooleanExpr(Tree* tree) {
    cout << "PARSER : parseBooleanExpr()..." << endl;
    tree->addNode("BooleanExpr", "branch");

    if(tokens[currentTokenIndex].type != TokenType::TK_OPEN_PAREN){
        BoolVal(tree);
    } else {
        match(TokenType::TK_OPEN_PAREN, tree);
        Expr(tree);
        BooleanOp(tree);
        Expr(tree);
        match(TokenType::TK_CLOSE_PAREN, tree);
    }
    tree->endChildren();
}

// to match ID
void Id(Tree* tree) {
    cout << "PARSER : parseID()..." << endl;
    tree->addNode(TokenTypeToStringOne(TokenType::TK_ID), "branch");
    
    match(TokenType::TK_ID, tree);
    
    tree->endChildren();
}

// definitly need it 
void CharList(Tree* tree) {
    cout << "PARSER : parseCharList()..." << endl;
    tree->addNode("CharList", "branch");
    if (tokens[currentTokenIndex].type == TokenType::TK_CHAR) {
        Char(tree);
        CharList(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_SPACE){
        Space(tree);
        CharList(tree);
    } else if (tokens[currentTokenIndex].type == TokenType::TK_QUOTE){
        return; //Empty Production
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : character || space || quote" ;
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
        tree->endChildren();
}

//parse char
void Char(Tree* tree){
    cout << "PARSER : parseChar()..." << endl;
    
    match(TokenType::TK_CHAR,tree);
    
}

// to parse numbers NO MULTI DIGITS NO MORE
void Digit(Tree* tree){
        cout << "PARSER : parseDigit()..." << endl;
        match(TokenType::TK_DIGIT, tree);
     
}

// to parse a space 
void Space(Tree* tree){
    match(TokenType::TK_SPACE,tree);

}

// to parse a boolean operation
void BooleanOp(Tree* tree){
    if(tokens[currentTokenIndex].type == TokenType::TK_NOT_EQUAL 
        || tokens[currentTokenIndex].type == TokenType::TK_BOOLOP ){
        match(tokens[currentTokenIndex].type, tree);
    }
    else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : != || == " ;
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
}

// to parse a boolean value
void BoolVal(Tree* tree){
    if(tokens[currentTokenIndex].type == TokenType::TK_TRUE 
        || tokens[currentTokenIndex].type == TokenType::TK_FALSE ){
        match(tokens[currentTokenIndex].type, tree);
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToStringOne(tokens[currentTokenIndex].type) << " Expected this token : true || false" ;
        cout << "\nAt index "<< tokens[currentTokenIndex].line << " : " << tokens[currentTokenIndex].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
}

// to parse and Int operation 

void IntOp(Tree* tree){
    cout << "PARSER : parseIntOp()..." << endl;
    
    match(TokenType::TK_PLUS, tree);
    
}