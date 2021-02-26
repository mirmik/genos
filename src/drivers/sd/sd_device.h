#ifndef GENOS_DRIVERS_SD_DEVICE_H
#define GENOS_DRIVERS_SD_DEVICE_H

#include <drivers/spi/spi_client.h>
#include <systime/systime.h>

#include <igris/util/crc.h>

#include <string.h>

// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
uint8_t const CMD0 = 0X00;
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
uint8_t const CMD8 = 0X08;
/** SEND_CSD - read the Card Specific Data (CSD register) */
uint8_t const CMD9 = 0X09;
/** SEND_CID - read the card identification information (CID register) */
uint8_t const CMD10 = 0X0A;
/** SEND_STATUS - read the card status register */
uint8_t const CMD13 = 0X0D;
/** READ_BLOCK - read a single data block from the card */
uint8_t const CMD17 = 0X11;
/** WRITE_BLOCK - write a single data block to the card */
uint8_t const CMD24 = 0X18;
/** WRITE_MULTIPLE_BLOCK - write blocks of data until a STOP_TRANSMISSION */
uint8_t const CMD25 = 0X19;
/** ERASE_WR_BLK_START - sets the address of the first block to be erased */
uint8_t const CMD32 = 0X20;
/** ERASE_WR_BLK_END - sets the address of the last block of the continuous
    range to be erased*/
uint8_t const CMD33 = 0X21;
/** ERASE - erase all previously selected blocks */
uint8_t const CMD38 = 0X26;
/** APP_CMD - escape for application specific command */
uint8_t const CMD55 = 0X37;
/** READ_OCR - read the OCR register of a card */
uint8_t const CMD58 = 0X3A;
/** SET_WR_BLK_ERASE_COUNT - Set the number of write blocks to be
     pre-erased before writing */
uint8_t const ACMD23 = 0X17;
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
uint8_t const ACMD41 = 0X29;
//------------------------------------------------------------------------------
/** status for card in the ready state */
uint8_t const R1_READY_STATE = 0X00;
/** status for card in the idle state */
uint8_t const R1_IDLE_STATE = 0X01;
/** status bit for illegal command */
uint8_t const R1_ILLEGAL_COMMAND = 0X04;
/** start data token for read or write single block*/
uint8_t const DATA_START_BLOCK = 0XFE;
/** stop token for write multiple blocks*/
uint8_t const STOP_TRAN_TOKEN = 0XFD;
/** start data token for write multiple blocks*/
uint8_t const WRITE_MULTIPLE_TOKEN = 0XFC;
/** mask for data response tokens after a write block operation */
uint8_t const DATA_RES_MASK = 0X1F;
/** write data accepted token */
uint8_t const DATA_RES_ACCEPTED = 0X05;







namespace genos
{
	class sd_device
	{
		spi_client * client;

	public:
		sd_device(spi_client* client) : client(client) {}

		char spiSend(uint8_t c)
		{
			char tx[1];
			char rx[1];

			tx[0] = 0xFF;
			client->exchange(tx, rx, 1);

			return rx[0];
		}


		void begin(spi_client * client)
		{
			this->client = client;
		}

		/*void _command_send_part(uint8_t cmd, uint32_t arg)
		{
			char tx[6];
			char rx[6];

			tx[0] = cmd | 0x40;
			tx[1] = (arg >> 24) & 0xFF;
			tx[2] = (arg >> 16) & 0xFF;
			tx[3] = (arg >> 8)  & 0xFF;
			tx[4] = (arg >> 0)  & 0xFF;
			tx[5] = mmc_crc7((const uint8_t*)tx, 5) << 1 | 1;

			client->exchange(tx, rx, 6);
		}

		uint8_t _command_recv_part()
		{
			char tx[2];
			char rx[2];

			tx[0] = 0xFF;
			tx[1] = 0xFF;

			client->exchange(tx, rx, 2);

			return tx[1];
		}*/

