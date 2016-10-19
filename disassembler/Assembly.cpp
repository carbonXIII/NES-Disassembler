#include "Assembly.h"

using namespace std;

Block::Block(Block* parent): lines(){
    this->parent = parent;
}

void Block::addLine(Line* line){
    lines.add(line);
    if(line->branches > 0){
        numBranches = line->branches;
        complete = true;
        spawnChildren();
    }
}

void Block::fill(Line* line){
    int i = 0;
    while(!complete){
        addLine(line + i);
    }
}