#include <vector>
#include <stdexcept>
#include <tuple>
#include <algorithm>
#include <map>

#include "ast.h" 

using namespace std;

// Global variables
vector<Token> tokens1;
int Index = 0;
string stringToPrint = "\"";
int CorrectScope = 1;
int scopeNumber = 1;

vector<tuple<int, pair<int, int>, pair<int, int>>> VectorOfscopes;
vector<std::tuple<int, int, int>> outputVector;

// Forward declarations
void match1(string expectedType);
void Program1(Tree* tree);
void Block1(Tree* tree);
void StatementList1(Tree* tree);
void Statement1(Tree* tree);
void PrintStatement1(Tree* tree);
void AssignmentStatement1(Tree* tree);
void VarDecl1(Tree* tree);
void WhileStatement1(Tree* tree);
void IfStatement1(Tree* tree);
void Expr1(Tree* tree);
void IntExpr1(Tree* tree);
void StringExpr1(Tree* tree);
void BooleanExpr1(Tree* tree);
void BoolVal1(Tree* tree);
void Digit1(Tree* tree);
void Id1(Tree* tree);
void IntOp1(Tree* tree);
void CharList1(Tree* tree);
void BooleanOp1(Tree* tree);
void Char1(Tree* tree);
void Space1(Tree* tree);
void Scopes(pair<int, int> start, pair<int, int> end);

//method to record the scope locations 
void Scopes(pair<int, int> start, pair<int, int> end) {
    
    VectorOfscopes.push_back(make_tuple(scopeNumber, start, end));
    scopeNumber++;

}


// method to print out the current scopes
void printScopes(){
    
    for (int i = 0; i < VectorOfscopes.size(); i++) {
    auto scope = VectorOfscopes[i];
    std::cout << std::get<0>(scope) << ": (" << std::get<1>(scope).first << ", " << std::get<1>(scope).second << ") - ("
        << std::get<2>(scope).first << ", " << std::get<2>(scope).second << ")" << std::endl;
    }

}
//attempt to sort scopes 
vector<std::tuple<int, int, int>> sortScopeLinesAndPositions() {
    std::vector<std::tuple<int, int, int>> sortedScopes;
    std::map<int, int> scopeToLineMap;
    
    for (const auto& scope : VectorOfscopes) {
        auto startPair = std::get<1>(scope);
        int scopeNumber = std::get<0>(scope);
        
        if (scopeToLineMap.find(scopeNumber) == scopeToLineMap.end()) {
            scopeToLineMap[scopeNumber] = startPair.first;
        }
        
        auto endPair = std::get<2>(scope);
        sortedScopes.push_back(std::make_tuple(scopeNumber, scopeToLineMap[scopeNumber], startPair.second));
    }
    
    std::sort(sortedScopes.begin(), sortedScopes.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) < std::get<1>(b);
    });
    
    for (const auto& scope : sortedScopes) {
        
        int lineNumber = std::get<1>(scope);
        int position = std::get<2>(scope);
        std::cout << "Scope " << CorrectScope << " starts at line " << lineNumber << " position " << position << std::endl;
        CorrectScope++;
    }
    
    return sortedScopes;
}


