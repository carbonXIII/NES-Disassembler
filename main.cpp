#define DISASM_ONLY

#include <iostream>
#include "emulation/NES.h"
#include "emulation/Disassembler.h"

using namespace std;

int main(){
    Disassembler* disas = new Disassembler();
    NES nes(cin, disas);
    
    return 0;
}
