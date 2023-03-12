#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stdexcept>

#include "cst.h"
#include "lexer.h" 

using namespace std;

// Forward declarations
Tree* parse(vector<Token> tokenStream);


#endif // PARSER_H
