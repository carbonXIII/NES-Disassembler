#define DISASM_ONLY

#include "disassembler/Disassembler.h"
#include <iostream>
#include <fstream>

using namespace std;

string path = "";
bool showLineNumbers = false;
bool showBranches = false;

ofstream file;
bool toFile = false;

void printUsage(){//print a usage statement
	cout << "disasm ROM_FILE [-l | -b] [-o OUTPUT_FILE]" << endl;
	cout << "\t-l         : print line numbers" << endl;
	cout << "\t-b         : print number of potential branches next to each line" << endl;
	cout << "\tROM_FILE   : path to the rom file" << endl;
	cout << "\tOUTPUT_FILE: path to the output file" << endl;
}

bool processArgs(int argc, char** argv){//returns 1 on error
	for(int i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if(argv[i][1] == 'l'){
				showLineNumbers = true;
			}else if(argv[i][1] == 'b'){
				showBranches = true;	
			}else if(argv[i][1] == 'o'){
				i++;
				file.open(argv[i]);
				toFile = true;
			}else{
				printUsage();
				return true;
			}
		}else{
			path = string(argv[i]);
		}
	}
	
	if(path == ""){
		cout << "Input the path to the ROM file to disassemble: ";
		cin >> path;
	}
	
	return false;
}

int main(int argc, char** argv){
	if(processArgs(argc, argv))return 0;
	
	Cartridge cart(path);
	if(!toFile)cout << cart.getHeader()->toString() << endl;
	else file << cart.getHeader()->toString() << endl;
	
	Assembly assembly;
	NES nes(&cart);
	
	Disassembler disasm(&nes, &assembly);
	disasm.showLineNumbers(showLineNumbers);
	disasm.showBranches(showBranches);
	if(toFile)disasm.setOutputStream(&file);
	
	disasm.run();

	if(toFile)file.close();

    return 0;
}
