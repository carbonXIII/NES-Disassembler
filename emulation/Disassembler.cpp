#include "Disassembler.h"

Disassembler::Disassembler(){
    Super(parent->getCartridge()->getMinAddress(),parent->getCartridge()->getMaxAddress());//only disassemble the PRG rom
}

Disassembler::Disassembler(word maxPC){
    Super(parent->getCartridge()->getMinAddress(), maxPC);//only disassemble the PRG rom
}

void Disassembler::run(){
    //[TODO]: Add disassembly code
}