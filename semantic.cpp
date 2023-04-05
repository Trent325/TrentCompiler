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
unordered_map<int, std::unordered_map<std::string, string>> myHashTable;


// Forward declarations
void Scopes();


// main function to be called to do SA and produce ST
bool semantic(vector<Token> tokenStream){
    Tokens = tokenStream;
    Scopes();
    ScopePositions.pop_back();
    
    for (const auto& p : ScopePositions) {
        std::cout << "First value: " << p.first << ", second value: " << p.second << std::endl;
    }
     // Print out all the hash tables:
    for (auto& outerPair : myHashTable) {
        std::cout << "Hash table " << outerPair.first << ":\n";
        
    }
    
    return true;
}

//to record amount of scopes and break them up
void Scopes(){
    for (int i = 0; i < Tokens.size(); i++) {
        int endScope = 0;
        if(Tokens[i].type == TokenType::TK_OPEN_BRACE){
            scopes++;
            int beginScopeIndex = i;
            for (int j = beginScopeIndex+1; j < Tokens.size(); j++) {
                if(Tokens[j].type == TokenType::TK_OPEN_BRACE){
                    endScope++;
                }
                if(Tokens[j].type == TokenType::TK_CLOSE_BRACE && endScope == 0 ){
                    
                    pair<int, int> myPair = make_pair(beginScopeIndex, i);
                    ScopePositions.push_back(myPair);
                    
                    
                    beginScopeIndex = j;
                    //maybe add a new inner hashtable to the outter table here
                    unordered_map<std::string, string> scope1HashTable;
                    myHashTable[scopes] = scope1HashTable;
                }
                if(Tokens[j].type == TokenType::TK_CLOSE_BRACE && endScope != 0){
                    endScope--;
                }
            }

        }
    }
}

// add varDecl to hash map
void verifyVarDecl(){
    for (const auto& p : ScopePositions) {
        for(int i = p.first; i<p.second; i++){
            if(Tokens[i].type == TokenType::TK_I_TYPE){
                unordered_map<std::string, string> newInnerHashTable;
                newInnerHashTable["Integer"] = Tokens[i].lexeme;
            }
        }
        
    }
    
}

//to produce symbol table
void SymbolTable(){
    cout << "----------------------------------" << endl;
    cout << "ID___TYPE____SCOPE___LINE:POSITION" << endl;
    cout << "----------------------------------" << endl;

}