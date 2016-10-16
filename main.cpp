#define DISASM_ONLY

#include "disassembler/Disassembler.h"
#include <iostream>

using namespace std;

string path;
bool showLineNumbers = false;

void printUsage(){//print a usage statement
	cout << "disasm [ROMFile] [-l]" << endl;
	cout << "\t-l     : print line numbers" << endl;
	cout << "\tROMFile: path to the rom file" << endl;
}

bool processArgs(int argc, char** argv){//returns 1 on error
	for(int i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if(argv[i][1] == 'l'){
				showLineNumbers = true;
			}else{
				printUsage();
				return true;
			}
		}else{
			path = string(argv[i]);
		}
	}
	
	if(path == 0){
		cout << "Input the path to the ROM file to disassemble: ";
		cin >> path;
	}
	
	return false;
}

int main(int argc, char** argv){
	if(processArgs(argc, argv))return 0;
	
	Cartridge cart(path);
	cout << cart.getHeader()->toString();
	
	NES nes(&cart);
	Disassembler disasm(&nes);
	disasm.showLineNumbers(showLineNumbers);

	disasm.run();
    
    return 0;
}
