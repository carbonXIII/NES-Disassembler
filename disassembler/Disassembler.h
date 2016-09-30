#ifndef EMULATION_DISASSEMBLER_H_
#define EMULATION_DISASSEMBLER_H_

#include "../emulation/NES.h"

class Disassembler : public Processor{
public:
    Disassembler();
    Disassembler(word maxPC);
    
    ~Disassembler();

    std::string processOP();//processes the next OP and increments the PC to the beginning of the following OP
    
    void run();
};

#endif
