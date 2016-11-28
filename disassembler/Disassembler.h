#ifndef DISASSEMBLER_DISASSEMBLER_H_
#define DISASSEMBLER_DISASSEMBLER_H_

#include "../emulation/NES.h"
#include "Assembly.h"
#include <iostream>

struct Operation{
	char name[3];
	char addressMode;

	std::string getName() const {return std::string(name,3);}
	std::string operandToString(word operand) const;
}__attribute__ ((packed));

class Disassembler : public Processor{
	
	Assembly* assembly;
    
    void fillOPTable();

    word getOperands(char addressMode);
    std::string processOP();//processes the next OP and increments the PC to the beginning of the following OP
	
	bool lineNumbers = false;
	bool branches = false;
	std::ostream* out = &std::cout;
public:
	struct OPTable{
        byte lookup[256];
        Operation operations[256];

        Operation* getOP(byte opCode);
    } opTable;
	
    Disassembler(NES* parent, Assembly* assembly);
    Disassembler(NES* parent, Assembly* assembly, word initPC, word maxPC);
	
	void showLineNumbers(bool show = true) {lineNumbers = show;}
	void showBranches(bool show = true) {branches = show;}
	void setOutputStream(std::ostream* out) {this->out = out;}

    ~Disassembler();
    
    virtual void run();
};

#endif
