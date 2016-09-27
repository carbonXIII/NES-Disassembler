#define DISASM_ONLY

#include "emulation/NES.h"
#include "disassembler/Disassembler.h"
#include <iostream>

using namespace std;

int main(){
	Disassembler disasm = Disassembler();

    NES nes = NES((Processor*)&disasm);
    
    return 0;
}
