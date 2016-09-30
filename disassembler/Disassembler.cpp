#include "Disassembler.h"
#include <iostream>

using namespace std;

Disassembler::Disassembler() : Processor(parent->getCartridge()->getMinAddress(),parent->getCartridge()->getMaxAddress()){}

Disassembler::Disassembler(word maxPC) : Processor(parent->getCartridge()->getMinAddress(), maxPC) {}

Disassembler::~Disassembler(){
	//[TODO] graceful shutdown and cleanup of data
}

string Disassembler::processOP(){
	
}

void Disassembler::run(){
    while(PC < maxPC){
		cout << processOP() << endl;
	}
	
	cout << "End of segment" << endl;
}