// method to parse strings 
string TokenTypeToString1(TokenType type) {
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
void match1(TokenType type, Tree* tree) {
    if (Index >= tokens1.size()) {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToString1(tokens1[Index].type) << " Expected this token : [ $ ]" ;
        cout << "\nAt index "<< tokens1[Index].line << " : " << tokens1[Index].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");   
    
    }
    else if (tokens1[Index].type != type) {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToString1(tokens1[Index].type) << " Expected this token : " <<TokenTypeToString1(type);
        cout << "\nAt index "<< tokens1[Index].line << " : " << tokens1[Index].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");   
    } else{

        switch (tokens1[Index].type) {
            case TokenType::TK_ID:
            case TokenType::TK_I_TYPE:
            case TokenType::TK_S_TYPE:
            case TokenType::TK_DIGIT:
            case TokenType::TK_TRUE:
            case TokenType::TK_FALSE:
                tree->addNodeLocation(tokens1[Index].lexeme, "leaf", tokens1[Index].line, tokens1[Index].position);
                break;
            case TokenType::TK_PLUS:
                tree->addNodeBeginnging("ADD", "leaf");
                break;
            case TokenType::TK_BOOLOP:
                tree->addNodeBeginnging("IF EQUALS", "leaf");
                break;
            case TokenType::TK_NOT_EQUAL:
                tree->addNodeBeginnging("IF NOT EQUALS", "leaf");
                break;
            default:
                
                break;
        }
        
        Index++;
    }
    
}

// Main function to parse the input
Tree* ast(vector<Token> tokens1tream) {
    tokens1 = tokens1tream;
    Index = 0;
    VectorOfscopes.clear();
    int CorrectScope = 1;

    Tree* tree = new Tree();
    Program1(tree);
    // Check that all input has been consumed
    if (Index != tokens1.size() || tokens1.back().type !=  TokenType::TK_EOF) {
        throw runtime_error("Syntax error: unexpected token");
    }
    return tree;
}
//Non Terminals
void Program1(Tree* tree) {
    
    
    Block1(tree);
    match1(TokenType::TK_EOF, tree);
    
}

// to parse Block 
void Block1(Tree* tree) {
    

    tree->addNode("Block", "branch");
    int startOfScopeline = tokens1[Index].line;
    int startOfScopePosition = tokens1[Index].position;
    pair<int, int> Start = make_pair(startOfScopeline, startOfScopePosition);
    
    match1(TokenType::TK_OPEN_BRACE, tree);
    
    StatementList1(tree);

    int EndOfScopeline = tokens1[Index].line;
    int EndOfScopePosition = tokens1[Index].position;
    pair<int, int> End = make_pair(EndOfScopeline, EndOfScopePosition);
    Scopes(Start, End);
    match1(TokenType::TK_CLOSE_BRACE, tree);
    
    tree->endChildren();
}

// to parse Statement List
void StatementList1(Tree* tree) {
    
    if (Index >= tokens1.size() || tokens1[Index].type == TokenType::TK_CLOSE_BRACE) {
        // Empty production
        return;
    }
    
    Statement1(tree);
    StatementList1(tree);
    
}

// to parse statement
void Statement1(Tree* tree) {
    
    
    if (tokens1[Index].type == TokenType::TK_PRINT) {
        PrintStatement1(tree);
    } else if (tokens1[Index].type == TokenType::TK_ID){
        AssignmentStatement1(tree);
    } else if (tokens1[Index].type == TokenType::TK_I_TYPE
                || tokens1[Index].type == TokenType::TK_S_TYPE
                    || tokens1[Index].type == TokenType::TK_B_TYPE){
        VarDecl1(tree);
    } else if (tokens1[Index].type == TokenType::TK_WHILE){
        WhileStatement1(tree);
    } else if (tokens1[Index].type == TokenType::TK_IF){
        IfStatement1(tree);
    } else if (tokens1[Index].type == TokenType::TK_OPEN_BRACE){
        Block1(tree);
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToString1(tokens1[Index].type) << " Expected this token : statement " ;
        cout << "\nAt index "<< tokens1[Index].line << " : " << tokens1[Index].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
    
}

// to parse Print Statement
void PrintStatement1(Tree* tree) {
    
    tree->addNode("PrintStatement", "branch");
    match1(TokenType::TK_PRINT, tree);
    match1(TokenType::TK_OPEN_PAREN, tree);
    Expr1(tree);
    match1(TokenType::TK_CLOSE_PAREN, tree);
    
    tree->endChildren();
}

// to parse Assignment Statement
void AssignmentStatement1(Tree* tree) {
   
    tree->addNode("AssignmentStatement", "branch");
    Id1(tree);
    match1(TokenType::TK_ASSIGN, tree);
    Expr1(tree);
    tree->endChildren();
}

// to parse a variable decl
void VarDecl1(Tree* tree) {
    
    tree->addNode("VarDecl", "branch");
    TokenType type = tokens1[Index].type;
    match1(type, tree);
    Id1(tree);
    tree->endChildren();
}

// to parse a While Statement
void WhileStatement1(Tree* tree) {
    

    tree->addNode("WhileStatement", "branch");
    match1(TokenType::TK_WHILE, tree);

    BooleanExpr1(tree);
    Block1(tree);

    tree->endChildren();
}

// to parse and IF
void IfStatement1(Tree* tree) {
    
    tree->addNode("IfStatement", "branch");
    match1(TokenType::TK_IF, tree);
    BooleanExpr1(tree);
    Block1(tree);
    tree->endChildren();
}

// to Parse Expr
void Expr1(Tree* tree) {
   
    
    if (tokens1[Index].type == TokenType::TK_DIGIT) {
        IntExpr1(tree);
    } else if (tokens1[Index].type == TokenType::TK_QUOTE) {
        StringExpr1(tree);
    } else if (tokens1[Index].type == TokenType::TK_OPEN_PAREN
                || tokens1[Index].type == TokenType::TK_TRUE 
                    || tokens1[Index].type == TokenType::TK_FALSE) {
        BooleanExpr1(tree);
    } else if (tokens1[Index].type == TokenType::TK_ID){
        Id1(tree);
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToString1(tokens1[Index].type) << " Expected this token : digit || quote || ( || true || false || Id " ;
        cout << "\nAt index "<< tokens1[Index].line << " : " << tokens1[Index].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
    
}

// to parse IntExpr
void IntExpr1(Tree* tree) {
    
    

    Digit1(tree);
    if(tokens1[Index].type != TokenType::TK_PLUS){
        return; //Empty Production
    } else {
        IntOp1(tree);
        Expr1(tree);
    }
    
}

// To parse Strings
void StringExpr1(Tree* tree) {
   
    match1(TokenType::TK_QUOTE, tree);
    CharList1(tree);
    match1(TokenType::TK_QUOTE, tree);
    
}

// to match Bools
void BooleanExpr1(Tree* tree) {


    if(tokens1[Index].type != TokenType::TK_OPEN_PAREN){
        BoolVal1(tree);
    } else {
        match1(TokenType::TK_OPEN_PAREN, tree);
        Expr1(tree);
        BooleanOp1(tree);
        Expr1(tree);
        match1(TokenType::TK_CLOSE_PAREN, tree);
    }
   
}

// to match ID
void Id1(Tree* tree) {
    
    match1(TokenType::TK_ID, tree);
    
    
}

// definitly need it 
void CharList1(Tree* tree) {
    
    if (tokens1[Index].type == TokenType::TK_CHAR) {
        stringToPrint += tokens1[Index].lexeme;
        Char1(tree);
        CharList1(tree);
    } else if (tokens1[Index].type == TokenType::TK_SPACE){
        stringToPrint += tokens1[Index].lexeme;
        Space1(tree);
        CharList1(tree);
    } else if (tokens1[Index].type == TokenType::TK_QUOTE){
        stringToPrint += "\"";
        tree->addNode(stringToPrint, "leaf");
        stringToPrint = "\"";
        return; //Empty Production
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToString1(tokens1[Index].type) << " Expected this token : character || space || quote" ;
        cout << "\nAt index "<< tokens1[Index].line << " : " << tokens1[Index].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
        
}

//parse char
void Char1(Tree* tree){
    
    match1(TokenType::TK_CHAR,tree);
    
}

// to parse numbers NO MULTI DIGITS NO MORE
void Digit1(Tree* tree){
        match1(TokenType::TK_DIGIT, tree);
     
}

// to parse a space 
void Space1(Tree* tree){
    match1(TokenType::TK_SPACE,tree);

}

// to parse a boolean operation
void BooleanOp1(Tree* tree){
    if(tokens1[Index].type == TokenType::TK_NOT_EQUAL 
        || tokens1[Index].type == TokenType::TK_BOOLOP ){
        match1(tokens1[Index].type, tree);
    }
    else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToString1(tokens1[Index].type) << " Expected this token : != || == " ;
        cout << "\nAt index "<< tokens1[Index].line << " : " << tokens1[Index].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
}

// to parse a boolean value
void BoolVal1(Tree* tree){
    if(tokens1[Index].type == TokenType::TK_TRUE 
        || tokens1[Index].type == TokenType::TK_FALSE ){
        match1(tokens1[Index].type, tree);
    } else {
        cout << "\nError";
        cout << "\nReceived this token: " << TokenTypeToString1(tokens1[Index].type) << " Expected this token : true || false" ;
        cout << "\nAt index "<< tokens1[Index].line << " : " << tokens1[Index].position <<"\n";
        throw runtime_error("\nPARSER: PARSE FAILED DUE TO INCORRECT TOKEN");
    }
}

// to parse and Int operation 

void IntOp1(Tree* tree){
    
    match1(TokenType::TK_PLUS, tree);
    
}