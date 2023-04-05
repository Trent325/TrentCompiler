#ifndef SEM_H
#define SEM_H

#include <vector>
#include <stdexcept>

#include "cst.h"
#include "lexer.h" 

using namespace std;

// Forward declarations
bool TreeTraverse(Tree* tree);
void SymbolTable();
int PrintWarnings();
int PrintErrors();


#endif // PARSER_H