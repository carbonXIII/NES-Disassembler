/*
 * NES.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: jordanrich
 */
#include "NES.h"
#include <stdexcept>

using namespace std;

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
    if(head->isTrainer()){
		trainer = new byte[512];
		file.read((char*)trainer, 512);
	}
	
    prgROM = new byte[head->getPrgROMSize()];
	file.read((char*)prgROM, head->getPrgROMSize());
    
	chrROM = new byte[head->getChrROMSize()];
	file.read((char*)chrROM, head->getChrROMSize());
    
	if(head->isPCROM()){
		pcROM = new byte[16];
		file.read((char*)pcROM, 8);
		if(!file.eof())file.read((char*)pcROM, 8);
	}
}

void Cartridge::useTrainer(bool b){
	usingTrainer = b;
}

word Cartridge::getMinAddress(int mode){
	if(mode == 0){//CPU
		return 0x8000;
	}return 0;//PPU or PC
}

word Cartridge::getMaxAddress(int mode){
	if(mode == 0){//CPU
		return getMinAddress(0) + head->getPrgROMSize() - 1;
	}if(mode == 1){//PPU
		return head->getChrROMSize() - 1;
	}return (head->isPCROM()) ? 15 : 0;//PC
}

byte& Cartridge::getCPU(word addr){
	return prgROM[addr - getMinAddress(0)];
}

byte& Cartridge::getPPU(word addr){
	return chrROM[addr];
}

byte& Cartridge::getPC(word addr){
	return pcROM[addr];
}

byte& Cartridge::get(word addr, int mode){
	if(addressOutOfBounds(addr, mode)) throw out_of_range("The address is out of range of the device.");
	
	if(mode == 0){
		return getCPU(addr);
	}if(mode == 1){
		return getPPU(addr);
	}return getPC(addr);
}

Cartridge::Cartridge(string path){//create a cartridge from a given byte stream
	file.open(path, ios::binary);

	//create a new header and fill it
	head = new Header();
	file.read((char*)head,11);
	file.ignore(5);

	fill();
}

Cartridge::Cartridge(Header* header, string path){//create a cartridge from a given header
	file.open(path, ios::binary);
	this->head = header;//save a pointer to the required header for future ref
	fill();
}

Cartridge::~Cartridge(){
	delete [] trainer;
	delete [] prgROM;
	delete [] chrROM;
	delete [] pcROM;
}

Cartridge* NES::getCartridge(){
	return cart;
}

void NES::insertCartridge(Cartridge* newCart){
	delete cart;
	cart = newCart;
}

NES::NES(Cartridge* cart){
	this->cart = cart;
}
