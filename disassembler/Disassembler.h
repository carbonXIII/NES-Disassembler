#ifndef EMULATION_DISASSEMBLER_H_
#define EMULATION_DISASSEMBLER_H_

#include "../emulation/NES.h"

class Disassembler : public Processor{
public:
    Disassembler();
    Disassembler(word maxPC);
    
    ~Disassembler();

    void run();
};

#endif
