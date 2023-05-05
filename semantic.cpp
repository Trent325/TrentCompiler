#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <tuple>

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
vector<tuple<string, string>> InitVector;


// Forward declarations
bool is_integer(const string& str);
string typeAssigner(string var);
bool isInIntExpr(string element);
bool isValidIntExpr(string element, string typePassed);
bool CreateMap(vector<tuple<int, int, int, int, int>>ScopePositions);
bool verifyScope(vector<tuple<int, int, int, int, int>> vector, int scope, int j);

bool FindSymbols(Tree* tree,vector<tuple<int, int, int, int, int>>ScopePositions){
    // get a vector of all the node names in the tree
    vector<string> elements = tree->getElements();
    //create a hash table for each scope 
    unordered_map<string, tuple<string, bool, bool>> scopeHashTable;
    TokenMember = tree->getElements1();
    /*
    for (int i = 0; i < TokenMember.size(); i++) {
        string name = get<0>(TokenMember[i]);
        int line = get<1>(TokenMember[i]);
        int position = get<2>(TokenMember[i]);
        cout << line << ":" << position << " " << name << endl;
        
    }*/
    // traverse through tree elements
    for (int i = 0; i < elements.size(); i++) {
        //cout <<"element: " << elements[i] << endl;
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
            int AssignmentLocation = i;
            
            string variableName = elements[i+1];
           
            //add assignmentStatement
            if (variableName != "ADD") {
                pair<string, tuple<string, bool, bool, int>> Symbol(elements[AssignmentLocation], make_tuple(elements[i], true, false,i));
                SymbolList.push_back(Symbol);
                auto& my_tuple = scopeHashTable[variableName];
                get<2>(my_tuple) = true;
            }
            if (variableName == "ADD") {
                i++;
                while(elements[i] == "ADD"){
                    i++;
                }
                variableName = elements[i];
                pair<string, tuple<string, bool, bool, int>> Symbol(elements[AssignmentLocation], make_tuple(elements[i], true, false,i));
                SymbolList.push_back(Symbol);
                auto it = scopeHashTable.find(variableName);
                if (it != scopeHashTable.end()) {
                   string symbolType = get<0>(it->second);
                   if(symbolType == "int"){
                    auto& my_tuple = scopeHashTable[variableName];
                    get<2>(my_tuple) = true;
                    int iterator = i+1;
                    while(isInIntExpr(elements[iterator])){
                        auto currentIntExpr = scopeHashTable.find(elements[iterator]);
                        if(currentIntExpr != scopeHashTable.end()){
                            string typeInExpr = get<0>(currentIntExpr->second);
                            if(isValidIntExpr(elements[iterator],typeInExpr)) {
                                iterator++;
                                if(typeInExpr == "int"){
                                    
                                }
                            }else {
                                string error =  "Error: Type Error " + elements[iterator] + " of type " + symbolType+ " was used in int Expr, Can only use variable or digits. ";
                                errors.push_back(error);
                                return false;
                            }
                        } else {
                            if(isInIntExpr(elements[iterator])){
                                iterator++; 
                            }else{
                                string error =  "Error: Type Error " + elements[iterator] + " of type " + symbolType+ " was used in int Expr, Can only use variable or digits. ";
                                errors.push_back(error);
                                return false;
                            }
                        }
                        //
                    }
                    
                    if(elements[iterator] == "true" || elements[iterator] == "true"){
                        string error =  "Error: Type Error " + elements[iterator] + " of type " + symbolType+ " was used in int Expr, Can only use variable or digits. ";
                        errors.push_back(error);
                        return false;
                    }else{
                        i = iterator-1;
                    }
            
                   } 
                }
            } else {
                auto it = scopeHashTable.find(variableName);
                if (it != scopeHashTable.end()) {
                    string symbolType = get<0>(it->second);
                    if(symbolType == "int"){
                        
                            if(!isdigit(elements[i+2][0])){
                                string error =  "Error: Initialized int " + variableName + " assigned wrong type of "+elements[i+1];
                                errors.push_back(error);
                                return false;
                            }

                        } else if(symbolType == "string"){
                            if(isdigit(elements[i+2][0])){
                            string error =  "Error: Initialized string " + variableName + " assigned wrong type of int";
                            errors.push_back(error);
                            return false;
                            } else if(elements[i+2] == "true" || elements[i+2] == "false"){
                                string error =  "Error: Initialized string " + variableName + " assigned wrong type of boolean";
                                errors.push_back(error);
                                return false;
                            }
                            for (const auto& item : SymbolList) {
                                if (item.first == elements[i+2]) {
                                    string type = get<0>(item.second);
                                    if(type == "int"){
                                        string error =  "Error: Initialized string " + elements[i+2] + " assigned wrong type of int";
                                        errors.push_back(error);
                                        return false;
                                    }
                                }
                            }
                        } else {
                            bool found = false;
                            for (auto& outerPair : myHashTable) {
                                auto innerIt = outerPair.second.find(variableName);
                                if (innerIt != outerPair.second.end()) {
                                    get<2>(innerIt->second) = true;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                string error =  "Error: Initialized variable " + variableName + " but not declared.";
                                errors.push_back(error);
                                return false;
                            }
                            //string error =  "Error: Type MisMatch variable " + variableName + " of type " + symbolType + " was assigned int ";
                            //errors.push_back(error);
                            //return false;
                        }
                    }
                }
            
                /*
               else {
                // Search in the outer scopes
                bool found = false;
                for (auto& outerPair : myHashTable) {
                    auto innerIt = outerPair.second.find(variableName);
                    if (innerIt != outerPair.second.end()) {
                        get<2>(innerIt->second) = true;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    string error =  "Error: Initialized variable " + variableName + " but not declared.";
                    errors.push_back(error);
                    return false;
                }
            } */
        } else if(elements[i] == "WhileStatement"){
            int iterator = i+1;
            int iteratorcounter = 0;
            while(elements[iterator] == "IF EQUALS" || elements[iterator] == "IF NOT EQUALS"){
                iterator++;
                iteratorcounter++;
            }

            // do while not a block
            
            while(elements[iterator+2] != "Block"){
                string typeOne = typeAssigner(elements[iterator]);
                string typeTwo = typeAssigner(elements[iterator+1]);
            
                if(typeOne != typeTwo){
                    //add error
                    string error =  "Error: Incorrect Type Comparision of " + elements[iterator] + " of type " + typeOne + " to "+ elements[iterator+1] + " of type "+ typeTwo;
                    errors.push_back(error);
                    return false;
                }
                iterator++;
                iterator++;
            }
            
            i = iterator+1;
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
    return true;
}
//to set a value in the hash map of boolean
void setBooleanValue(string key) {
    // iterate over the outer map
    for (auto& entry : ResultHashTable) {
        // check if the inner map has the key
        if (entry.second.find(key) != entry.second.end()) {
            // set the second boolean value of the tuple to true
            get<2>(entry.second[key]) = true;
        }
    }
}
//add to hashmap
bool CreateMap(vector<tuple<int, int, int, int, int>>ScopePositions){
    unordered_map<string, tuple<string, bool, bool>> InnerHashTable;
    for(int i = ScopePositions.size() - 1; i >= 0; i--){
        for(int j = SymbolList.size() - 1; j >= 0; j--){
            if(verifyScope(ScopePositions, i, get<3>(SymbolList[j].second))){
                 if(SymbolList[j].first == "AssignmentStatement" ){
                    }
                if(SymbolList[j].first != "AssignmentStatement"){
                    if (InnerHashTable.find(SymbolList[j].first) != InnerHashTable.end()) {
                        string error =  "Error: Variable Already Declared in this scope! variable name : " + SymbolList[j].first ;
                        errors.push_back(error);
                        return false;
                    }else{
                        InnerHashTable[SymbolList[j].first] = make_tuple(get<0>(SymbolList[j].second), get<1>(SymbolList[j].second), get<2>(SymbolList[j].second));
                        SymbolList.erase(SymbolList.begin() + j); // Remove the symbol from the symbol list
                    }
                } else {
                    if (InnerHashTable.find(get<0>(SymbolList[j].second)) != InnerHashTable.end()) {
                        setBooleanValue(get<0>(SymbolList[j].second));
                    }
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
    
        int Thisline = get<1>(TokenMember[j]);
        int Thisposition = get<2>(TokenMember[j]);
        int ScopeStartLn = get<1>(vector[scope]);
        int ScopeStartPo = get<2>(vector[scope]);
        int ScopeEndLn = get<3>(vector[scope]);
        int ScopeEndPo = get<4>(vector[scope]);
        
        if(get<0>(TokenMember[j]) == "AssignmentStatement" ){
            int Thisline = get<1>(TokenMember[j+1]);
            int Thisposition = get<2>(TokenMember[j+1]);
            /* for debugging
            cout << "Assignment: " << get<0>(TokenMember[j]) << endl; 
            cout << "Thisline: " << Thisline << endl;
            cout << "Thisposition: " << Thisposition << endl;
            cout << "ScopeStartLn: " << ScopeStartLn << endl;
            cout << "ScopeStartPo: " << ScopeStartPo << endl;
            cout << "ScopeEndLn: " << ScopeEndLn << endl;
            cout << "ScopeEndPo: " << ScopeEndPo << endl;
        */
            if(Thisline >= ScopeStartLn && Thisposition >= ScopeStartPo){
                if(Thisline <= ScopeEndLn ){
                    return true;
                }
            }
        }
        if(Thisline >= ScopeStartLn && Thisposition >= ScopeStartPo){
            if(Thisline <= ScopeEndLn ){
                return true;
            }

        }
    return false;
}
//to see if it is a valid int Expr
bool isValidIntExpr(string element, string typePassed){
    if(is_integer(element)){
        return true;
    } else if (element.length() == 1){
        string var = element;
        string type = typeAssigner(var);
        //cout << "THIS IS TYPE: " << type << endl;
        if(type == "int"){
            return true;
        }
        if(typePassed == "int"){
            return true;
        }
        return false;
    }
    return false;
}
//to see if it is in an intExpr
bool isInIntExpr(string element){
    if(element == "VarDecl" || element == "Block" || element == "AssignmentStatement"
         || element == "PrintStatement"  || element == "WhileStatement" || element == "IfStatement" 
            || element == "true" || element == "false"){
            return false;
    } else if(element == ""){
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
    } else {
        for (const auto& item : SymbolList) {
            if (item.first == var) {
                string type = get<0>(item.second);
                return type;
            }
        }
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
            tuple<string, string> targetTuple = make_tuple(symbolName, symbolType);
            bool found = false;
            for (auto& t : InitVector) {
                if (t == targetTuple) {
                    cout << "\tIs Init: " << "1" << endl;
                    found = true;
                    break;
                }
            }
            if(!found){
                cout << "\tIs Init: " << "0" << endl;
            }
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
                //check if Init in Scope
                string warning =  "Variable "+innerPair.first+" of type "+ get<0>(tupleValue) +" is declared but not initalized";
                warnings.push_back(warning);
                WarningCount++;
            } else {
                get<2>(tupleValue) = true;
                myHashTable[outerPair.first][innerPair.first] = tupleValue;
                InitVector.push_back(make_tuple(innerPair.first,get<0>(tupleValue)));
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
    int scopes = 0;
    errors.clear();
    warnings.clear();
    ScopeLoc.clear();
    myHashTable.clear();
    ResultHashTable.clear();
    SymbolList.clear();
    TokenMember.clear();
    InitVector.clear();   
}