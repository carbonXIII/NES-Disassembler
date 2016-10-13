#define DISASM_ONLY

#include "disassembler/Disassembler.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
	string path;
	
	if(argc > 1){
		path = string(argv[1]);
	}else {
		cout << "Input the path to the ROM file to disassemble: ";
		cin >> path;
	}

	Cartridge cart(path);
	cout << cart.getHeader()->toString();
	
	NES nes(&cart);
	Disassembler disasm(&nes);

	disasm.run();
    
    return 0;
}
