#include <igris/protocols/numcmd/numcmd.h>
#include <igris/util/hexascii.h>

#include <errno.h>
#include <stdlib.h>

int numcmd_checksum_check(const char* str)
{
	uint8_t sum = 0;
	uint8_t csum;
	
	for (int i = 1; i < 11; i++)
	{
		sum += *(str + i);
	};

	csum = hex_to_uint8(str + 11);

	if (sum != csum) 
		return 0;
	
	return 1;
}

int numcmd_do(const char* str, struct numcmd_record* table, uint32_t* result) 
{
	struct numcmd_record* it;
	uint8_t code;

	code = hex_to_uint8(str + 1);

	it = table;
	for (;it->func != NULL; ++it) 
	{
		if (code == it->num) 
		{
			*result = it->func(hex_to_uint32(str + 3));
			return 0;
		}
	}

	return ENOENT;
}

void numcmd_form_checksum(char* str) 
{
	uint8_t sum = 0;

	for (int i = 1; i < 11; i++)
	{
		sum += *(str + i);
	};	

	uint8_to_hex(str + 11, sum);
}
