/*
 * Addressable.h
 *
 *  Created on: Sep 14, 2016
 *      Author: jordanrich
 */

#ifndef EMULATION_ADDRESSABLE_H_
#define EMULATION_ADDRESSABLE_H_

#include "../tools.h"

template<typename address, typename number>
class Addressable {
public:
	enum {CPU, PPU, PC};

	Addressable() {};
	virtual ~Addressable() = 0;

	number& get(address addr, int mode=CPU) = 0;//return xvalue ref to memory at address
	
	address getMinAdress(int mode=CPU) = 0;//returns the minimum valid address value for the given mode
	address getMaxAddress(int mode=PPU) = 0;//max valid address
	bool addressOutOfBounds(address addr, int mode=CPU){//checks if the given address is out of bounds (in the given mode)
		return getMinAddress(m) > addr || addr > getMaxAddress(m);
	}
};

#endif /* EMULATION_ADDRESSABLE_H_ */
