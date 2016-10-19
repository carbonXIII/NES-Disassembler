//compresses a spreadsheet of 6502 op code information into a .bin file for use during disassembly/emulation (without sacrificing speed of lookup)

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef unsigned char byte;

string outputPath = "";
ofstream output;

string csvPath = "";
ifstream csvFile;
long csvFileSize = 0;

enum : char {accu, imme, impl, rela, abso, zero, indi, aIndX, aIndY, zIndX, zIndY, indXI, indYI, iIndX, iIndY};//accumulator,immediate,implied,relative,absolute,zero-page,indirect,absolute-indexed,zero-indexed,indexed-indirect,indirect-indexed (respectively with x and y variants of indexed ops)
const std::map<string, int> addressNames = {
    {string("Accumulator"),accu},
    {string("Immediate"),imme},
    {string(""),impl},
    {string("Relative"),rela},
    {string("Absolute"),abso},
    {string("Zero Page"),zero},
	{string("Indirect"),indi},
    {string("Absolute, X"),aIndX},
    {string("Absolute, Y"),aIndY},
    {string("Zero Page, X"),zIndX},
    {string("Zero Page, Y"),zIndY},
    {string("(Indirect, X)"),indXI},
    {string("(Indirect, Y)"),indYI},
    {string("(Indirect), X"),iIndX},
    {string("(Indirect), Y"),iIndY}
};

byte lookup[256];//the lookup table for the ordering of the opcodes (also to be written to the file)
byte buffer[1024];//the used 4 byte sections will be written to the file
int bI = 0;

void printUsage(){
    cout << "USAGE: compressor [-o output] [csvpath]" << endl;
    cout << "\t-o     : Specify the file path of the compressed output file" << endl;
    cout << "\tcsvpath: The file path of the input csv file" << endl;
}

void dumpTable(ostream& out){//dump the lookup table followed by the used portion of the buffer to a stream
    out.write((char*)lookup,256);
    out.write((char*)buffer,bI*4);
}

void printAddressOptions(){
    for(auto it = addressNames.begin(); it != addressNames.end(); it++){
        cout << "\t" << it->second << ": " << it->first << endl;
    }
}

bool processArgs(int argc, char** argv){//returns true on failure
    for(int i = 1; i < argc; i++){
    	if(strcmp(argv[i],"-o") == 0){
    		if(argc > ++i){
                outputPath = string(argv[i]);
            }else{
                printUsage();
                return true;
            }
        }else{
            csvPath = string(argv[i]);
        }
    }
    
    return false;
}

void errCSV(){
    cout << "The csv file must be in the order: \"code (2 hex digits), name, address\"!" << endl;
    exit(1);
}

int hexToInt(char* a){
    stringstream ss;

    int rtn;
    ss << std::hex << a;
    ss >> rtn;

    if(ss.fail()){
    	errCSV();
    }

    return rtn;
}

int decToInt(char a){
	if(a >= '0' && a <= '9'){
		return a  - '0';
	}errCSV();
}

bool processLine(string* line){
	cout << "Processing line \'" << (*line) << "\'" << endl;
    
    char* str = new char[line->size()];
    strcpy(str, line->data());
    
    char* code = strtok(str,",");
    if(code[2] != ' ' && code[2] != '\0')errCSV();
    lookup[hexToInt(code)] = bI;//add the location of the op code info to the lookup table
    
    char* name = strtok(0,",");
    for(int i = 0; i < 3; i++){
        if(name[i] == ' ' || name[i] == '\0')errCSV();
        buffer[bI*4+i] = name[i];
    }
    
    char* addr = strtok(0,",");
    if(addr == nullptr){//replace a nullptr from strtok with an empty string
    	addr = new char[1];
    	addr[0] = '\0';
    }if(addr[0] == '\"'){//remove quotes
        if(addr[strlen(addr) - 1] != '\"')addr[strlen(addr)] = ',';
        addr++;
    }while(addr[strlen(addr) - 1] == ' ' || addr[strlen(addr) - 1] == '\r' || addr[strlen(addr) - 1] == '\"'){//remove spaces and line feeds
        addr[strlen(addr) - 1] = '\0';
    }

    if(!addressNames.count(string(addr))){
        cout << "Unknown addressing mode for op code \'" << code << "\'. Input the correct addressing mode # from list:" << endl;
        printAddressOptions();
        int mode; cin >> mode;
        buffer[bI*4+3] = mode;
    }else{
        buffer[bI*4+3] = addressNames.at(string(addr));
    }
	
	char* branches = strtok(0,",");
	buffer[bI*4+3] |= decToInt(branches[0]) << 4;
       
    bI++;//increment to next available slot
}

int main(int argc, char** argv){
    fill(lookup,lookup+256,0xFF);
	if(argc >= 1){//there are arguments
        if(processArgs(argc, argv))
            exit(1);
    }
    
    if(csvPath == ""){
        cout << "Input the path to the csv file (note: the csv file must be in the order: \"code (2 hex digits), name, address\"):" << endl;
        cin >> csvPath;
    }
    
    try{
        csvFile.open(csvPath.c_str());
    }catch (int e) {
        cout << "Failed to open file." << endl;
        return 0;
    }
    
    string ln;
    while(!csvFile.eof()){
    	getline(csvFile, ln);
    	if(ln == "")break;
    	processLine(&ln);
    }
    
    bool print = false;
    if(outputPath == ""){
        cout << "Print the table to the console? ";
        cin >> print;
        if(print){
            dumpTable(cout);
        }else{
            cout << "File output path? ";
            cin >> outputPath;
        }
    }
    
    try{
        output.open(outputPath.c_str(), ios::binary);
    }catch(int e){
        cout << "Failed to create output file." << endl;
        return 0;
    }
    
    dumpTable(output);
    
    return 0;
}
