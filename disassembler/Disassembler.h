#ifndef EMULATION_DISASSEMBLER_H_
#define EMULATION_DISASSEMBLER_H_

#include "../emulation/NES.h"
#include <iostream>

enum : char {accu, imme, impl, rela, abso, zero, indi, aIndX, aIndY, zIndX, zIndY, indXI, indYI, iIndX, iIndY};

struct Instruction{
	char name[3];
	char addressMode;

	std::string getName() const {return std::string(name,3);}
}__attribute__ ((packed));

class Disassembler : public Processor{
	struct InstructionSet{
        byte lookup[256];
        Instruction instructions[256];
    } instrSet;
    
    void fillInstructionTable();
    std::string processOperands(char addressMode);//read the next few bytes and increment the PC to the end of the operands (the beginning of the following OP)
    std::string processOP();//processes the next OP and increments the PC to the beginning of the following OP
	
	bool lineNumbers = false;
	std::ostream* out = &std::cout;
public:
    Disassembler(NES* parent);
    Disassembler(NES* parent, word initPC, word maxPC);
	
	void showLineNumbers(bool show = true) {lineNumbers = show;}
	void setOutputStream(std::ostream* out) {this->out = out;}

    ~Disassembler();
    
    virtual void run();
};

#endif
