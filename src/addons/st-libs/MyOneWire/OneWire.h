#ifndef GENOS_ONEWIRE_
#define GENOS_ONEWIRE_

#include <hal/irq.h>
#include <asm/delay.h>
#include <periph/map.h>
#include <drivers/gpio/pin.h>

#define ONEWIRE_SEARCH 1
#define ONEWIRE_CRC 1

namespace drivers
{
	class OneWire
	{
		gpio_pin pin;


#if ONEWIRE_SEARCH
		// global search state
		unsigned char ROM_NO[8];
		uint8_t LastDiscrepancy;
		uint8_t LastFamilyDiscrepancy;
		bool LastDeviceFlag;
#endif

	public:
		OneWire(gpio_pin pin) : pin(pin) {}

		void init()
		{
			pin.mode(GPIO_MODE_INPUT);
		}

		void input_state() { pin.mode(GPIO_MODE_INPUT); }
		void output_state() { pin.mode(GPIO_MODE_OUTPUT); }
		//uint8_t readpin() { return pin.get(); }
		//void writehigh() { return pin.set(1); }
		//void writelow() { return pin.set(0); }

		uint8_t reset()
		{
			uint8_t r;
			uint8_t retries = 125;

			irqs_disable();
			input_state();
			irqs_enable();

			// wait until the wire is high... just in case
			do
			{
				if (--retries == 0) return 0;

				delay_us(2);//DelayuS(2);
			}
			while ( !pin.get() );

			irqs_disable();
			pin.set(0);
			output_state();	// drive output low
			irqs_enable();
			delay_us(500);
			irqs_disable();
			input_state();	// allow it to float
			delay_us(80);
			r = !pin.get();
			irqs_enable();
			delay_us(420);
			return r;
		}



		void write_bit(uint8_t v)
		{
			if (v & 1)
			{
				irqs_disable();
				pin.set(0);;
				output_state();	// drive output low
				delay_us(10);
				pin.set(1);;	// drive output high
				irqs_enable();
				delay_us(55);
			}
			else
			{
				irqs_disable();
				pin.set(0);;
				output_state();	// drive output low
				delay_us(65);
				pin.set(1);;	// drive output high
				irqs_enable();
				delay_us(5);
			}
		}

		uint8_t read_bit()
		{
			uint8_t r;

			irqs_disable();
			pin.set(0);;
			output_state();
			delay_us(3);
			input_state();	// let pin float, pull up will raise
			delay_us(10);
			r = pin.get();
			irqs_enable();
			delay_us(53);
			return r;
		}













		void write(uint8_t v, uint8_t parasite = 0)
		{
			uint8_t bitMask;

			for (bitMask = 0x01; bitMask; bitMask <<= 1)
			{
				write_bit((bitMask & v) ? 1 : 0);
			}

			if ( !parasite )
			{
				irqs_disable();
				input_state();
				pin.set(0);
				irqs_enable();
			}
		}

		void write_bytes(const uint8_t *buf, uint16_t count)
		{
			uint16_t i;

			for (i = 0 ; i < count ; i++)
				write(buf[i]);


			irqs_disable();
			input_state();
			pin.set(0);
			irqs_enable();

		}

		uint8_t read()
		{
			uint8_t bitMask;
			uint8_t r = 0;

			for (bitMask = 0x01; bitMask; bitMask <<= 1)
			{
				if ( read_bit()) r |= bitMask;
			}

			return r;
		}

		void read_bytes(uint8_t *buf, uint16_t count)
		{
			uint16_t i;

			for (i = 0 ; i < count ; i++)
				buf[i] = read();
		}





		void select(const uint8_t rom[8])
		{
			int i;

			write(0x55);           // Choose ROM

			for ( i = 0; i < 8; i++) write(rom[i]);
		}

		void skip()
		{
			write(0xCC);          // Skip ROM
		}

		void depower()
		{
			irqs_disable();
			input_state();
			irqs_enable();
		}





#if ONEWIRE_SEARCH
		// Clear the search state so that if will start from the beginning again.
		void reset_search();

		// Setup the search to find the device type 'family_code' on the next call
		// to search(*newAddr) if it is present.
		void target_search(uint8_t family_code);

