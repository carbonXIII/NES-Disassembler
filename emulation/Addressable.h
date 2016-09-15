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
	enum mode {CPU, PPU, PC};

	Addressable() {};
	virtual ~Addressable() = 0;

	number& get(address addr, mode m=CPU) = 0;//return xvalue ref to memory at address
};

#endif /* EMULATION_ADDRESSABLE_H_ */
