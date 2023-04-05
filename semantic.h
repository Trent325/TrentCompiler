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
void PrintWarnings();
int CountWarnings();
int CountErrors();
void PrintErrors();
void clearSemantics();


#endif // PARSER_H