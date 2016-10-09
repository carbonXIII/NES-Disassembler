#ifndef EMULATION_PROCESSOR_H_
#define EMULATION_PROCESSOR_H_

#include "../tools.h"

class NES;

class Processor{
    bool validParent() const {return parent != nullptr;}

protected:
    NES* parent;//the NES to execute within
    
    word PC;//program counter (all processors have a program counter)
    word maxPC;//the failsafe max PC address
    
    byte getByte();//get a CPU byte and don't increment the PC
    byte getByteNext();//get a CPU byte and increment the PC
    
    word getWord();
    word getWordNext();
public:
    Processor(NES* parent, word initPC, word maxPC);
    virtual ~Processor() {};
    
    virtual void run() = 0;//update loop; might be run on a seperate thread (depending on implementation)
};

#endif
