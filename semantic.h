#ifndef SEM_H
#define SEM_H

#include <vector>
#include <stdexcept>

#include "cst.h"
#include "lexer.h" 

using namespace std;

// Forward declarations
bool FindSymbols(Tree* tree,vector<tuple<int, int, int, int, int>>ScopePositions);
void SymbolTable();
void PrintWarnings();
int CountWarnings();
int CountErrors();
void PrintErrors();
void clearSemantics();
bool verifyScope(vector<tuple<int, int, int, int, int>> vector);
vector<tuple<string, string, string>> SymbolTableForCodeGen();



#endif // PARSER_H