#ifndef GENOS_I2C_H
#define GENOS_I2C_H

namespace genos
{
	// linux compatible
	class i2c_master
	{
	public:
		virtual int set_slave_address(uint8_t addr);

		virtual int write(const void* out, uint16_t olen) = 0;
		
		// combined transaction. it mixing reading and writing.
		virtual int read(void* in, uint16_t ilen) = 0;
	
		virtual int set_baudrate(int rate) = 0;
	}
}

#endif