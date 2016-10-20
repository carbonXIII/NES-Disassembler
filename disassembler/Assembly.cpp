#include "Assembly.h"
#include <exception>

using namespace std;

byte Instruction::numBranches() const{
	return op->addressMode >> 4;
}

word Instruction::getBranchAddress() const{
	return operand;
}

string Instruction::toString(bool lnNumbering) const{
	string rtn;
	if(lnNumbering){
		addHex(addr, rtn, 0);
		rtn += ": ";
	}

	rtn += op->getName() + " ";
	rtn += op->operandToString(operand);

	return rtn;
}

Instruction::Instruction(word addr, Operation* op, word operand){
	this->addr = addr;
	this->op = op;
	this->operand = operand;
}

Block* BlockPool::createBlock(Block* parent, word addr){
	return pool.emplace_back(*this, parent, this->parent.find(addr));
}

Block* BlockPool::createRoot(){
	return pool.emplace_back(*this, this->parent.begin());
}

void Block::spawnChildren(){
	if(end->numBranches() == 1){
		children[0] = pool.createBlock(this, end->operand);
	}else{
		children = new Block[2];
		children[0] = pool.createBlock(this, end->addr + 1);
		children[1] = pool.createBlock(this, end->operand);
	}
}

bool Block::processLine(vector<Instruction>::iterator line){
	end = line->numBranches();
	if(line->numBranches() == 0){
		return false;
	}else{
		spawnChildren();
		return true;
	}
}

Block::Block(BlockPool& pool, Block* parent, vector<Instruction>::iterator begin){
	this->pool = pool;
	this->parent = parent;
	this->begin = begin;
}

Block::Block(BlockPool& pool, vector<Instruction>::iterator begin){
	this->pool = pool;
	this->begin = begin;
}

byte Block::numChildren() const{
	end->numBranches();
}

void Block::fill(){
	for(auto i = begin; i++; i != pool.getAssembly().end()){
		if(processLine(i))break;
	}for(int i = 0; i < numChildren(); i++){
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
