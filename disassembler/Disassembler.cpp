#include "Disassembler.h"
#include <cstdlib>

using namespace std;

Disassembler::Disassembler() : Processor(parent->getCartridge()->getMinAddress(),parent->getCartridge()->getMaxAddress()){}

Disassembler::Disassembler(word maxPC) : Processor(parent->getCartridge()->getMinAddress(), maxPC) {}

Disassembler::~Disassembler(){
	//[TODO] graceful shutdown and cleanup of data
}

void Disassembler::fillInstructionTable(){
	ifstream fin;
	int size = 0;

	try{
		fin.open(OP_FILE, ios::ate | ios::binary);
		size = fin.tellg();
		fin.seekg(0, ios::beg);
	}catch(int& e){
		cout << "Instruction set file not loaded! Ensure " << OP_FILE << " exists!" << endl;
		exit(1);
	}
	
	fin.read((char*)&instrSet, size);
}

string Disassembler::processOperands(char addressMode){
	string rtn = "";
	switch(addressMode){
		case accu:
			rtn += " A";
			break;
		case imme:
			rtn += " #$";
			addHex(getByteNext(),rtn);
			break;
		case rela:
		case zero:
			rtn += " $";
			addHex(getByteNext(),rtn);
			break;
		case abso:
			rtn += " $";
			addHex(getWordNext(),rtn);
			break;
		case indi:
			rtn += " ($";
			addHex(getByteNext(),rtn);
			rtn += ")";
			break;
		case aIndX:
			rtn += " $";
			addHex(getWordNext(),rtn);
			rtn += ",X";
			break;
		case aIndY:
			rtn += " $";
			addHex(getWordNext(),rtn);
			rtn += ",Y";
			break;
		case zIndX:
			rtn += " $";
			addHex(getByteNext(),rtn);
			rtn += ",X";			
			break;
		case zIndY:
			rtn += " $";
			addHex(getByteNext(),rtn);
			rtn += ",Y";			
			break;
		case iIndX:
			rtn += " ($";
			addHex(getByteNext(),rtn);
			rtn += "),X";			
			break;
		case iIndY:
			rtn += " ($";
			addHex(getByteNext(),rtn);
			rtn += "),Y";				
			break;
		case indXI:
			rtn += " ($";
			addHex(getByteNext(),rtn);
			rtn += ",X)";				
			break;
		case indYI:
			rtn += " ($";
			addHex(getByteNext(),rtn);
			rtn += ",Y)";			
			break;
	}return rtn;
}

string Disassembler::processOP(){
	string rtn = "";
	
	byte op = getByteNext();
	byte location = instrSet.lookup[op];
	
	Instruction* instr = &instrSet.instructions[location];
	if(instr->getName()[0] == 0)return "";
	
	rtn += instr->getName();
	rtn += processOperands(instr->addressMode);

	return rtn;
}

void Disassembler::run(){
    while(PC < maxPC){
		cout << processOP() << endl;
	}
	
	cout << "End of segment" << endl;
}
