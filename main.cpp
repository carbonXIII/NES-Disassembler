#define DISASM_ONLY

#include "disassembler/Disassembler.h"
#include <iostream>
#include <fstream>

using namespace std;

string path = "";
bool showLineNumbers = false;

ofstream file;
bool toFile = false;

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
	
	NES nes(&cart);
	Disassembler disasm(&nes);

	disasm.showLineNumbers(showLineNumbers);
	if(toFile)disasm.setOutputStream(&file);

	disasm.run();

	if(toFile)file.close();

    return 0;
}
