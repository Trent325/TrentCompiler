#include <vector>
#include <stdexcept>
#include <unordered_map>

#include "parser.h" 

using namespace std;

// Global variables
vector<Token> Tokens;
int scopes = 0;

//vector of scopes start and ends
vector<std::pair<int, int>> ScopePositions;
//creates a hash table for each scope that can have another hash table of variables
unordered_map<int, std::unordered_map<std::string, tuple<std::string, bool, bool>>> myHashTable;


// Forward declarations

bool TreeTraverse(Tree* tree){
    // get a vector of all the node names in the tree
    std::vector<std::string> elements = tree->getElements();
    //create a hash table for each scope 
    unordered_map<std::string, tuple<std::string, bool, bool>> scopeHashTable;

    /*cout << "TEST" << endl;
    // print the vector
    for (int i = 0; i < elements.size(); i++) {
        cout << elements[i] << " ";
    }
    cout << "\n TEST" << endl;
    */
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
            
        }
        else if(elements[i] == "VarDecl"){
            scopeHashTable[elements[i+1]] = make_tuple(elements[i+2], true, false);;
            i += 2;
        }
    }
    // add scope hash table to table
    myHashTable[scopes] = scopeHashTable;


    return true;

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
            tableStr += "\n Name: " + get<0>(tupleValue) 
                        + ", Type: " + innerPair.first 
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

