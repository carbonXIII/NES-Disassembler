#ifndef DISASSEMBLER_ASSEMBLY_H_
#define DISASSEMBLER_ASSEMBLY_H_

#include <vector>
#include <string>
#include <map>
#include "../tools.h"

struct Operation;

struct Instruction{//structure to hold all line information
    word addr;
    Operation* op;
    word operand;

    byte numBranches() const;//the number of branches: 0 (CONTINUE), 1(JUMP/BREAK), 2(BRANCH)
    word getBranchAddress() const;//only valid if numBranches != 0

    std::string toString(bool lnNumbering = false) const;

    Instruction(word addr, Operation* op, word operand);
};

struct BlockPool{
private:
	Assembly& parent;//the assembly the block pool is stored in
public:
	std::vector<Block> pool;

	Block* createBlock(Block* parent, word addr);
	Block* createRoot();
	Assembly& getAssembly() {return parent;}
};

struct Block{//a block of code (seperated by branches)
friend struct BlockPool;

private:
	BlockPool& pool;//a pointer to the pool to create new blocks in
    Block* parent;//nullptr if root block
    Block** children;//single element or array; not valid if the block is a leaf
    
    std::vector<Instruction>::iterator begin;
    std::vector<Instruction>::iterator end;

    void spawnChildren();
    bool processLine(std::vector<Instruction>::iterator line);//returns true if the line ends the block; if the line is the end of the block, sets the end iterator to that value

protected:
    Block(BlockPool& pool, Block* parent, std::vector<Instruction>::iterator begin);//construct a branch block
	Block(BlockPool& pool, std::vector<Instruction>::iterator begin);//construct a root block

public:
    void fill();//develops a tree from a vector of instruction lines
    byte numChildren() const;//returns the number of children in the block

    std::vector<Instruction>::iterator get(int i);
    Block* getChild(int i);
};

class Assembly{
    std::vector<Instruction> lines;
    BlockPool blockPool;

public:
    std::vector<Instruction>::iterator begin() const {return lines.begin();}//return the iterator for the first instruction
    std::vector<Instruction>::iterator end() const {return lines.end();}//return the iterator for the last instruction added
    std::vector<Instruction>::iterator find(word addr);//binary search as all the lines are in order by address

    void addLine(Instruction& instr);
};

#endif
