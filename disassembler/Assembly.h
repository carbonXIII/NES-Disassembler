#include <vector>
#include <string>
#include "../tools.h"

struct Instruction;

struct Line{//structure to hold all line information
    word addr;
    Instruction instr;
    word nextAddr[2];//0 (Stopping), 2 (Branch), 1 (Continue)
}

struct Block{//a block of code (seperated by branches)
private:   
    Block* parent;//null if the original block
    Block* children;//single element or array
    
    int numChildren = 0;
    
    std::vector<Line*> lines;
    
    void spawnChildren();
    
    bool complete = false;
    
    Block(Block* parent);
    
    void addLine(Line* line);
public:  
    Line get(int i);
    std::string toString();
    
    Block* getChildren();
    
    void fill(std::map<word,Line>* lines, word startAddr);//create tree from array of lines
}

class Assembly{
    std::map<word,Line> data;
}