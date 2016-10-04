#ifndef EMULATION_PROCESSOR_H_
#define EMULATION_PROCESSOR_H_

#include "../tools.h"

class NES;

class Processor{
protected:
    NES* parent;//the NES to execute within
    
    word PC;//program counter (all processors have a program counter)
    word maxPC;//the failsafe max PC address
    
    byte getByte() {return parent->getCartridge()->get(PC);}//get a CPU byte and don't increment the PC
    byte getByteNext(return parent->getCartridge()->get(PC++)}//get a CPU byte and increment the PC
    
    word getWord() {return ((word)parent->getCartridge()->get(PC) << 8) & parent->getCartridge()->get(PC+1);}
    word getWordNext() {return ((word)parent->getCartridge()->get(PC++) << 8) & parent->getCartridge()->get(PC++);}
                     
public:
    Processor(word initPC, word maxPC);
    virtual ~Processor() {};
    
    virtual void run() = 0;//update loop; might be run on a seperate thread (depending on implementation)
};

#endif
