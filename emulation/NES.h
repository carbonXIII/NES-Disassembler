#include "../tools.h"
#include <iostream>

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
    
    Header(std::istream istr);
    
    inline long getPrgROMSize() const;
    inline long getChrROMSize() const;
    inline long getPrgRAMSize() const;
    inline bool isTrainer() const;
    inline bool isPCROM() const;
} __attribute__ ((pack));

class Cartridge{
    Header* head;
    byte* trainer;//if existent, 512 bytes
    byte* prgROM;//program ROM (contains the code and data sections of the program)
    byte* chrROM;//character ROM
    byte* pcROM;//players choice ROM (if existent, 16 bytes)
    
    void fill();
    
public:
    Cartridge(std::istream istr);//create a cartridge from a given byte stream
    Cartridge(Header* header);//create a cartride from a given header
    
    RAM* ram;
}

//the representation of the hardware of the NES (cartridge, processor, etc)
class NES{
    Cartridge cart;
  
}
