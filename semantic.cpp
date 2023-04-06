#include <vector>
#include <stdexcept>
#include <unordered_map>

#include "parser.h" 
#include "ast.h" 

using namespace std;

// Global variables
int scopes = 0;
vector<string> errors;
vector<string> warnings;
//vector of scopes start and ends
vector<pair<int, int>> ScopeLoc;
//creates a hash table for each scope that can have another hash table of variables
unordered_map<int, unordered_map<string, tuple<string, bool, bool>>> myHashTable;

unordered_map<int, unordered_map<string, tuple<string, bool, bool>>> ResultHashTable;

vector<pair<string, tuple<string, bool, bool, int>>> SymbolList;
vector<tuple<string, int, int>> TokenMember;


// Forward declarations
bool is_integer(const string& str);
string typeAssigner(string var);
bool isInIntExpr(string element);
bool isValidIntExpr(string element);
bool CreateMap(vector<tuple<int, int, int, int, int>>ScopePositions);
bool verifyScope(vector<tuple<int, int, int, int, int>> vector, int scope, int j);

bool FindSymbols(Tree* tree,vector<tuple<int, int, int, int, int>>ScopePositions){
    // get a vector of all the node names in the tree
    vector<string> elements = tree->getElements();
    //create a hash table for each scope 
    
    unordered_map<string, tuple<string, bool, bool>> scopeHashTable;

    TokenMember = tree->getElements1();
    /* Prints out AST
    for (int i = 0; i < TokenMember.size(); i++) {
        string name = get<0>(TokenMember[i]);
        int line = get<1>(TokenMember[i]);
        int position = get<2>(TokenMember[i]);
        cout << line << ":" << position << " " << name << endl;
        
    }*/
   
    cout << "\n" << endl;
    
    // traverse through tree elements
    for (int i = 0; i < elements.size(); i++) {
        //find the amount of scopes
        if(elements[i] == "Block" && scopes == 0){
            scopes++;
        }
        //once a scope is done add it to the hash mape
        if(elements[i] == "Block" && scopes != 0){
            myHashTable[scopes] = scopeHashTable;
            scopes++;
            //create a hash table for this scope 
            scopeHashTable.clear();
            
        } else if(elements[i] == "VarDecl"){
            string variableName = elements[i+2];
            scopeHashTable[elements[i+2]] = make_tuple(elements[i+1], true, false);
            pair<string, tuple<string, bool, bool, int>> Symbol(elements[i+2], make_tuple(elements[i+1], true, false,i+2));
            SymbolList.push_back(Symbol);
            i += 2;
        } else if(elements[i] == "AssignmentStatement"){
            string variableName = elements[i+1];
            if (variableName.length() > 1) {
                variableName = elements[i+2];
                auto it = scopeHashTable.find(variableName);
                if (it != scopeHashTable.end()) {
                   string type = get<0>(it->second);
                   if(type == "int"){
                    variableName = elements[i+2];
                    int iterator = i+2;
                    while(isInIntExpr(elements[iterator])){
                        if(isValidIntExpr(elements[iterator])){
                            iterator++;
                        }else{
                            string error =  "Error: Type Error " + elements[iterator] + " of type " + type + " was used in int Expr, Can only use variable or digits. ";
                            errors.push_back(error);
                            return false;
                        }
                    }
                    // Change the value of the last boolean in the tuple
                    auto& my_tuple = scopeHashTable[elements[i+2]];
                    get<2>(my_tuple) = true;
                   } else {
                        string error =  "Error: Type MisMatch variable " + variableName + " of type " + type + " was assigned int ";
                        errors.push_back(error);
                        return false;
                   }
                }
            } 
            auto it = scopeHashTable.find(variableName);
            if (it != scopeHashTable.end()) {
                get<2>(it->second) = true; 
            } else {
                string error =  "Error: Initialized varaible " + variableName + " but not declared.";
                errors.push_back(error);
                return false;
            }
        } else if(elements[i] == "IF EQUALS" || elements[i] == "IF NOT EQUALS" ){
            string str = elements[i+1];
            if(is_integer(elements[i+1])){
                if(is_integer(elements[i+2])){
                    // do nothing this is correct
                } else {
                    //varOne
                    string varOne = elements[i+1];
                    string typeOne = typeAssigner(varOne);
                    //varTwo
                    string varTwo = elements[i+2];
                    string typeTwo = typeAssigner(varTwo);
                    //add error
                    string error =  "Error: Incorrect Type Comparision of " + varOne + " of type " + typeOne + " to "+ varTwo + " of type "+ typeTwo;
                    errors.push_back(error);
                    return false;
                }
            } else if(str.length() == 1){
                //varOne
                string varOne = elements[i+1];
                auto it = scopeHashTable.find(varOne);
                string typeOne = get<0>(it->second);
                //varTwo
                string varTwo = elements[i+2];
                auto two = scopeHashTable.find(varTwo);
                string typeTwo = get<0>(two->second);
                if(typeOne == typeTwo){
                    // do nothing it is correct
                } else {
                    string error =  "Error: Incorrect Type Comparision of " + varOne + " of type " + typeOne + " to "+ varTwo + " of type "+ typeTwo;
                    errors.push_back(error);
                    return false;
                }
            } else if(str == "true" || str == "false"){
                if(elements[i+2]== "true" || elements[i+2] == "false"){
                    // do nothing its correct
                } else {
                    //varOne
                    string varOne = elements[i+1];
                    string typeOne = typeAssigner(varOne);
                    //varTwo
                    string varTwo = elements[i+2];
                    string typeTwo = typeAssigner(varTwo);
                    //add error
                    string error =  "Error: Incorrect Type Comparision of " + varOne + " of type " + typeOne + " to "+ varTwo + " of type "+ typeTwo;
                    errors.push_back(error);
                    return false;
                }
            } else if (str[0] == '"'){
                if(elements[i+2][0]== '"'){
                    // do nothing its correct
                } else {
                    //varOne
                    string varOne = elements[i+1];
                    string typeOne = typeAssigner(varOne);
                    //varTwo
                    string varTwo = elements[i+2];
                    string typeTwo = typeAssigner(varTwo);
                    //add error
                    string error =  "Error: Incorrect Type Comparision of " + varOne + " of type " + typeOne + " to "+ varTwo + " of type "+ typeTwo;
                    errors.push_back(error);
                    return false;
                }
            }
        }
    }
    // add scope hash table to table
    myHashTable[scopes] = scopeHashTable;
    if(CreateMap(ScopePositions) != true){
        return false;
    }
    //add a function to take scopes in and token stream and decide if they are init in the token stream 

    return true;

}
// check if init
void initSetter(vector<tuple<int, int, int, int, int>>ScopePositions){
    //for each scope
    for(int i = ScopePositions.size() - 1; i >= 0; i--){
        //for each token
        for(int j = TokenMember.size() - 1; j >= 0; j--){
            if(get<0>(TokenMember[j]) == "AssignmentStatement"){
                if(verifyScope(ScopePositions, i, j)){
                    //if it is assignment statement and a part of this scope than check if the variable was init, if it was nothing
                    //if it was not init the variable
                }
            }
        }
    }
}

