#ifndef GENOS_I2C_LINUX_H
#define GENOS_I2C_LINUX_H

namespace genos
{
	class linux_i2c_master : public i2c_master
	{
		int fd;

		int open(const char* path)
		{
			fd = open(path, O_RDWR);
			return fd >= 0 ? 0 : -1;
		}

		int set_baudrate(int rate) override
		{
			// unsuported for ioctl.
			return -1;
		}

		int set_slave_address()
		{
			if (ioctl(file, I2C_SLAVE, addr) < 0)
			{
				return -1;
			}
			return 0;
		}

		int write(const void* out, uint16_t olen) override
		{
			return write(fd, out, olen);
		}

		int read(void* in, uint16_t ilen) override
		{
			return read(fd, in, ilen);
		}
	};
}

#endif