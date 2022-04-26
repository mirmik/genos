#ifndef GENOS_DRIVERS_SD_DEVICE_H
#define GENOS_DRIVERS_SD_DEVICE_H

#include <drivers/spi/spi_client.h>
#include <systime/systime.h>
#include <igris/util/crc.h>
#include <string.h>
#include "sdinfo.h"

class Sd2Card
{
	genos::spi_client * client;
	char rx[48];


	uint32_t block_;
	uint8_t chipSelectPin_;
	uint8_t errorCode_;
	uint8_t inBlock_;
	uint16_t offset_;
	uint8_t partialBlockRead_;
	uint8_t status_;
	uint8_t type_;

public:
	//Sd2Card(genos::spi_client* client) : client(client) {}

	uint8_t type(void) const
	{
		return type_;
	}

	void type(uint8_t value)
	{
		type_ = value;
	}

	void error(int code)
	{
		dprln("Error:", code);
	}



	/**
	   Read a cards CID register. The CID contains card identification
	   information such as Manufacturer ID, Product name, Product serial
	   number and Manufacturing date. */
	uint8_t readCID(cid_t* cid)
	{
		return readRegister(CMD10, cid);
	}
	/**
	   Read a cards CSD register. The CSD contains Card-Specific Data that
	   provides information regarding access to the card's contents. */
	uint8_t readCSD(csd_t* csd)
	{
		return readRegister(CMD9, csd);
	}

	char spiSend(uint8_t c = 0xFF)
	{
		char tx[1];
		char rx[1];

		tx[0] = c;
		client->exchange(tx, rx, 1);

		return rx[0];
	}

	char spiRec()
	{
		return spiSend(0xFF);
	}


	void begin(genos::spi_client * client)
	{
		this->client = client;
	}

	uint8_t init(genos::spi_client * client=nullptr)
	{
		if (client)
			begin(client);

		uint32_t arg;

		int64_t t0 = millis();

		client->lock_bus();
		client->select();

		// command to go idle in SPI mode
		while ((status_ = cardCommand(CMD0, 0)) != R1_IDLE_STATE)
		{
			unsigned int d = millis() - t0;
			if (d > SD_INIT_TIMEOUT)
			{
				error(SD_CARD_ERROR_CMD0);
				goto fail;
			}
		}
		// check SD version
		if ((cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND))
		{
			type(SD_CARD_TYPE_SD1);
		}
		else
		{
			// only need last byte of r7 response
			for (uint8_t i = 0; i < 4; i++)
			{
				status_ = spiRec();
			}
			if (status_ != 0XAA)
			{
				error(SD_CARD_ERROR_CMD8);
				goto fail;
			}
			type(SD_CARD_TYPE_SD2);
		}
		// initialize card and send host supports SDHC if SD2
		arg = type() == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

		while ((status_ = cardAcmd(ACMD41, arg)) != R1_READY_STATE)
		{
			// check for timeout
			unsigned int d = millis() - t0;
			if (d > SD_INIT_TIMEOUT)
			{
				error(SD_CARD_ERROR_ACMD41);
				goto fail;
			}
		}
		// if SD2 read OCR register to check for SDHC card
		if (type() == SD_CARD_TYPE_SD2)
		{
			if (cardCommand(CMD58, 0))
			{
				error(SD_CARD_ERROR_CMD58);
				goto fail;
			}
			if ((spiRec() & 0XC0) == 0XC0)
			{
				type(SD_CARD_TYPE_SDHC);
			}
			// discard rest of ocr - contains allowed voltage range
			for (uint8_t i = 0; i < 3; i++)
			{
				spiRec();
			}
		}

		dprln("initialize success");

		client->deselect();
		client->unlock_bus();
		return true;

fail:
		client->deselect();
		client->unlock_bus();
		return false;
	}

	uint8_t cardCommand(uint8_t cmd, uint32_t arg)
	{
		const int SIZE = 6 + 1 + 1;// + answer_size;
		//client->select();
		char tx[SIZE];
		char rx[SIZE];

		client->select();

		tx[0] = cmd | 0x40;
		tx[1] = (arg >> 24) & 0xFF;
		tx[2] = (arg >> 16) & 0xFF;
		tx[3] = (arg >> 8)  & 0xFF;
		tx[4] = (arg >> 0)  & 0xFF;
		tx[5] = mmc_crc7((const uint8_t*)tx, 5) << 1 | 1;

		memset(tx + 6, 0xFF, SIZE - 6);

		client->exchange(tx, rx, SIZE);
		//client->deselect();

		dprdump(tx, SIZE);
		dprdump(rx, SIZE);

		//memcpy(outrx, rx + 6 + 1, answer_size);

		return rx[SIZE - 1];
	}


	uint8_t cardAcmd(uint8_t cmd, uint32_t arg)
	{
		cardCommand(CMD55, 0);
		return cardCommand(cmd, arg);
	}

	//------------------------------------------------------------------------------
	/** read CID or CSR register */
	uint8_t readRegister(uint8_t cmd, void* buf)
	{
		uint8_t* dst = reinterpret_cast<uint8_t*>(buf);
		if (cardCommand(cmd, 0))
		{
			error(SD_CARD_ERROR_READ_REG);
			goto fail;
		}
		if (!waitStartBlock())
		{
			goto fail;
		}
		// transfer data
		for (uint16_t i = 0; i < 16; i++)
		{
			dst[i] = spiRec();
		}
		spiRec();  // get first crc byte
		spiRec();  // get second crc byte
		chipSelectHigh();
		return true;

fail:
		chipSelectHigh();
		return false;
	}


