#ifndef EMULATION_NES_H_
#define EMULATION_NES_H_

#include "../tools.h"
#include "Addressable.h"
#include "Processor.h"

#ifndef DISASM_ONLY
//include all headers not needed for disassembly
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

    inline long getPrgROMSize() const;
    inline long getChrROMSize() const;
    inline long getPrgRAMSize() const;
    inline bool isTrainer() const;
    inline bool isPCROM() const;
} __attribute__ ((packed));

class Cartridge : public Addressable<word, byte>{//NES has 16bit addressing
    Header* head;
    byte* trainer;//if existent, 512 bytes copied to $7000 when the program begins
    byte* prgROM;//program ROM (contains the code and data sections of the program)
    byte* chrROM;//character ROM connected to the PPU
    byte* pcROM;//players choice ROM (if existent, 16 bytes); not normally accessable
    
    bool usingTrainer = false;
    
    void fill();
    
    std::ifstream file;

    byte& getCPU(word addr);
    byte& getPPU(word addr);
    byte& getPC(word addr);
public:
    Cartridge(std::string path);//create a cartridge from the file stream
    Cartridge(Header* header, std::string path);//create a cartride from a given header
    ~Cartridge();
    
    word getMinAddress(int mode=CPU);
    word getMaxAddress(int mode=CPU);

    byte& get(word addr, int mode=CPU);
    void useTrainer(bool b);

    RAM* ram;
};

//the representation of the hardware of the NES (cartridge, processor, etc)
class NES{
    Cartridge* cart;//the cartridge containing the ROM and additonal RAM for the system
    
public:
    Cartridge* getCartridge();
    void insertCartridge(Cartridge* newCart);
    bool validCartridge() const {return cart != nullptr;}

    NES(Cartridge* cart=nullptr);
};

#endif
