#include "Disassembler.h"
#include <cstdlib>

using namespace std;

Disassembler::Disassembler(NES* parent, Assembly* assembly) : Processor(parent, parent->getCartridge()->getMinAddress(), parent->getCartridge()->getMaxAddress()) {
	this->assembly = assembly;
	fillOPTable();
}
Disassembler::Disassembler(NES* parent, Assembly* assembly, word initPC, word maxPC) : Processor(parent, initPC, maxPC) {
	this->assembly = assembly;
	fillOPTable();
}

Disassembler::~Disassembler(){/*[TODO] graceful shutdown and cleanup of data*/}

Operation* Disassembler::OPTable::getOP(byte opCode){
	return operations + lookup[opCode];
}

void Disassembler::fillOPTable(){
	ifstream fin;
	int size = 0;

	try{
		fin.open(OP_FILE, ios::ate | ios::binary);
		size = fin.tellg();
		fin.seekg(0, ios::beg);
	}catch(int& e){
		*out << "Operation table file not loaded! Ensure " << OP_FILE << " exists!" << endl;
		exit(1);
	}
	
	fin.read((char*)&opTable, size);
	fill(((char*)opTable.operations) + (size - 256), ((char*)opTable.operations) + 1024, 0);
}

word Disassembler::getOperands(char addressMode){
	switch (addressMode & NIBBLE_MASK){
		case imme:
		case rela:
		case zero:
		case indi:
		case zIndX:
		case zIndY:
		case iIndX:
		case iIndY:
		case indXI:
		case indYI:
			return getByteNext();
		case abso:
		case aIndX:
		case aIndY:
			return getWordNext();
		default:
			return 0;
	}
}

string Operation::operandToString(word operand) const{
	string rtn = "";
	switch(addressMode & NIBBLE_MASK){
		case accu:
			rtn += "A";
			break;
		case imme:
			rtn += "#$";
			addHex(operand & BYTE_MASK,rtn);
			break;
		case rela:
		case zero:
			rtn += "$";
			addHex(operand & BYTE_MASK,rtn);
			break;
		case abso:
			rtn += "$";
			addHex(operand,rtn,0);
			break;
		case indi:
			rtn += "($";
			addHex(operand & BYTE_MASK,rtn);
			rtn += ")";
			break;
		case aIndX:
			rtn += "$";
			addHex(operand,rtn,0);
			rtn += ",X";
			break;
		case aIndY:
			rtn += "$";
			addHex(operand,rtn,0);
			rtn += ",Y";
			break;
		case zIndX:
			rtn += "$";
			addHex(operand & BYTE_MASK,rtn);
			rtn += ",X";			
			break;
		case zIndY:
			rtn += "$";
			addHex(operand & BYTE_MASK,rtn);
			rtn += ",Y";			
			break;
		case iIndX:
			rtn += "($";
			addHex(operand & BYTE_MASK,rtn);
			rtn += "),X";			
			break;
		case iIndY:
			rtn += "($";
			addHex(operand & BYTE_MASK,rtn);
			rtn += "),Y";				
			break;
		case indXI:
			rtn += "($";
			addHex(operand & BYTE_MASK,rtn);
			rtn += ",X)";				
			break;
		case indYI:
			rtn += "($";
			addHex(operand & BYTE_MASK,rtn);
			rtn += ",Y)";			
			break;
	}return rtn;
}

string Disassembler::processOP(){
	word initPC = PC;
	
	byte opCode = getByteNext();
	Operation* op = opTable.getOP(opCode);
	word operands = getOperands(op->addressMode);

	Instruction instr(initPC, op, operands);
	assembly->addLine(instr);//add the line to the assembly
	return instr.toString(lineNumbers, branches);
}

void Disassembler::run(){
	do{
		string asmString = processOP();
		if(asmString.size() != 0){
			*out << asmString << endl;
		}
	}while(PC != 0);
	
	*out << "End of segment" << endl;
}
