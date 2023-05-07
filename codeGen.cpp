#include <vector>
#include <stdexcept>
#include <tuple>
#include <algorithm>
#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "codeGen.h" 

using namespace std;
//create a vector of OP code to print
string OpCodes[256];
//create a vector for Temp Memory Locations
vector<tuple<string, string, string>> TmemLoc;
//           varname|T Index|value
//create a string for TempValues
string Tmem = "T";
//create an index for Opcodes
int OpIndex = 0;

//forward decl
void AssignTempLocs();

void GenerateCode(Tree* tree){
    //fill Op codes
    for (int i = 0; i < 256; i++) {
        OpCodes[i] = "00";
    }
    // get a vector of all the node names in the tree
    vector<string> elements = tree->getElements();
    //create an index for temp values
    int memIndex = 0;
    for (int i = 0; i < elements.size(); i++) {
       cout <<"element: " << elements[i] << endl;

       if(elements[i] == "Block"){
            
       } else if(elements[i] == "VarDecl"){
            OpCodes[OpIndex] = "A9";
            OpIndex++;
            OpCodes[OpIndex] = "00";
            OpIndex++;
       } else if(elements[i] == "AssignmentStatement"){
            if(elements[i+1] == "ADD"){
                int TotalAdds = 0;
                i++;
                while(elements[i] == "ADD"){
                    TotalAdds++;
                    i++;
                }
            }
            OpCodes[OpIndex] = "8D";
            OpIndex++;
            string TempMindex = Tmem + to_string(memIndex);
            TmemLoc.push_back(make_tuple(elements[i],TempMindex,elements[i+2]));
            memIndex++;
            OpCodes[OpIndex] = TempMindex;
            OpIndex++;
            OpIndex++;
       } else {
        //default error case
       } 
    }
    OpCodes[OpIndex] = "00";
    OpIndex++;
    AssignTempLocs();
}
void AssignTempLocs(){
    for (int i = 0; i < 256; i++) {
        char Fchar = OpCodes[i][0];
        if(Fchar == 'T'){
            for (auto t : TmemLoc) {
                if (get<1>(t) == OpCodes[i]) {
                    stringstream ss;
                    ss << hex << OpIndex;
                    string hex_str = ss.str();
                    if (hex_str.size() == 1) {
                        hex_str = "0" + hex_str;
                    }
                    OpCodes[i] = hex_str;
                    stringstream sv;
                    sv << hex << get<2>(t);
                    string hex_value = sv.str();
                    if (hex_value.size() == 1) {
                        hex_value = "0" + hex_value;
                    }
                    OpCodes[OpIndex] = hex_value;
                }
            }
        }
    }
}
void PrintCodes() {
    int NewLine = 0;
    for (int i = 0; i < 256; i++) {
        cout << OpCodes[i] << " ";
        NewLine++;
        if(NewLine == 16){
            cout << endl;
            NewLine = 0;
        }
    }
}
void resetCodeGen(){
    for (int i = 0; i < 256; i++) {
        OpCodes[i] = "00";
    }
    TmemLoc.clear();
    OpIndex = 0;

}