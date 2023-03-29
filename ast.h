#ifndef AST_H
#define AST_H

#include <vector>
#include <stdexcept>

#include "cst.h"
#include "lexer.h" 

using namespace std;

// Forward declarations
Tree* ast(vector<Token> tokenStream);


#endif // AST_H
