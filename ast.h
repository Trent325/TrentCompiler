#ifndef AST_H
#define AST_H

#include <vector>
#include <stdexcept>

#include "cst.h"
#include "lexer.h" 

using namespace std;

// Forward declarations
Tree* ast(vector<Token> tokenStream);
void printScopes();

vector<tuple<int, int, int, int, int>> sortScopeLinesAndPositions();



#endif // AST_H
