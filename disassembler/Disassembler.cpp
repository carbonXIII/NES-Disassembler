#include "Disassembler.h"
#include <cstdlib>

using namespace std;

Disassembler::Disassembler(NES* parent) : Processor(parent, parent->getCartridge()->getMinAddress(), parent->getCartridge()->getMaxAddress()) {
	fillInstructionTable();
}
Disassembler::Disassembler(NES* parent, word initPC, word maxPC) : Processor(parent, initPC, maxPC) {
	fillInstructionTable();
}

Disassembler::~Disassembler(){/*[TODO] graceful shutdown and cleanup of data*/}

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
	fill(((char*)instrSet.instructions) + (size - 256), ((char*)instrSet.instructions) + 1024, 0);
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
			addHex(getWordNext(),rtn,0);
			break;
		case indi:
			rtn += " ($";
			addHex(getByteNext(),rtn);
			rtn += ")";
			break;
		case aIndX:
			rtn += " $";
			addHex(getWordNext(),rtn,0);
			rtn += ",X";
			break;
		case aIndY:
			rtn += " $";
			addHex(getWordNext(),rtn,0);
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
	do{
		word initPC = PC;
		string asmString = processOP();
		if(asmString.size() != 0)cout << std::hex << initPC << std::dec << ": " << asmString << endl;
	}
	while(PC != 0);
	
	cout << "End of segment" << endl;
}
