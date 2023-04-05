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


// Forward declarations
bool is_integer(const std::string& str);
string typeAssigner(string var);
bool isInIntExpr(string element);
bool isValidIntExpr(string element);

bool TreeTraverse(Tree* tree){
    // get a vector of all the node names in the tree
    vector<string> elements = tree->getElements();
    //create a hash table for each scope 
    unordered_map<string, tuple<string, bool, bool>> scopeHashTable;

    std::vector<std::tuple<std::string, int, int>> elements1 = tree->getElements1();

    for (int i = 0; i < elements1.size(); i++) {
        std::string name = std::get<0>(elements1[i]);
        int line = std::get<1>(elements1[i]);
        int position = std::get<2>(elements1[i]);
        std::cout << line << ":" << position << " " << name << std::endl;
        
    }
    
    cout << "\n" << endl;
    
    // traverse through tree elements
    for (int i = 0; i < elements.size(); i++) {
        //find the amount of scopes
        if(elements[i] == "Block" && scopes == 0){
            scopes++;
        }
        //once a scope is done add it to the hash mape
        else if(elements[i] == "Block" && scopes != 0){
            myHashTable[scopes] = scopeHashTable;
            scopes++;
            //create a hash table for this scope 
            scopeHashTable.clear();
            
        } else if(elements[i] == "VarDecl"){
            string variableName = elements[i+2];
             // Check if the variable name already exists in the current scope
            if (scopeHashTable.find(variableName) != scopeHashTable.end()) {
                // Variable name already exists in the current scope
                string error =  "Error: variable name " + variableName + " already exists in the current scope.";
                errors.push_back(error);
                return false;
            }
            scopeHashTable[elements[i+2]] = make_tuple(elements[i+1], true, false);
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
                    cout << "\n got here"<< elements[iterator] << endl;
                    while(isInIntExpr(elements[iterator])){
                        if(isValidIntExpr(elements[iterator])){
                            cout << "\n got in while here"<< endl;
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
                get<1>(it->second) = true; 
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

    return true;

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
bool is_integer(const std::string& str) {
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
    // Create a vector to store the output strings
    vector<string> output;

    // Iterate over the outer hash table
    for (auto& outerPair : myHashTable) {
        string tableStr = "Scope " + to_string(outerPair.first) + ":";
        // Iterate over the inner hash table
        for (auto& innerPair : outerPair.second) {
            // Add the inner hash table key and value to the table string
            auto tupleValue = innerPair.second;
            //convert booleans to their values 
            string IsDeclared = get<1>(tupleValue) ? "true" : "false";
            string IsInit = get<2>(tupleValue) ? "true" : "false";
            tableStr += "\n Type: " + get<0>(tupleValue) 
                        + ", Name: " + innerPair.first 
                        + ", Is Declared : " + IsDeclared 
                        + ", Is Initialized : " + IsInit;
        }
        // Add the outer and inner hash table elements to the output vector
        output.push_back(tableStr);
    }

    // Iterate over the output vector in reverse order
    for (auto it = output.rbegin(); it != output.rend(); ++it) {
        // Print each string to the console
        cout << *it << "\n";
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
                string warning =  "Variable is declared but not initalized";
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


