#include "../tools.h"

#ifndef DISASM_ONLY
//include all headers not needed for disassemlby
#endif

struct RAM;//predefine RAM

struct Header{
    byte sig[3];
    byte prgROMSize;//program ROM size in 16KB units
    byte chrROMSize;//character ROM size in 8KB units
    FlagByte f6;
    FlagByte f7;
    byte prgRAMSize;//program RAM size in 8KB units
    FlagByte f9;
    FlagByte f10;//unused
    //followed by 5 bytes of 0s
    
    Header(istream istr){
        istr.read((char*)this, 11);
        istr.ignore(5);
    }
    
    inline long getPrgROMSize(){
        return prgROMSize << 14;
    }
    
    inline long getChrROMSize(){
        return prgROMSize << 13;
    }
    
    inline long getPrgRAMSize(){
        return prgRAMSize << 13;
    }
    
    inline bool isTrainer(){
        return f6.b2();   
    }
    
    inline bool isPCROM(){
        return f7.b1();
    }
} __attribute__ ((pack));

class Cartridge{
    Header* head;
    byte* trainer;//if existent, 512 bytes
    byte* prgROM;//program ROM (contains the code and data sections of the program)
    byte* chrROM;//character ROM
    byte* pcROM;//players choice ROM (if existent, 16 bytes)
    
    void fill(){//fill the Cartridge info based on Header info
        trainer = ne byte[head.];
        prgROM = new byte[head.getPrgROMSize()];
        chrROM = new byte[head.getChrROMSize()];
        if(head.isPCROM())pcROM = new byte[16];
    }
    
public:
    Cartridge(istream istr){//create a cartridge from a given byte stream
        head = new Header(istr);
        fill();
    }
    
    Cartridge(Header* header){//create a cartride from a given header
        this.head = header;//save a pointer to the required header for future ref
        fill();
    }
    
    RAM* ram;
}

//the representation of the hardware of the NES (cartridge, processor, etc)
class NES{
    Cartridge cart;
  
}