/*
	gpio_expander - An attemp to create a fast and universal library for drive many GPIO chips
	
	Features:
	
	- Can drive many type of GPIO's chip from different makers.
	- Use almost same commands for all chips so they can be easily exchanged.
	- Can access special registers of some chip.

    Copyright (c) 2013-2014, sumotoy, coded by Max MC Costa.    

    gpio_expander Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gpio_expander Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

	
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:0.8b3 - Fixed an error that blocks I/O 16 in many functions
	(thanks Thorsten to point me this)
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


#ifndef _GPIO_EXPANDER_H_
#define _GPIO_EXPANDER_H_

#include <inttypes.h>
#include <Arduino.h>


#if defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0 or 3.1
	#define __FASTWRITE
#endif
class gpio_expander {

/*public:
	gpio_expander( );

	virtual void 	begin(bool protocolInitOverride=false) = 0;
	
	//void 			command();								

	
protected:
	inline uint16_t		uint8_t2word(uint8_t high_uint8_t,uint8_t low_uint8_t){return (word)high_uint8_t << 8 | (word)low_uint8_t;};
	inline uint8_t			word2highuint8_t(uint16_t data){return (uint8_t)(data >> 8);};
	inline uint8_t			word2lowuint8_t(uint16_t data){return (uint8_t)(data & 0x00FF);};

private:
*/
};
#endif