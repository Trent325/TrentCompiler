#ifndef codeGen_H
#define codeGen_H

#include <vector>
#include <stdexcept>
#include "cst.h"
#include "lexer.h" 


using namespace std;

void GenerateCode(Tree* tree);
void PrintCodes();
void resetCodeGen();
void AssignScope(const vector<pair<string, int>>& vec);

#endif // codeGen