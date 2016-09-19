#include "Disassembler.h"

Disassembler::Disassembler() : Processor(parent->getCartridge()->getMinAddress(),parent->getCartridge()->getMaxAddress()){}

Disassembler::Disassembler(word maxPC) : Processor(parent->getCartridge()->getMinAddress(), maxPC) {};

void Disassembler::run(){
    //[TODO]: Add disassembly code
}
