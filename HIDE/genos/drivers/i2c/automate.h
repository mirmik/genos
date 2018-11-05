#ifndef GENOS_I2C_AUTOMATE_H
#define GENOS_I2C_AUTOMATE_H

namespace genos {
	namespace drivers {
		struct i2c_automate {
			/*enum state : public uint8_t {
				BusFail = 0x00,
				Start = 0x08,
				ReStart = 0x10,
				SLA_W_ACK = 0x18,
				SLA_W_NACK = 0x20,
				DAT_W_ACK = 0x28,
				DAT_W_NACK = 0x30,
				Collision = 0x38,
				SLA_R_ACK = 0x40,
				SLA_R_NACK = 0x48, 
				Recv_Byte = 0x50,
				Rec Byte+NACK = 0x58,
	
				/*0x68 Receive SLA+W LP 
				0x78 Receive SLA+W LP 
				0x60 Receive SLA+W Сид
				0x70 Receive SLA+W Bro
				0x80 Receive Byte & 0x
				0x88 Receive Last Byte
				0xA0 Receive ReStart О
				0xB0 Receive SLA+R LP 
				0xA8 Receive SLA+R Либ
				0xB8 Send Byte Receive
				0xC0 Send Last Byte Re
				0xC8 Send Last Byte Re*/
			//}

			enum status {
				Ok,
				Fault,
			};
		
			virtual void init_master(uint32_t baud) = 0;
			//virtual void start_write(uint8_t address, gxx::buffer buf) = 0;
			//virtual void start_read(uint8_t address, gxx::buffer buf);
			//void write(const char* data, size_t size);

			virtual void start_write(uint8_t target, char* data, size_t size);
			//virtual void start_read(uint8_t target, char* wdata, size_t wsize, char* rdata, size_t rsize);

			/*gxx::delegate<void, uint8_t> master_finish_handler;

			void set_master_finish_handler(gxx::delegate<void, uint8_t> dlg) {
				master_finish_handler = dlg;
			}

			void set_request_handler(gxx::delegate<void> dlg);*/
		};
	}
}

#endif