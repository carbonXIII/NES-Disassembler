#ifndef EMULATION_DISASSEMBLER_H_
#define EMULATION_DISASSEMBLER_H_

#include "../emulation/NES.h"

enum : char {accu, imme, impl, rela, abso, zero, indi, aIndX, aIndY, zIndX, zIndY, indXI, indYI, iIndX, iIndY};

class Disassembler : public Processor{
    struct InstructionSet{
        byte lookup[256];
        struct Instruction{
            char name[3];
            char addressMode;
            
            string getName() const {return string(name,3);}
        }__attribute__ ((packed)) instructions[256];
    }__attribute__ ((packed)) instrSet;
    
    void fillInstructionTable();
public:
    Disassembler();
    Disassembler(word maxPC);
    
    ~Disassembler();
    
    std::string processOperands(char addressMode);//read the next few bytes and increment the PC to the end of the operands (the beginning of the following OP)
    std::string processOP();//processes the next OP and increments the PC to the beginning of the following OP
    
    void run();
};

#endif
