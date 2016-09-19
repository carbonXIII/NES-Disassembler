#ifndef EMULATION_DISASSEMBLER_H_
#define EMULATION_DISASSEMBLER_H_

#include "NES.h"

class Disassembler : public Processor{
public:
    Disassembler();
    Disassembler(word maxPC);
    
    void run();
};

#endif
