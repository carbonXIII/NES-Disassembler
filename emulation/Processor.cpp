#include "Processor.h"
#include "NES.h"

Processor::Processor(word initPC, word maxPC){
    PC = initPC;
    this->maxPC = maxPC;
    parent = nullptr;
}

void Processor::setParent(NES* parent) {
	this->parent = parent;
}

byte Processor::getByte() {
	return parent->getCartridge()->get(PC);
}
byte Processor::getByteNext(){
	return parent->getCartridge()->get(PC);
}

word Processor::getWord() {
	return ((word)parent->getCartridge()->get(PC) << 8) & parent->getCartridge()->get(PC+1);
}
word Processor::getWordNext() {
	return ((word)parent->getCartridge()->get(PC++) << 8) & parent->getCartridge()->get(PC++);
}
