#ifndef EMULATION_DISASSEMBLER_H_
#define EMULATION_DISASSEMBLER_H_

#include "Processor.cpp"

class Disassembler : public Processor{
public:
    Dissassembler();
    Dissassembler(word maxPC);
    
    void run();
}

#endif