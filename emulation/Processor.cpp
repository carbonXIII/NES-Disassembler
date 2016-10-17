#include "Processor.h"
#include "NES.h"

#include <cassert>
#include <iostream>

using namespace std;

Processor::Processor(NES* parent, word initPC, word maxPC){
    this->parent = parent;
    assert(validParent());

    PC = initPC;
    this->maxPC = maxPC;
}

byte Processor::getByte() {
	cout << "\t" << PC << endl;
	return parent->getCartridge()->get(PC);
}
byte Processor::getByteNext(){
	return parent->getCartridge()->get(PC++);
}

word Processor::getWord() {
	return (word)parent->getCartridge()->get(PC) | ((word)parent->getCartridge()->get(PC+1) << 8);
}
word Processor::getWordNext() {
	word rtn = (word)parent->getCartridge()->get(PC) | ((word)parent->getCartridge()->get(PC+1) << 8);
	PC += 2;
	return rtn;
}