	//------------------------------------------------------------------------------
	/** Wait for start block token */
	uint8_t waitStartBlock(void)
	{
		unsigned int t0 = millis();
		while ((status_ = spiRec()) == 0XFF)
		{
			unsigned int d = millis() - t0;
			if (d > SD_READ_TIMEOUT)
			{
				error(SD_CARD_ERROR_READ_TIMEOUT);
				goto fail;
			}
		}
		if (status_ != DATA_START_BLOCK)
		{
			error(SD_CARD_ERROR_READ);
			goto fail;
		}
		return true;

fail:
		chipSelectHigh();
		return false;
	}

	void chipSelectHigh()
	{
		client->deselect();
	}

	void chipSelectLow()
	{
		client->select();
	}

	/**
	   Read a 512 byte block from an SD card device.

	   \param[in] block Logical block to be read.
	   \param[out] dst Pointer to the location that will receive the data.

	   \return The value one, true, is returned for success and
	   the value zero, false, is returned for failure.
	*/
	uint8_t readBlock(uint32_t block, uint8_t* dst)
	{
		return readData(block, 0, 512, dst);
	}

//------------------------------------------------------------------------------
	/** Skip remaining data in a block when in partial block read mode. */
	void readEnd(void)
	{
		if (inBlock_)
		{
			// skip data and crc
			while (offset_++ < 514)
			{
				spiRec();
			}
			chipSelectHigh();
			inBlock_ = 0;
		}
	}



	uint8_t readData(uint32_t block,
	                 uint16_t offset, uint16_t count, uint8_t* dst)
	{
		if (count == 0)
		{
			return true;
		}
		if ((count + offset) > 512)
		{
			goto fail;
		}
		if (!inBlock_ || block != block_ || offset < offset_)
		{
			block_ = block;
			// use address if not SDHC card
			if (type() != SD_CARD_TYPE_SDHC)
			{
				block <<= 9;
			}
			if (cardCommand(CMD17, block))
			{
				error(SD_CARD_ERROR_CMD17);
				goto fail;
			}
			if (!waitStartBlock())
			{
				goto fail;
			}
			offset_ = 0;
			inBlock_ = 1;
		}


		// skip data before offset
		for (; offset_ < offset; offset_++)
		{
			spiRec();
		}
		// transfer data
		for (uint16_t i = 0; i < count; i++)
		{
			dst[i] = spiRec();
		}

		offset_ += count;
		if (!partialBlockRead_ || offset_ >= 512)
		{
			// read rest of data, checksum and set chip select high
			readEnd();
		}
		return true;

fail:
		chipSelectHigh();
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	   Writes a 512 byte block to an SD card.

	   \param[in] blockNumber Logical block to be written.
	   \param[in] src Pointer to the location of the data to be written.
	   \param[in] blocking If the write should be blocking.
	   \return The value one, true, is returned for success and
	   the value zero, false, is returned for failure.
	*/
	uint8_t writeBlock(uint32_t blockNumber, const uint8_t* src, uint8_t blocking)
	{
#if SD_PROTECT_BLOCK_ZERO
		// don't allow write to first block
		if (blockNumber == 0)
		{
			error(SD_CARD_ERROR_WRITE_BLOCK_ZERO);
			goto fail;
		}
#endif  // SD_PROTECT_BLOCK_ZERO

		// use address if not SDHC card
		if (type() != SD_CARD_TYPE_SDHC)
		{
			blockNumber <<= 9;
		}
		if (cardCommand(CMD24, blockNumber))
		{
			error(SD_CARD_ERROR_CMD24);
			goto fail;
		}
		if (!writeData(DATA_START_BLOCK, src))
		{
			goto fail;
		}
		if (blocking)
		{
			// wait for flash programming to complete
			if (!waitNotBusy(SD_WRITE_TIMEOUT))
			{
				error(SD_CARD_ERROR_WRITE_TIMEOUT);
				goto fail;
			}
			// response is r2 so get and check two bytes for nonzero
			if (cardCommand(CMD13, 0) || spiRec())
			{
				error(SD_CARD_ERROR_WRITE_PROGRAMMING);
				goto fail;
			}
		}
		chipSelectHigh();
		return true;

fail:
		chipSelectHigh();
		return false;
	}

	//------------------------------------------------------------------------------
// send one block of data for write block or write multiple blocks
	uint8_t writeData(uint8_t token, const uint8_t* src)
	{
		spiSend(token);
		for (uint16_t i = 0; i < 512; i++)
		{
			spiSend(src[i]);
		}

		spiSend(0xff);  // dummy crc
		spiSend(0xff);  // dummy crc

		status_ = spiRec();
		if ((status_ & DATA_RES_MASK) != DATA_RES_ACCEPTED)
		{
			error(SD_CARD_ERROR_WRITE);
			chipSelectHigh();
			return false;
		}
		return true;
	}

	uint8_t waitNotBusy(unsigned int timeoutMillis)
	{
		unsigned int t0 = millis();
		unsigned int d;
		do
		{
			if (spiRec() == 0XFF)
			{
				return true;
			}
			d = millis() - t0;
		}
		while (d < timeoutMillis);
		return false;
	}


//------------------------------------------------------------------------------
	/** Check if the SD card is busy

	  \return The value one, true, is returned when is busy and
	   the value zero, false, is returned for when is NOT busy.
	*/
	uint8_t isBusy(void)
	{
		chipSelectLow();
		uint8_t b = spiRec();
		chipSelectHigh();

		return (b != 0XFF);
	}

};




#endif