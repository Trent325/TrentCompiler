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
vector<tuple<string, string,string,string>> TmemLoc;
//           varname|T Index|value|Tyoe
//create a string for TempValues
string Tmem = "T";
//create an index for Opcodes
int OpIndex = 0;
int TotalAdds = 0;
string accumlator = "01";
bool accumlatorLoadFlag = true;
vector<tuple<string, string, int>> Tnums;
vector<string> elements;
//make an int to track string loc in heap (start of heap for strings)
int SLocHeap = 234;

//forward decl
void AssignTempLocs();
string stringToHex(string nonHex);
string intToHex(int index);
bool is_int(const string& str);
string searchForVarLoc(string searchString);
string searchForVarVal(string searchTerm);
void AssignVarVal(string searchTerm, string value);
void intAdd(int i);
string searchForVarType(string searchString);
vector<string> writeStringToHeap(const string& input);
void StringAssignment(int i);
void backpatch();

void GenerateCode(Tree* tree){
    //fill Op codes
    for (int i = 0; i < 256; i++) {
        OpCodes[i] = "00";
    }
    // get a vector of all the node names in the tree
    elements = tree->getElements();
    //create an index for temp values
    int memIndex = 0;
    for (int i = 0; i < elements.size(); i++) {
       cout <<"element: " << elements[i] << endl;

       if(elements[i] == "Block"){
            
       } else if(elements[i] == "VarDecl"){
            if(accumlator == "00"){
                //just store it in memory
                OpCodes[OpIndex] = "8D";
                OpIndex++;
                string TempMindex = Tmem + to_string(memIndex);
                TmemLoc.push_back(make_tuple(elements[i+2], TempMindex,"00", elements[i+1]));
                memIndex++;
                OpCodes[OpIndex] = TempMindex;
                OpIndex++;
                OpIndex++;
                i+= 2;
            } else {
                accumlator = "00";
                accumlatorLoadFlag = false;
                OpCodes[OpIndex] = "A9";
                OpIndex++;
                OpCodes[OpIndex] = "00";
                OpIndex++;
                OpCodes[OpIndex] = "8D";
                OpIndex++;
                string TempMindex = Tmem + to_string(memIndex);
                TmemLoc.push_back(make_tuple(elements[i+2], TempMindex,"00", elements[i+1]));
                memIndex++;
                OpCodes[OpIndex] = TempMindex;
                OpIndex++;
                OpIndex++;
            }
            
       } else if(elements[i] == "AssignmentStatement"){
            string type = searchForVarType(elements[i+1]);
            if(elements[i+1] == "ADD"){
                
                while(elements[i+1] == "ADD"){
                    TotalAdds++;
                    i++;
                }
                intAdd(i);
                i += TotalAdds + 2;
                TotalAdds = 0;
            } else if(type == "string"){
                StringAssignment(i);
                i+=2;
            }else if(type == "boolean"){
                cout << "in boolean" << endl;
                if(elements[i+2] == "false"){
                    OpCodes[OpIndex] = "A9";
                    OpIndex++;
                    OpCodes[OpIndex] = "F4";
                    OpIndex++;
                    OpCodes[OpIndex] = "8D";
                    OpIndex++;
                    string mem = searchForVarLoc(elements[i+1]);
                    string memLoc = stringToHex(mem);
                    OpCodes[OpIndex] = memLoc;
                    OpIndex++;
                    OpIndex++;
                } else if(elements[i+2] == "true"){
                    OpCodes[OpIndex] = "A9";
                    OpIndex++;
                    OpCodes[OpIndex] = "FA";
                    OpIndex++;
                    OpCodes[OpIndex] = "8D";
                    OpIndex++;
                    string mem = searchForVarLoc(elements[i+1]);
                    string memLoc = stringToHex(mem);
                    OpCodes[OpIndex] = memLoc;
                    OpIndex++;
                    OpIndex++;
                }
            } else if(!is_int(elements[i+2])){
                OpCodes[OpIndex] = "AD";
                OpIndex++;
                //now search for Variable Index load that into next one 
                //TODO search and load from memory
                string MLOC = searchForVarLoc(elements[i+2]);
                OpCodes[OpIndex] = MLOC;
                OpIndex++;
                OpCodes[OpIndex] = "00";
                OpIndex++;
                OpCodes[OpIndex] = "8D";
                OpIndex++;
                string mem = searchForVarLoc(elements[i+1]);
                string memLoc = stringToHex(mem);
                OpCodes[OpIndex] = memLoc;
                OpIndex++;
                OpIndex++;
            } else {
                OpCodes[OpIndex] = "A9";
                OpIndex++;
                OpCodes[OpIndex] = stringToHex(elements[i+2]);
                OpIndex++;
                AssignVarVal(elements[i+1],elements[i+2]);
                OpCodes[OpIndex] = "8D";
                OpIndex++;
                string loc = searchForVarLoc(elements[i+1]);
                string memLoc = stringToHex(loc);
                OpCodes[OpIndex] = memLoc;
                OpIndex++;
                OpIndex++;
            }
            
       } else {
        //default error case
       } 
    }
    OpCodes[OpIndex] = "00";
    OpIndex++; OpIndex++;
    AssignTempLocs();
    backpatch();
}
void backpatch(){
    OpCodes[244] = "66";
    OpCodes[245] = "61";
    OpCodes[246] = "6C";
    OpCodes[247] = "73";
    OpCodes[248] = "65";
    OpCodes[249] = "00";
    OpCodes[250] = "74";
    OpCodes[251] = "72";
    OpCodes[252] = "75";
    OpCodes[253] = "65";
    OpCodes[254] = "00";
}
void StringAssignment(int i){
    OpCodes[OpIndex] = "A9";
    OpIndex++;
    vector<string> HeapString = writeStringToHeap(elements[i+2]);
    SLocHeap += HeapString.size();
    string heapLoc = intToHex(SLocHeap);
    for(int i = HeapString.size()-1; i>=0;i--){
        OpCodes[SLocHeap] = HeapString[i];
        SLocHeap--;
    }
    OpCodes[OpIndex] = heapLoc;
    OpIndex++;
    OpCodes[OpIndex] = "8D";
    OpIndex++;
    string mem = searchForVarLoc(elements[i+1]);
    string memLoc = stringToHex(mem);
    OpCodes[OpIndex] = memLoc;
    OpIndex++;
    OpIndex++;
}
vector<string> writeStringToHeap(const string& input) {
    vector<string> result;
    ostringstream hex_stream;
    hex_stream << hex << setfill('0') << uppercase << noskipws;
    
    for (char c : input) {
        
        hex_stream << setw(2) << static_cast<int>(c);
        if(hex_stream.str() != "22"){
            result.push_back(hex_stream.str());
            hex_stream.str(""); // clear the stream for the next character
        }
        hex_stream.str("");
        
    }
    
    return result;
}
void intAdd(int i){
     bool isSaved = false;
                for(int j = TotalAdds+2; j>0; j--){
                    if(!is_int(elements[i+j])){
                        if(j+1 == 2){
                            OpCodes[OpIndex] = "6D";
                            OpIndex++;
                            OpCodes[OpIndex] = "FF";
                            OpIndex++;
                            OpIndex++;
                            OpCodes[OpIndex] = "8D";
                            OpIndex++;
                            string MLOC = searchForVarLoc(elements[j+i]);
                            OpCodes[OpIndex] = MLOC;
                            OpIndex++;
                            break;
                        }
                    } else {
                        OpCodes[OpIndex] = "A9";
                        OpIndex++;
                        string adder = stringToHex(elements[i+j]);
                        OpCodes[OpIndex] = adder;
                        OpIndex++;
                    }
                    if(!isSaved){
                        OpCodes[OpIndex] = "8D";
                        OpIndex++;
                        OpCodes[OpIndex] = "FF";
                        OpIndex++;
                        OpIndex++;
                        isSaved = true;
                    } else {
                        if(j-1 != 1){
                            OpCodes[OpIndex] = "6D";
                            OpIndex++;
                            OpCodes[OpIndex] = "FF";
                            OpIndex++;
                            OpIndex++;
                            OpCodes[OpIndex] = "8D";
                            OpIndex++;
                            OpCodes[OpIndex] = "FF";
                            OpIndex++;
                            OpIndex++;
                        } else {
                            // do nothing for now
                        }
                            
                    }
                }
}
void AssignTempLocs(){
    string TnumsFound;
    string hex_str;
    string hex_value;
    int opIndexer;
    for (int i = 0; i < 256; i++) {
        char Fchar = OpCodes[i][0];
        if(Fchar == 'T'){
            for (auto t : TmemLoc) {
                if (get<1>(t) == OpCodes[i]) {
                    bool found = false;
                    for (auto const &elem : Tnums) {
                        if (get<0>(elem) == OpCodes[i]) {
                            found = true;
                            TnumsFound = get<1>(elem);
                            opIndexer = get<2>(elem);
                            break;
                        }
                    }
                    if(found == true){
                        hex_str = TnumsFound;
                        hex_value = "00";
                        
                    } else {
                        hex_str = intToHex(OpIndex);
                        Tnums.push_back(make_tuple(OpCodes[i],hex_str,OpIndex));
                        hex_value = stringToHex(get<2>(t));
                        opIndexer = OpIndex;
                        
                    }
                        OpCodes[i] = hex_str;
                        OpCodes[opIndexer] = hex_value;
                        OpIndex++;
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
string searchForVarType(string searchString) {
    string result = "";
    for (auto tuple : TmemLoc) {
        if (get<0>(tuple) == searchString) {
            result = get<3>(tuple);
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
    elements.clear();

}