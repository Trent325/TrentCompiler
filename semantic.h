#ifndef SEM_H
#define SEM_H

#include <vector>
#include <stdexcept>

#include "cst.h"
#include "lexer.h" 

using namespace std;

// Forward declarations
bool semantic(vector<Token> tokenStream);
void SymbolTable();


#endif // PARSER_H