		uint8_t command(uint8_t cmd, uint32_t arg)
		{
			const int SIZE = 6 + 1 + 6;
			client->select();
			char tx[SIZE];
			char rx[SIZE];

			tx[0] = cmd | 0x40;
			tx[1] = (arg >> 24) & 0xFF;
			tx[2] = (arg >> 16) & 0xFF;
			tx[3] = (arg >> 8)  & 0xFF;
			tx[4] = (arg >> 0)  & 0xFF;
			tx[5] = mmc_crc7((const uint8_t*)tx, 5) << 1 | 1;

			memset(tx + 6, 0xFF, SIZE - 6);

			client->exchange(tx, rx, SIZE);
			client->deselect();

			dprdump(tx, SIZE);
			dprdump(rx, SIZE);

			return rx[7];
		}


		uint8_t card_acmd(uint8_t cmd, uint32_t arg)
		{
			command(CMD55, 0);
			return command(cmd, arg);
		}

		uint8_t command_long(uint8_t cmd, uint32_t arg)
		{
			const int SIZE = 6 + 1 + 17;
			client->select();
			char tx[SIZE];
			char rx[SIZE];

			tx[0] = cmd | 0x40;
			tx[1] = (arg >> 24) & 0xFF;
			tx[2] = (arg >> 16) & 0xFF;
			tx[3] = (arg >> 8)  & 0xFF;
			tx[4] = (arg >> 0)  & 0xFF;
			tx[5] = mmc_crc7((const uint8_t*)tx, 5) << 1 | 1;

			memset(tx + 6, 0xFF, SIZE - 6);

			client->exchange(tx, rx, SIZE);
			client->deselect();

			dprdump(tx, SIZE);
			dprdump(rx, SIZE);

			return rx[7];
		}

		void test()
		{
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
			chipSelectHigh();



			client->unlock_bus();
		}

		/*uint8_t readCID(cid_t* cid)
		{
			return readRegister(CMD10, cid);
		}*/


		uint8_t cardCommand(uint8_t cmd, uint32_t arg)
		{
			// end read if in partialBlockRead mode
			readEnd();

			// select card
			chipSelectLow();

			// wait up to 300 ms if busy
			waitNotBusy(300);

			// send command
			spiSend(cmd | 0x40);

			// send argument
			for (int8_t s = 24; s >= 0; s -= 8)
			{
				spiSend(arg >> s);
			}

			// send CRC
			uint8_t crc = 0XFF;
			if (cmd == CMD0)
			{
				crc = 0X95;  // correct crc for CMD0 with arg 0
			}
			if (cmd == CMD8)
			{
				crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
			}
			spiSend(crc);

			// wait for response
			for (uint8_t i = 0; ((status_ = spiRec()) & 0X80) && i != 0XFF; i++)
				;
			return status_;
		}




		void spiSend(uint8_t data)
		{
			// no interrupts during byte send - about 8 us
			cli();
			for (uint8_t i = 0; i < 8; i++)
			{
				fastDigitalWrite(SPI_SCK_PIN, LOW);

				fastDigitalWrite(SPI_MOSI_PIN, data & 0X80);

				data <<= 1;

				fastDigitalWrite(SPI_SCK_PIN, HIGH);
			}
			// hold SCK high for a few ns
			nop;
			nop;
			nop;
			nop;

			fastDigitalWrite(SPI_SCK_PIN, LOW);
			// enable interrupts
			sei();
		}


		uint8_t cardCommand(uint8_t cmd, uint32_t arg)
		{
			// end read if in partialBlockRead mode
			readEnd();

			// select card
			chipSelectLow();

			// wait up to 300 ms if busy
			waitNotBusy(300);

			// send command
			spiSend(cmd | 0x40);

			// send argument
			for (int8_t s = 24; s >= 0; s -= 8)
			{
				spiSend(arg >> s);
			}

			// send CRC
			uint8_t crc = 0XFF;
			if (cmd == CMD0)
			{
				crc = 0X95;  // correct crc for CMD0 with arg 0
			}
			if (cmd == CMD8)
			{
				crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
			}
			spiSend(crc);

			// wait for response
			for (uint8_t i = 0; ((status_ = spiRec()) & 0X80) && i != 0XFF; i++)
				;
			return status_;
		}

	};
}




#endif