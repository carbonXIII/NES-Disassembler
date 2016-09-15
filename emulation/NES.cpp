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

byte& Cartridge::get(word addr, mode m){

}

Cartridge::Cartridge(istream& istr){//create a cartridge from a given byte stream
	head = new Header(istr);
	fill();
}

Cartridge::Cartridge(Header* header){//create a cartride from a given header
	this->head = header;//save a pointer to the required header for future ref
	fill();
}

