#include "Disassembler.h"

Disassembler::Disassembler() : Processor(parent->getCartridge()->getMinAddress(),parent->getCartridge()->getMaxAddress()){}

Disassembler::Disassembler(word maxPC) : Processor(parent->getCartridge()->getMinAddress(), maxPC) {}

Disassembler::~Disassembler(){
	//graceful shutdown and cleanup of data
}

void Disassembler::run(){
    //[TODO]: Add disassembly code
}
