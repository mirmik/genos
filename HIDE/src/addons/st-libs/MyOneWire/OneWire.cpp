#include <OneWire.h>


namespace drivers
{

#if ONEWIRE_SEARCH



//
// You need to use this function to start a search again from the beginning.
// You do not need to do it for the first search, though you could.
//
	void OneWire::reset_search()
	{
		// reset the search state
		LastDiscrepancy = 0;
		LastDeviceFlag = false;
		LastFamilyDiscrepancy = 0;

		for (int i = 7; ; i--)
		{
			ROM_NO[i] = 0;

			if ( i == 0) break;
		}
	}

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
//
	void OneWire::target_search(uint8_t family_code)
	{
		// set the search state to find SearchFamily type devices
		ROM_NO[0] = family_code;

		for (uint8_t i = 1; i < 8; i++)
			ROM_NO[i] = 0;

		LastDiscrepancy = 64;
		LastFamilyDiscrepancy = 0;
		LastDeviceFlag = false;
	}

//
// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
	bool OneWire::search(uint8_t *newAddr, bool search_mode /* = true */)
	{
		dprln("search");

		uint8_t id_bit_number;
		uint8_t last_zero, rom_byte_number;
		bool    search_result;
		uint8_t id_bit, cmp_id_bit;

		unsigned char rom_byte_mask, search_direction;

		// initialize for search
		id_bit_number = 1;
		last_zero = 0;
		rom_byte_number = 0;
		rom_byte_mask = 1;
		search_result = false;

		// if the last call was not the last one
		if (!LastDeviceFlag)
		{
			// 1-Wire reset
			if (!reset())
			{
				// reset the search
				LastDiscrepancy = 0;
				LastDeviceFlag = false;
				LastFamilyDiscrepancy = 0;
				return false;
			}

			// issue the search command
			if (search_mode == true)
			{
				write(0xF0);   // NORMAL SEARCH
			}
			else
			{
				write(0xEC);   // CONDITIONAL SEARCH
			}

			// loop to do the search
			do
			{
				// read a bit and its complement
				id_bit = read_bit();
				cmp_id_bit = read_bit();

				dprln("id_bit", id_bit);
				dprln("cmp_id_bit", cmp_id_bit);

				// check for no devices on 1-wire
				if ((id_bit == 1) && (cmp_id_bit == 1))
				{
					break;
				}
				else
				{
					// all devices coupled have 0 or 1
					if (id_bit != cmp_id_bit)
					{
						search_direction = id_bit;  // bit write value for search
					}
					else
					{
						// if this discrepancy if before the Last Discrepancy
						// on a previous next then pick the same as last time
						if (id_bit_number < LastDiscrepancy)
						{
							search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
						}
						else
						{
							// if equal to last pick 1, if not then pick 0
							search_direction = (id_bit_number == LastDiscrepancy);
						}

						// if 0 was picked then record its position in LastZero
						if (search_direction == 0)
						{
							last_zero = id_bit_number;

							// check for Last discrepancy in family
							if (last_zero < 9)
								LastFamilyDiscrepancy = last_zero;
						}
					}

					// set or clear the bit in the ROM byte rom_byte_number
					// with mask rom_byte_mask
					if (search_direction == 1)
						ROM_NO[rom_byte_number] |= rom_byte_mask;
					else
						ROM_NO[rom_byte_number] &= ~rom_byte_mask;

					// serial number search direction write bit
					write_bit(search_direction);

					// increment the byte counter id_bit_number
					// and shift the mask rom_byte_mask
					id_bit_number++;
					rom_byte_mask <<= 1;

					// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
					if (rom_byte_mask == 0)
					{
						rom_byte_number++;
						rom_byte_mask = 1;
					}
				}
			}
			while (rom_byte_number < 8); // loop until through all ROM bytes 0-7

			// if the search was successful then
			if (!(id_bit_number < 65))
			{
				// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
				LastDiscrepancy = last_zero;

				// check for last device
				if (LastDiscrepancy == 0)
				{
					LastDeviceFlag = true;
				}

				search_result = true;
			}
		}

		// if no device found then reset counters so next 'search' will be like a first
		if (!search_result || !ROM_NO[0])
		{
			LastDiscrepancy = 0;
			LastDeviceFlag = false;
			LastFamilyDiscrepancy = 0;
			search_result = false;
		}
		else
		{
			for (int i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
		}

		return search_result;
	}




#endif

































#if ONEWIRE_CRC
// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
//

#if ONEWIRE_CRC8_TABLE
// Dow-CRC using polynomial X^8 + X^5 + X^4 + X^0
// Tiny 2x16 entry CRC table created by Arjen Lentz
// See http://lentz.com.au/blog/calculating-crc-with-a-tiny-32-entry-lookup-table
static const uint8_t dscrc2x16_table[] =
{
	0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
	0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
	0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
	0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74
};

// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (Use tiny 2x16 entry CRC table)
uint8_t OneWire::crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--)
	{
		crc = *addr++ ^ crc;  // just re-using crc as intermediate
		crc = dscrc2x16_table[crc & 0x0f] ^
		      dscrc2x16_table[16 + ((crc >> 4) & 0x0f)];
	}

	return crc;
}
#else
//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but a little smaller, than the lookup table.
//
uint8_t OneWire::crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--)
	{
		uint8_t inbyte = *addr++;

		for (uint8_t i = 8; i; i--)
		{
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;

			if (mix) crc ^= 0x8C;

			inbyte >>= 1;
		}
	}

	return crc;
}
#endif

#if ONEWIRE_CRC16
bool OneWire::check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc)
{
	crc = ~crc16(input, len, crc);
	return (crc & 0xFF) == inverted_crc[0] && (crc >> 8) == inverted_crc[1];
}

uint16_t OneWire::crc16(const uint8_t* input, uint16_t len, uint16_t crc)
{
#if defined(__AVR__)

	for (uint16_t i = 0 ; i < len ; i++)
	{
		crc = _crc16_update(crc, input[i]);
	}

#else
	static const uint8_t oddparity[16] =
	{ 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

	for (uint16_t i = 0 ; i < len ; i++)
	{
		// Even though we're just copying a byte from the input,
		// we'll be doing 16-bit computation with it.
		uint16_t cdata = input[i];
		cdata = (cdata ^ crc) & 0xff;
		crc >>= 8;

		if (oddparity[cdata & 0x0F] ^ oddparity[cdata >> 4])
			crc ^= 0xC001;

		cdata <<= 6;
		crc ^= cdata;
		cdata <<= 1;
		crc ^= cdata;
	}

#endif
	return crc;
}
#endif

#endif













}
