#define DISASM_ONLY

#include "disassembler/Disassembler.h"
#include <iostream>

using namespace std;

int main(){
	string path;
	cout << "Input the path to the ROM file to disassemble: ";
	cin >> path;

	Cartridge cart(path);
	NES nes(&cart);
	Disassembler disasm(&nes);

	disasm.run();
    
    return 0;
}
