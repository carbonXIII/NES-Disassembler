#include "Processor.h"
#include "NES.h"

#include <cassert>

Processor::Processor(NES* parent, word initPC, word maxPC){
    this->parent = parent;
    assert(validParent());

    PC = initPC;
    this->maxPC = maxPC;
}

byte Processor::getByte() {
	return parent->getCartridge()->get(PC);
}
byte Processor::getByteNext(){
	return parent->getCartridge()->get(PC++);
}

word Processor::getWord() {
	return ((word)parent->getCartridge()->get(PC) << 8) & parent->getCartridge()->get(PC+1);
}
word Processor::getWordNext() {
	return ((word)parent->getCartridge()->get(PC++) << 8) & parent->getCartridge()->get(PC++);
}
