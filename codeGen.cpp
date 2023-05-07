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
vector<tuple<string, string,string>> TmemLoc;
//           varname|T Index|value
//create a string for TempValues
string Tmem = "T";
//create an index for Opcodes
int OpIndex = 0;
string accumlator = "01";

//forward decl
void AssignTempLocs();
string stringToHex(string nonHex);
string intToHex(int index);
bool is_int(const string& str);
string searchForVarLoc(string searchString);
string searchForVarVal(string searchTerm);
void AssignVarVal(string searchTerm, string value);

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
            if(accumlator == "00"){
                //do nothing dont need to load it 
            } else {
                accumlator = "00";
                OpCodes[OpIndex] = "A9";
                OpIndex++;
                OpCodes[OpIndex] = "00";
                OpIndex++;
                OpCodes[OpIndex] = "8D";
                OpIndex++;
                string TempMindex = Tmem + to_string(memIndex);
                TmemLoc.push_back(make_tuple(elements[i+2],TempMindex,"00"));
                memIndex++;
                OpCodes[OpIndex] = TempMindex;
                OpIndex++;
                OpIndex++;
            }
            
       } else if(elements[i] == "AssignmentStatement"){
            if(elements[i+1] == "ADD"){
                int TotalAdds = 0;
                i++;
                while(elements[i] == "ADD"){
                    TotalAdds++;
                    i++;
                }
            }
            
            
            if(!is_int(elements[i+2])){
                OpCodes[OpIndex] = "8D";
                OpIndex++;
                string memLoc = stringToHex(searchForVarLoc(elements[i+2]));
                OpCodes[OpIndex] = memLoc;
                OpIndex++;
            } else {
                OpCodes[OpIndex] = "A9";
                OpIndex++;
                OpCodes[OpIndex] = stringToHex(elements[i+2]);
                OpIndex++;
                AssignVarVal(elements[i+1],elements[i+2]);
            }
            
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
                cout << get<1>(t) << " H" << endl;
                if (get<1>(t) == OpCodes[i]) {
                    string hex_str = intToHex(OpIndex);
                    OpCodes[i] = hex_str;
                    string hex_value = stringToHex(get<2>(t));
                    OpCodes[OpIndex] = hex_value;
                }
            }
        }
    }
}
string searchForVarLoc(string searchString) {
    string result = "";
    for (auto tuple : TmemLoc) {
        if (get<0>(tuple) == searchString) {
            result = get<1>(tuple);
            break;
        }
    }
    return result;
}
void AssignVarVal(string searchTerm, string value) {
    for (auto &tuple : TmemLoc) {
        if (get<0>(tuple) == searchTerm) {
            get<2>(tuple) = value;
        }
    }
}
string searchForVarVal(string searchTerm) {
    for (auto const &tuple : TmemLoc) {
        if (get<0>(tuple) == searchTerm) {
            return get<2>(tuple);
        }
    }
    return "";
}
string stringToHex(string nonHex){
    stringstream ss;
    ss << hex << uppercase << nonHex;
    string hex_str = ss.str();
    if (hex_str.size() == 1) {
        hex_str = "0" + hex_str;
    }
    return hex_str;
}
string intToHex(int index){
    stringstream ss;
    ss << hex << uppercase << index;
    string hex_str = ss.str();
    if (hex_str.size() == 1) {
        hex_str = "0" + hex_str;
    }
    return hex_str;
}
bool is_int(const string& str) {
    if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) {
        return false;
    }
    char* endptr = nullptr;
    strtol(str.c_str(), &endptr, 10);
    return (*endptr == '\0');
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