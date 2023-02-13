#include <iostream>
#include "lexer.cpp" //imports the lexer from the file lexer
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
//Function ChatGPT created to add two vectors together
std::vector<Token> addVectors(std::vector<Token> &a, std::vector<Token> &b) {
    std::vector<Token> result;

    result.reserve(a.size() + b.size());
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());

    return result;
}
//Function I created to track errors of the program
int countErrors(const std::vector<Token> tokens) {
  int error_count = 0;
  for (const auto& token : tokens) {
    if (token.type == TK_ERROR) {
      ++error_count;
    }
  }
  return error_count;
}
//I wrote all of this file
int main() {
    
    string filename;
    cout << "Enter the name of the text file: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: unable to open file" << endl;
        return 1;
    }

    int lineCount = 1;
    int ProgramCount = 1;
    int ProgramCountPrev = 0;
    string line;
    std::vector<Token> UNlexedTokens;
    std::vector<Token> lexedTokens;
    while (getline(file, line)) {
        if(ProgramCount != ProgramCountPrev){
            cout << "INFO LEXER STARTED ON PROGRAM " << ProgramCount << endl;
            ProgramCountPrev = ProgramCount;
        }
        std::vector<Token> temp = lexer(line, lineCount);
        std::vector<Token> ProgramTokens = addVectors(ProgramTokens, temp);
        UNlexedTokens = ProgramTokens;
        if (line.find("$") != string::npos) {
            int errors = countErrors(ProgramTokens);
            cout << "INFO LEXER COMPLETED WITH " << errors <<" ERRORS"<< endl;
            ProgramCount++;
            lexedTokens = addVectors(lexedTokens, ProgramTokens);
        }
        lineCount++;
        
    }
    Token last_element = UNlexedTokens[UNlexedTokens.size() - 1];
    if (last_element.value != "$"){
        std::cout << "INFO WARNING NO EOF or $ FOUND TO TERMINATE PROGRAM" <<  std::endl;
    }
    
    return 0;
    }