//add to hashmap
bool CreateMap(vector<tuple<int, int, int, int, int>>ScopePositions){
    unordered_map<string, tuple<string, bool, bool>> InnerHashTable;
    for(int i = ScopePositions.size() - 1; i >= 0; i--){
        for(int j = SymbolList.size() - 1; j >= 0; j--){
            if(verifyScope(ScopePositions, i, get<3>(SymbolList[j].second))){
                if (InnerHashTable.find(SymbolList[j].first) != InnerHashTable.end()) {
                    string error =  "Error: Variable Already Declared in this scope! variable name : " + SymbolList[j].first ;
                    errors.push_back(error);
                    return false;
                }else{
                    InnerHashTable[SymbolList[j].first] = make_tuple(get<0>(SymbolList[j].second), get<1>(SymbolList[j].second), get<2>(SymbolList[j].second));
                    SymbolList.erase(SymbolList.begin() + j); // Remove the symbol from the symbol list
                }
            }
        }
        ResultHashTable[i+1] = InnerHashTable;
        InnerHashTable.clear();
    }
    return true;
}
// to check the scope
bool verifyScope(vector<tuple<int, int, int, int, int>> vector, int scope, int j){
        int ScopeFound;
    
        int Thisline = get<1>(TokenMember[j]);
        int Thisposition = get<2>(TokenMember[j]);
        int ScopeStartLn = get<1>(vector[scope]);
        int ScopeStartPo = get<2>(vector[scope]);
        int ScopeEndLn = get<3>(vector[scope]);
        int ScopeEndPo = get<4>(vector[scope]);
        if(Thisline >= ScopeStartLn && Thisposition > ScopeStartPo){
            if(Thisline <= ScopeEndLn ){
                return true;
            }
        }
    return false;

}

