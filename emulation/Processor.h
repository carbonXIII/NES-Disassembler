#ifndef EMULATION_PROCESSOR_H_
#define EMULATION_PROCESSOR_H_

#include "../tools.h"

class NES;

class Processor{
protected:
    NES* parent;//the NES to execute within
    
    word PC;//program counter (all processors have a program counter)
    word maxPC;//the failsafe max PC address
public:
    Processor(word initPC, word maxPC);
    
    virtual void run() = 0;
};

#endif
