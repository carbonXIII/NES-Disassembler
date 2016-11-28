//compresses a spreadsheet of 6502 op code information into a .bin file for use during disassembly/emulation (without sacrificing speed of lookup or memory)

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cmath>

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
    cout << "The csv file must be in the order: \"code (2 hex digits), name, address, # of branches (1 hex digit)\"!" << endl;
    exit(1);
}

int hexToInt(char* a, int n=2){
//n: digits to read
	
	int total = 0;
    for(int i = 0; i < n; i++){
		if(a[i] >= '0' && a[i] <= '9'){
			total += (a[i] - '0') << ((n - i - 1)*4);
		}else if(a[i] >= 'A' && a[i] <= 'F'){
			total += (a[i] - 'A' + 10) << ((n - i - 1)*4);
		}else{
			errCSV();
		}
	}
	return total;
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
    
	char* branches;
    char* addr = strtok(0,",");
	if(addr == nullptr || (*(addr - 1) == ',' && *(addr - 2) == '\0')){
		/*replace a skipped string from strtok with an empty string and set the value returned from strtok to the string reperesentation of the branches. using a hardcoded yet consistent hack to detect this*/
    	branches = addr;
		addr = new char[1];
    	addr[0] = '\0';
    }else if(addr[0] == '\"'){//remove quotes
        if(addr[strlen(addr) - 1] != '\"'){
			addr[strlen(addr)] = ',';
			strtok(0, ",");
			addr[strlen(addr) - 1] = '\0';
		}
		addr++;
    }

    if(!addressNames.count(string(addr))){
        cout << "Unknown addressing mode for op code \'" << (int)addr[0] << "\'. Input the correct addressing mode # from list:" << endl;
        printAddressOptions();
        int mode; cin >> mode;
        buffer[bI*4+3] = mode;
    }else{
        buffer[bI*4+3] = addressNames.at(string(addr));
    }
	
	if(branches == nullptr) branches = strtok(0,",");
	buffer[bI*4+3] |= hexToInt(branches,1) << 4;
       
    bI++;//increment to next available slot
}

int main(int argc, char** argv){
    fill(lookup,lookup+256,0xFF);
	if(argc >= 1){//there are arguments
        if(processArgs(argc, argv))
            exit(1);
    }
    
    if(csvPath == ""){
        cout << "Input the path to the csv file (note: the csv file must be in the order: \"code (2 hex digits), name, address, # of branches (1 dec digit)\"):" << endl;
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
