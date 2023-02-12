#include <iostream>
#include "lexer.cpp" //imports the lexer from the file lexer
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
//I wrote 
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
    while (getline(file, line)) {
        if(ProgramCount != ProgramCountPrev){
            cout << "INFO LEXER STARTED ON PROGRAM " << ProgramCount << endl;
            ProgramCountPrev = ProgramCount;
        }
        lexer(line, lineCount);
        if (line.find("$") != string::npos) {
            cout << "INFO LEXER COMPLETED WITH IDK YET" <<"ERRORS" << endl;
            ProgramCount++;
        }
        lineCount++;
        
    }

    return 0;
    }