		// Look for the next device. Returns 1 if a new address has been
		// returned. A zero might mean that the bus is shorted, there are
		// no devices, or you have already retrieved all of them.  It
		// might be a good idea to check the CRC to make sure you didn't
		// get garbage.  The order is deterministic. You will always get
		// the same devices in the same order.
		bool search(uint8_t *newAddr, bool search_mode = true);
#endif










#if ONEWIRE_CRC
		// Compute a Dallas Semiconductor 8 bit CRC, these are used in the
		// ROM and scratchpad registers.
		static uint8_t crc8(const uint8_t *addr, uint8_t len);

#if ONEWIRE_CRC16
		// Compute the 1-Wire CRC16 and compare it against the received CRC.
		// Example usage (reading a DS2408):
		//    // Put everything in a buffer so we can compute the CRC easily.
		//    uint8_t buf[13];
		//    buf[0] = 0xF0;    // Read PIO Registers
		//    buf[1] = 0x88;    // LSB address
		//    buf[2] = 0x00;    // MSB address
		//    WriteBytes(net, buf, 3);    // Write 3 cmd bytes
		//    ReadBytes(net, buf+3, 10);  // Read 6 data bytes, 2 0xFF, 2 CRC16
		//    if (!CheckCRC16(buf, 11, &buf[11])) {
		//        // Handle error.
		//    }
		//
		// @param input - Array of bytes to checksum.
		// @param len - How many bytes to use.
		// @param inverted_crc - The two CRC16 bytes in the received data.
		//                       This should just point into the received data,
		//                       *not* at a 16-bit integer.
		// @param crc - The crc starting value (optional)
		// @return True, iff the CRC matches.
		static bool check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc = 0);

		// Compute a Dallas Semiconductor 16 bit CRC.  This is required to check
		// the integrity of data received from many 1-Wire devices.  Note that the
		// CRC computed here is *not* what you'll get from the 1-Wire network,
		// for two reasons:
		//   1) The CRC is transmitted bitwise inverted.
		//   2) Depending on the endian-ness of your processor, the binary
		//      representation of the two-byte return value may have a different
		//      byte order than the two bytes you get from 1-Wire.
		// @param input - Array of bytes to checksum.
		// @param len - How many bytes to use.
		// @param crc - The crc starting value (optional)
		// @return The CRC16, as defined by Dallas Semiconductor.
		static uint16_t crc16(const uint8_t* input, uint16_t len, uint16_t crc = 0);
#endif
#endif









	};
}

/*#define BOOL uint8_t

#define FALSE 0
#define TRUE  1

// you can exclude onewire_search by defining that to 0
#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif

// Select the table-lookup method of computing the 8-bit CRC
// by setting this to 1.  The lookup table enlarges code size by
// about 250 bytes.  It does NOT consume RAM (but did in very
// old versions of OneWire).  If you disable this, a slower
// but very compact algorithm is used.
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 1
#endif

// You can allow 16-bit CRC checks by defining this to 1
// (Note that ONEWIRE_CRC must also be 1.)
#ifndef ONEWIRE_CRC16
#define ONEWIRE_CRC16 1
#endif

typedef struct{
	GPIO_TypeDef*         m_Port;
	__IO uint32_t*        m_Register;
	uint32_t              m_RegMask;
	uint32_t              m_InputMask;
	uint32_t              m_OutputMask;
	uint16_t              m_BitMask;
#ifdef ONEWIRE_SEARCH
	// global search state
	unsigned char ROM_NO[8];
	uint8_t LastDiscrepancy;
	uint8_t LastFamilyDiscrepancy;
	uint8_t LastDeviceFlag;
#endif
}OWire;

void OWInit(OWire* owire, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void OWInput(OWire* owire);
void OWOutput(OWire* owire);
uint8_t OWReadPin(OWire* owire);
void OWWriteHigh(OWire* owire);
void OWWriteLow(OWire* owire);


static void OWInterrupts(void);
static void OWNoInterrupts(void);

uint8_t OWReset(OWire* owire);

#ifdef ONEWIRE_SEARCH
void OWReset_search(OWire* owire);
uint8_t OWSearch(OWire* owire, uint8_t *newAddr);
#endif

void OWWrite_bit(OWire* owire, uint8_t v);

void OWWrite(OWire* owire, uint8_t v);

void OWWrite_bytes(OWire* owire, const uint8_t *buf, uint16_t count);

uint8_t OWRead(OWire* owire);

void OWRead_bytes(OWire* owire, uint8_t *buf, uint16_t count);

void OWSelect(OWire* owire, uint8_t rom[8]);

void OWSkip(OWire* owire);

void OWDepower(OWire* owire);

#ifdef ONEWIRE_CRC
uint8_t OWCrc8( uint8_t *addr, uint8_t len);
#ifdef ONEWIRE_CRC16
uint8_t OWCheck_crc16(uint8_t* input, uint16_t len, uint8_t* inverted_crc);
uint16_t OWCrc16(uint8_t* input, uint16_t len);
#endif

#endif*/

#endif
