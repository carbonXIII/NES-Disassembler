#include "Assembly.h"
#include "Disassembler.h"
#include <exception>

using namespace std;

byte Instruction::numBranches() const{
	return op->addressMode >> 4;
}

word Instruction::getBranchAddress() const{
	switch(op->addressMode & NIBBLE_MASK){
		case indi:
			return addr + 1;//TODO find possible values of variable
		case rela:
			return operand + addr;
		default:
			return operand;
	}
}

string Instruction::toString(bool lnNumbering, bool branches) const{
	if(op->getName()[0] == 0)return "";
	
	string rtn;
	if(lnNumbering){
		addHexWord(addr, rtn);
		rtn += ": ";
	}
	
	rtn += op->getName() + " ";
	rtn += op->operandToString(operand);
	
	if(branches){
		rtn += " (";
		addHexNibble(numBranches(), rtn);
		rtn += ")";
	}
	return rtn;
}

Instruction::Instruction(word addr, Operation* op, word operand){
	this->addr = addr;
	this->op = op;
	this->operand = operand;
}

Block* BlockPool::createBlock(Block* parent, word addr){
	pool.emplace_back(this, parent, this->parent->find(addr));
	return (Block*)&*pool.end();
}

Block* BlockPool::createRoot(){
	pool.emplace_back(this, this->parent->begin());
	return (Block*)&*pool.end();
}

BlockPool::BlockPool(Assembly* parent){
	pool.reserve(1);
	this->parent = parent;
}

void Block::spawnChildren(){
	if(end->numBranches() == 1 || end->numBranches() == 14){
		children[0] = pool->createBlock(this, end->operand);
	}else if(end->numBranches() == 2){
		children = new Block*[2];
		children[0] = pool->createBlock(this, end->addr + 1);
		children[1] = pool->createBlock(this, end->operand);
	}
}

bool Block::processLine(vector<Instruction>::iterator line){
	if(line->numBranches() == 0 || line->numBranches() == 14){
		return false;
	}else{
		spawnChildren();
		return true;
	}
}

Block::Block(BlockPool* pool, Block* parent, vector<Instruction>::iterator begin){
	this->pool = pool;
	this->parent = parent;
	this->begin = begin;
}

Block::Block(BlockPool* pool, vector<Instruction>::iterator begin){
	this->pool = pool;
	this->begin = begin;
}

byte Block::numChildren() const{
	end->numBranches();
}

void Block::fill(){
	for(auto i = begin; i != pool->getAssembly()->end(); i++){
		if(processLine(i))break;
	}if(numChildren() != 14)for(int i = 0; i < numChildren(); i++){
		children[i]->fill();
	}
}

vector<Instruction>::iterator Block::get(int i){
	return begin + i;
}

Block* Block::getChild(int i){
	return children[i];
}

vector<Instruction>::iterator Assembly::find(word address){
	auto min = lines.begin();
	auto max = lines.end();

	do{
		auto i = min + (min - max)/2;

		if(i->addr == address)return i;
		if(i->addr < address)max = i;
		if(i->addr > address)min = i;
	}while(min != max);

	throw invalid_argument("Assembly does not contain valid instruction at the given address.");
}

void Assembly::developTree(){
	blockPool.createRoot();
	blockPool.pool[0].fill();
}

void Assembly::addLine(Instruction& instr){
	lines.push_back(instr);
}

Assembly::Assembly() : blockPool(this) {}