//to see if it is a valid int Expr
bool isValidIntExpr(string element){
    if(is_integer(element)){
        return true;
    } else if (element.length() == 1){
        string var = element;
        string type = typeAssigner(var);
        if(type == "int"){
            return true;
        }
        return false;
    }
    return false;
}
//to see if it is in an intExpr
bool isInIntExpr(string element){
    if(element == "VarDecl" || element == "Block" || element == "AssignmentStatement"
         || element == "PrintStatement"  || element == "WhileStatement" || element == "IfStatement"){
            return false;
         }
         return true;
}

//type assigner for errors
string typeAssigner(string var){
    if(is_integer(var)){
        return "int";
    }else if(var== "true" || var == "false"){
        return "boolean";
    }else if(var[0] == '"'){
        return "string";
    }
}

//check if a string is an integer
bool is_integer(const string& str) {
    if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) {
        return false;
    }
    char* endptr = nullptr;
    strtol(str.c_str(), &endptr, 10);
    return (*endptr == '\0');
}

//to print out errors 
void PrintErrors(){
    for (string s : errors) {
        cout << "\n" << s ;
    }
}

//count errors
int CountErrors(){
    int totalerrors = 0;
    for (string s : errors) {
        totalerrors++;
    }
    return totalerrors;
}

//to produce symbol table
void SymbolTable() {
    cout << "----------------------------------" << endl;
    // Print out all the hash tables in reverse order
    for (const auto& outerPair : ResultHashTable) {
        cout << "Scope: " << outerPair.first << endl;
        for (const auto& innerPair : outerPair.second) {
            string symbolName = innerPair.first;
            string symbolType = get<0>(innerPair.second);
            bool isConst = (get<1>(innerPair.second) ? true : false);
            bool isinit = (get<2>(innerPair.second) ? true : false);
            cout << "\tSymbol Name: " << symbolName << endl;
            cout << "\tSymbol Type: " << symbolType << endl;
            cout << "\tIs Declared: " << isConst << endl;
            cout << "\tIs Init: " << isinit << endl;
            cout << "\n " << endl;
        }
    }
}
//to print out warnings 
void PrintWarnings(){
    for (string s : warnings) {
        cout << "\n" << s ;
    }
}

//count warnings 
int CountWarnings(){
    int WarningCount = 0;

    // Iterate over the outer hash table
    for (auto& outerPair : myHashTable) {
        for (auto& innerPair : outerPair.second) {
            auto tupleValue = innerPair.second;
            // Check if either IsDeclared or IsInit is false
            if (!get<2>(tupleValue)) {
                string warning =  "Variable "+innerPair.first+" is declared but not initalized";
                warnings.push_back(warning);
                WarningCount++;
            }
        }
    }
    return WarningCount;

}

//to clear global variables 
void clearSemantics(){
    scopes = 0;
    errors.clear();
    myHashTable.clear();

}


