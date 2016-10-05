#define DISASM_ONLY

#include "disassembler/Disassembler.h"
#include <iostream>

using namespace std;

int main(){
	string path;
	cout << "Input the path to the ROM file to disassemble: ";
	cin >> path;

	Disassembler disasm;
	Cartridge cart(path);

	NES nes = NES(&disasm, &cart);

	disasm.run();
    
    return 0;
}
