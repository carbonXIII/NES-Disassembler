/*
 * NES.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: jordanrich
 */
#include "NES.h"
#include <stdexcept>

using namespace std;

Header::Header(istream& istr){
    istr.read((char*)this, 11);
    istr.ignore(5);
}

inline long Header::getPrgROMSize() const{
	return prgROMSize << 14;
}

inline long Header::getChrROMSize() const{
	return prgROMSize << 13;
}

inline long Header::getPrgRAMSize() const{
	return prgRAMSize << 13;
}

inline bool Header::isTrainer() const{
	return f6.b2();
}

inline bool Header::isPCROM() const{
	return f7.b1();
}

void Cartridge::fill(){//fill the Cartridge info based on Header info
    trainer = new byte[head->isTrainer()];
    prgROM = new byte[head->getPrgROMSize()];
    chrROM = new byte[head->getChrROMSize()];
    if(head->isPCROM())pcROM = new byte[16];
}

void Cartridge::useTrainer(bool b){
	usingTrainer = b;
}

word Cartridge::getMinAddress(int mode){
	if(mode == Addressable.CPU){
		return 0x8000;	
	}return 0;//PPU or PC
}

word Catridge::getMaxAddress(int mode){
	if(mode == Addressable.CPU){
		return getMinAddress(Addressable.CPU) + head->getPrgROMSize() - 1;
	}if(mode == Addresable.PPU){
		return head->getChrROMSize() - 1;
	}return (head->isPCROM()) ? 15 : 0;//PC
}

byte& Cartridge::getCPU(word addr){
	return prgROM[addr - getMinAddress(Addressable.CPU)];
}

byte& Cartridge::getPPU(word addr){
	return chrROM[addr];
}

byte& Cartridge::getPC(word addr){
	return pcROM[addr];
}

byte& Cartridge::get(word addr, int mode){
	if(getAddressOutOfBounds(addr, mode)) throw out_of_range("The address is out of range of the device.");
	
	if(m == Addressable.CPU){
		return getCPU(addr);
	}if(m == Addressable.PPU){
		return getPPU(addr);
	}return getPC(addr);
}

Cartridge::Cartridge(istream& istr){//create a cartridge from a given byte stream
	head = new Header(istr);
	fill();
}

Cartridge::Cartridge(Header* header){//create a cartride from a given header
	this->head = header;//save a pointer to the required header for future ref
	fill();
}

