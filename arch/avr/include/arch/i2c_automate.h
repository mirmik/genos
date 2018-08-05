#ifndef GENOS_AVR_ARCH_I2C_H
#define GENOS_AVR_ARCH_I2C_H

//#include <periph/map.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//0x00 Bus Fail Автобус сломался… эээ в смысле аппаратная ошибка шины. Например, внезапный старт посреди передачи бита.
//0x08 Start Был сделан старт. Теперь мы решаем что делать дальше, например послать адрес ведомого
//0x10 ReStart Был обнаружен повторный старт. Можно переключиться с записи на чтение или наоборот. От логики зависит.
//0x18 SLA+W+ACK Мы отправили адрес с битом записи, а в ответ получили ACK от ведомого. Значит можно продолжать.
//0x20 SLA+W+NACK Мы отправили адрес с битом записи, а нас послали NACK. Обидно, сгенерим ошибку или повторим еще раз.
//0x28 Byte+ACK Мы послали байт и получили подтверждение, что ведомый его принял. Продолжаем.
//0x30 Byte+NACK Мы послали байт, но подтверждение не получили. Видимо ведомый уже сыт по горло нашими подачками или он захлебнулся в данных. Либо его ВНЕЗАПНО посреди передачи данных украли инопланетяне.
//0x38 Collision А у нас тут клановые разборки — пришел другой мастер, по хамски нас перебил, да так, что мы от возмущения аж заткнулись. Ничего I’l be back! До встречи через n тактов!
//0x40 SLA+R+ACK Послали адрес с битом на чтение, а ведомый отозвался. Хорошо! Будем читать.
//0x48 SLA+R+NACK Крикнули в шину «Эй ты, с адресом ХХХ, почитай нам сказки» А в ответ «Иди NACK!» В смысле на запрос адреса с битом чтения никто не откликнулся. Видимо не хотят или заняты. Также может быть никого нет дома.
//0x50 Receive Byte Мы приняли байт. И думаем что бы ответить ведомому. ACK или NACK.
//0x58 Receive Byte+NACK Мы приняли байт от ведомого и сказали ему «иди NACK!» И он обиженый ушел, освободив шину.

//0x68 Receive SLA+W LP Мы были мастером, трепались с подчиненными по шине. И тут появляется на шине другой, более равный, мастер, перебивает нас и молвит «Уважаемый ХХ, а не возьмете ли вы вот эти байтики…» Чтож, он круче. Придется бросать передачу и брать его байты себе.
//0x78 Receive SLA+W LP Broadcast Были мы, как нам казалось, самыми крутыми мастерами на шине. Пока не появился другой мастер и перебив нас прогундосил на всю шину «Эй, слышь тыыы. Слушай сюда» Девайсы-лохи, с неотключенными широковещательными запросами подчиняются. Остальные отмораживаются и всякое Broadcast-быдло игнорируют.
//0x60 Receive SLA+W Сидим на шине, никого не трогаем, ни с кем не общаемся. А тут нас по имени… Конечно отзовемся :)
//0x70 Receive SLA+W Broadcast Ситуация повторяется, но на этот раз слышим уже знакомое нам «Слышь, тыыыы». Кто? К кому? Игнорируем Broadcast запросы? Или нет? Зависит от моральных качеств программы.
//0x80 Receive Byte & 0x90 Receive Byte Broadcast Принимаем байты. От кого и в каком виде не важно. Решаем что сказать Давай еще (ACK) или «Иди NACK». Тут уже по обстоятельствам.
//0x88 Receive Last Byte & 0x98 Receive Last Byte Broadcast Приняли последний байт и распихиваем по карманам.
//0xA0 Receive ReStart Ой у нас Повторный старт. Видимо то что пришло в первый раз был таки адрес страницы. А сейчас пойдут данные…
//0xB0 Receive SLA+R LP Слали мы что то слали, а тут нас перебивает другой мастер, обращается по имени и говорит «А ну ХХ зачитай нам что нибудь из Пушкина» Что делать, приходится читать.
//0xA8 Receive SLA+R Либо просто к нам какой то другой мастер по имени обращается и просить ему передать байтиков.
//0xB8 Send Byte Receive ACK Ну дали мы ему байт. Он нам ACK. А мы тем временем думаем слать ему еще один (последний) и говорить «иди NACK». Или же у нас дофига их и можно еще пообщаться.
//0xC0 Send Last Byte Receive NACK Дали мастеру последний имеющийся байт, а он нам «иди NACK». Хамло. Ну и хрен с ним. Уходим с шины.
//0xC8 Send Last Byte Receive ACK Дали мастеру последний имеющийся байт, а он требует еще. Но у нас нету, так что разворачиваемся и уходим с шины. А он пусть карманы воздухом наполняет (в этот момент мастер начнет получать якобы от slave 0xFF байты, на самом деле это просто чтение висящей шины).

#include <gxx/debug/dprint.h>
#include <gxx/buffer.h>
#include <gxx/panic.h>

#include <genos/hal/i2c.h>
#include <periph/irqdefs.h>


namespace arch {
	class i2c_automate : public genos::hal::i2c_automate {
	public:
		gxx::delegate<void> error_handler;
		gxx::delegate<void> operation_finish_handler;

		enum Type {
			i2c_sawp = 0b00,
			i2c_sarp = 0b01,
			i2c_sawsarp = 0b10,
		};

		//static constexpr uint8_t TYPEMASK = 0b00000111;
		//static constexpr uint8_t ERR_BF = 0b10000000;
		//static constexpr uint8_t ERR_NA = 0b01000000;

		uint8_t target_address;
		gxx::buffer sendbuf;
		gxx::buffer recvbuf;
		size_t it;
		int rbytecount;

		union {
			uint8_t flags;
			struct {
				uint8_t type : 2;
				uint8_t ERR_BF : 1;
				uint8_t ERR_NA : 1;
				uint8_t ERR_NK : 1;
				uint8_t slave_mode:1;
			};
		};
		/*uint8_t mode;

		uint8_t i2c_Do;								// Переменная состояния передатчика IIC
		uint8_t i2c_InBuff[i2c_MasterBytesRX];		// Буфер прием при работе как Slave
		uint8_t i2c_OutBuff[i2c_MasterBytesTX];		// Буфер передачи при работе как Slave
		uint8_t i2c_SlaveIndex;						// Индекс буфера Slave
		
		
		uint8_t i2c_Buffer[i2c_MaxBuffer];			// Буфер для данных работы в режиме Master
		uint8_t i2c_index;							// Индекс этого буфера
		uint8_t i2c_ByteCount;						// Число байт передаваемых
		
		uint8_t i2c_SlaveAddress;						// Адрес подчиненного
		
		uint8_t i2c_PageAddress[i2c_MaxPageAddrLgth];	// Буфер адреса страниц (для режима с sawsarp)
		uint8_t i2c_PageAddrIndex;						// Индекс буфера адреса страниц
		uint8_t i2c_PageAddrCount;						// Число байт в адресе страницы для текущего Slave
*/
		void init_master(uint32_t scl_freq_hz = 100000) {
			/* twi bit rate formula from atmega128 manual pg 204
  			SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
  			note: TWBR should be 10 or higher for master mode
  			It is 72 for a 16mhz Wiring board with 100kHz TWI */
			TWBR = ((F_CPU / scl_freq_hz) - 16) / 2;
			TWSR = 0x00;
			genos::hal::irqtbl::set_handler(ATMEGA_IRQ_TWI, gxx::fastaction(&i2c_automate::handler, this));
			dprln("i2c init for master mode with TWBR:", TWBR, "(freq:", (uint32_t)scl_freq_hz, ')');
		}

		void enable() {
			TWCR |= 1<<TWIE | 1<<TWEN;
		}

		void start_write(uint8_t target, gxx::buffer buf) {
			sendbuf = buf;
			target_address = target << 1;
			it = 0;
			type = i2c_sawp;

			TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE; 
		}

		void start_read(uint8_t target, gxx::buffer wbuf, gxx::buffer rbuf, int readBytes) {
			recvbuf = rbuf;
			sendbuf = wbuf;
			rbytecount = readBytes;
			
			target_address = target << 1;
			it = 0;
			type = i2c_sawsarp;

			TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE; 
		}

		void handler() {
			uint8_t code = TWSR;
			
			dpr("i2c handler "); 
			dprhexln(code);
			
			switch(code & 0xF8)	{ // Отсекаем биты прескалера
				case 0x00: {	// Bus Fail (автобус сломался)
					ERR_BF = 1;
					TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
					error_handler();
					break;
				}
			
				case 0x08: { // Старт был, а затем мы:
					if ( type == i2c_sarp ) {	// В зависимости от режима
						//dprln("ADDR+R"); 
						TWDR = target_address | 0x01;		// Шлем Addr+R
					}
					else {									// Или
						//dprln("ADDR+W"); 
						TWDR = target_address & 0xFE;		// Шлем Addr+W
					}
					TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
					break;
				}
							
				case 0x10: { // Повторный старт был, а затем мы
					if ( type == i2c_sawsarp ) {	// В зависимости от режима
						TWDR = target_address | 0x01;		// Шлем Addr+R
					}
					else {									// Или 
						gxx::panic("strange if??? TODO");
						TWDR = target_address & 0xFE;		// Шлем Addr+W
					}
			
					// To Do: Добавить сюда обработку ошибок 
					TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
					break;
				}
			
				case 0x18: { // Был послан SLA+W получили ACK, а затем:
					if( type == i2c_sawp || type == i2c_sawsarp) { // В зависимости от режима
						TWDR = sendbuf[it++]; // Шлем байт данных, увеличиваем указатель буфера
						TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  // Go! 
					}
			
					//if( type == i2c_sawsarp )	{
						//TWDR = i2c_PageAddress[i2c_PageAddrIndex];					// Или шлем адрес странцы (по сути тоже байт данных)
						//i2c_PageAddrIndex++;										// Увеличиваем указатель буфера страницы
						//TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;	// Go!
					//}	 
				}
				break;
				
				case 0x20: {	// Был послан SLA+W получили NACK - слейв либо занят, либо его нет дома.
					ERR_NA = 1; // Код ошибки
					TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;	// Шлем шине Stop
					error_handler();		 												// Обрабатываем событие ошибки;
					break;
				}
			
				case 0x28: { // Байт данных послали, получили ACK!  (если sawp - это был байт данных. если sawsarp - байт адреса страницы)
					// А дальше: 
					if( type == i2c_sawp ) {// В зависимости от режима
						if (it == sendbuf.size()) { // Если был байт данных последний											
							TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;	// Шлем Stop
							operation_finish_handler();												// И выходим в обработку стопа
						}
						else {
							TWDR = sendbuf[it++];												// Либо шлем еще один байт
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
						}
					}
			
					if( type == i2c_sawsarp )	{					// В другом режиме мы
						if (it == sendbuf.size()) { // Если был байт данных последний											
							it = 0;
							TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;		// Запускаем Повторный старт!
						}
						else {
							TWDR = sendbuf[it++];												// Либо шлем еще один байт
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
						}
					}	 
					break;
				}
				
				case 0x30: { //Байт ушел, но получили NACK причин две. 1я передача оборвана слейвом и так надо. 2я слейв сглючил.
					ERR_NK = 1;				// Запишем статус ошибки. Хотя это не факт, что ошибка. 
					TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;	// Шлем Stop
					error_handler(); // Отрабатываем событие выхода
					break;
				}
			/*
				case 0x38:	//  Коллизия на шине. Нашелся кто то поглавней
						{
						i2c_Do |= i2c_ERR_LP;			// Ставим ошибку потери приоритета
			
						// Настраиваем индексы заново. 
						i2c_index = 0;
						i2c_PageAddrIndex = 0;
			
						TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;		// Как только шина будет свободна 
						break;																		// попробуем передать снова.
						}*/
			
				case 0x40: { // Послали SLA+R получили АСК. А теперь будем получать байты
					if ( it == rbytecount ) { // Если буфер кончится на этом байте, то 
						TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;	// Требуем байт, а в ответ потом пошлем NACK(Disconnect)
					}															// Что даст понять слейву, что мол хватит гнать. И он отпустит шину
					else {
						TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;	// Или просто примем байт и скажем потом ACK
					}
			
					break;
				}
			
				case 0x48: { // Послали SLA+R, но получили NACK. Видать slave занят или его нет дома. 
					ERR_NA = 1; // Код ошибки No Answer
					TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;	// Шлем Stop
					error_handler(); // Отрабатываем выходную ситуацию ошибки
					break;
				}
				
			
				case 0x50: { // Приняли байт.		 
					recvbuf[it++] = TWDR;			// Забрали его из буфера
					// To Do: Добавить проверку переполнения буфера. А то мало ли что юзер затребует
			
					if ( it+1 == rbytecount ) { // Если остался еще один байт из тех, что мы хотели считать
						TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;		// Затребываем его и потом пошлем NACK (Disconnect)
					}
					else {
						TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;		// Если нет, то затребываем следующий байт, а в ответ скажем АСК
					}
					break;
				}
			
				case 0x58: { // Вот мы взяли последний байт, сказали NACK слейв обиделся и отпал. 
					recvbuf[it] = TWDR;													// Взяли байт в буфер
					TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;			// Передали Stop
					operation_finish_handler();														// Отработали точку выхода
					break;
				}
/*			
// I			IC  Slave ============================================================================
			
				case 0x68:	// RCV SLA+W Low Priority							// Словили свой адрес во время передачи мастером
				case 0x78:	// RCV SLA+W Low Priority (Broadcast)				// Или это был широковещательный пакет. Не важно
						{
						i2c_Do |= i2c_ERR_LP | i2c_Interrupted;					// Ставим флаг ошибки Low Priority, а также флаг того, что мастера прервали
						
						// Restore Trans after.
						i2c_index = 0;											// Подготовили прерваную передачу заново
						i2c_PageAddrIndex = 0;
						}														// И пошли дальше. Внимание!!! break тут нет, а значит идем в "case 60"
			
				case 0x60: // RCV SLA+W  Incoming?								// Или просто получили свой адрес
				case 0x70: // RCV SLA+W  Incoming? (Broascast)					// Или широковещательный пакет
						{
			
						i2c_Do |= i2c_Busy;										// Занимаем шину. Чтобы другие не совались
						i2c_SlaveIndex = 0;										// Указатель на начало буфера слейва, Неважно какой буфер. Не ошибемся
			
						if (i2c_MasterBytesRX == 1)								// Если нам суждено принять всего один байт, то готовимся принять  его
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;			// Принять и сказать пошли все н... NACK!
							}
						else
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// А если душа шире чем один байт, то сожрем и потребуем еще ACK!
							}
						break;
						}
			
				case 0x80:	// RCV Data Byte									// И вот мы приняли этот байт. Наш или широковещательный. Не важно
				case 0x90:	// RCV Data Byte (Broadcast)
						{
						i2c_InBuff[i2c_SlaveIndex] = TWDR;						// Сжираем его в буфер.
						
						i2c_SlaveIndex++;										// Сдвигаем указатель
			
						if (i2c_SlaveIndex == i2c_MasterBytesRX-1) 				// Свободно место всего под один байт? 
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;			// Приянть его и сказать NACK!
							}
						else 
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Места еще дофига? Принять и ACK!
							}
						break;
						} 
			
				case 0x88: // RCV Last Byte										// Приянли последний байт
				case 0x98: // RCV Last Byte (Broadcast)
						{
						i2c_InBuff[i2c_SlaveIndex] = TWDR;						// Сожрали его в буфер
						
						if (i2c_Do & i2c_Interrupted)							// Если у нас был прерываный сеанс от имени мастера
							{
							TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Влепим в шину свой Start поскорей и сделаем еще одну попытку
							}
						else
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Если не было такого факта, то просто отвалимся и будем ждать
							}
			
						MACRO_i2c_WhatDo_SlaveOut												// И лениво отработаем наш выходной экшн для слейва
						break;
						}
			
				
				case 0xA0: // Ой, мы получили Повторный старт. Но чо нам с ним делать? 
						{
						// Можно, конечно, сделать вспомогательный автомат, чтобы обрабатывать еще и адреса внутренних страниц, подобно еепромке. 
						// Но я не стал заморачиваться. В этом случае делается это тут.
						TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// просто разадресуемся, проигнорировав этот посыл	
						break;
						}
			
			
			
				case 0xB0:  // Поймали свой адрес на чтение во время передачи Мастером			
						{
						i2c_Do |= i2c_ERR_LP | i2c_Interrupted;			// Ну чо, коды ошибки и флаг прерваной передачи.

						// Восстанавливаем индексы
						i2c_index = 0;
						i2c_PageAddrIndex = 0;
						}												// Break нет! Идем дальше						
			
				case 0xA8:	// // Либо просто словили свой адрес на чтение
						{
						i2c_SlaveIndex = 0;								// Индексы слейвовых массивов на 0
			
						TWDR = i2c_OutBuff[i2c_SlaveIndex];				// Чтож, отдадим байт из тех что есть.
						
						if(i2c_MasterBytesTX == 1)
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;	// Если он последний, мы еще на NACK в ответ надеемся 
							}
						else
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;	// А если нет, то  ACK ждем
							}
			
						break;
						} 
						
			
				case 0xB8: // Послали байт, получили ACK
						{			
						i2c_SlaveIndex++;								// Значит продолжаем дискотеку. Берем следующий байт
						TWDR = i2c_OutBuff[i2c_SlaveIndex];				// Даем его мастеру
			
						if (i2c_SlaveIndex == i2c_MasterBytesTX-1)		// Если он последний был, то
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;	// Шлем его и ждем NACK
							}
						else
							{
							TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|0<<TWEN|1<<TWIE;	// Если нет, то шлем и ждем ACK
							}
			
						break;
						}
			
				case 0xC0: // Мы выслали последний байт, больше у нас нет, получили NACK
				case 0xC8: // или ACK. В данном случае нам пох. Т.к. больше байтов у нас нет.
					{
					if (i2c_Do & i2c_Interrupted)									// Если там была прерваная передача мастера
						{															// То мы ему ее вернем
						i2c_Do &= i2c_NoInterrupted;								// Снимем флаг прерваности
						TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;	// Сгенерим старт сразу же как получим шину.
						}
					else
						{
						TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;	// Если мы там одни, то просто отдадим шину
						}
						MACRO_i2c_WhatDo_SlaveOut									// И отработаем выход слейва. Впрочем, он тут
																					// Не особо то нужен. Разве что как сигнал, что мастер
						break;														// Нас почтил своим визитом.
						}
			*/
				default:
					dpr("unresolved i2c code hex: ");
					dprhex(code);	
					dpr(" dec:");
					dprln(code);	
					gxx::panic("");
					break;
			}
		}
	};
}




















/*

#include <IIC_ultimate.h>


void DoNothing(void);

u08 i2c_Do;								// Переменная состояния передатчика IIC
u08 i2c_InBuff[i2c_MasterBytesRX];		// Буфер прием при работе как Slave
u08 i2c_OutBuff[i2c_MasterBytesTX];		// Буфер передачи при работе как Slave
u08 i2c_SlaveIndex;						// Индекс буфера Slave


u08 i2c_Buffer[i2c_MaxBuffer];			// Буфер для данных работы в режиме Master
u08 i2c_index;							// Индекс этого буфера
u08 i2c_ByteCount;						// Число байт передаваемых

u08 i2c_SlaveAddress;						// Адрес подчиненного

u08 i2c_PageAddress[i2c_MaxPageAddrLgth];	// Буфер адреса страниц (для режима с sawsarp)
u08 i2c_PageAddrIndex;						// Индекс буфера адреса страниц
u08 i2c_PageAddrCount;						// Число байт в адресе страницы для текущего Slave

											// Указатели выхода из автомата:
IIC_F MasterOutFunc = &DoNothing;			//  в Master режиме
IIC_F SlaveOutFunc 	= &DoNothing;			//  в режиме Slave
IIC_F ErrorOutFunc 	= &DoNothing;			//  в результате ошибки в режиме Master

/*
u08 	WorkLog[100];						// Лог пишем сюда
u08		WorkIndex=0;						// Индекс лога
*/

//ISR(TWI_vect)								// Прерывание TWI Тут наше все.
//{
/*
PORTB ^= 0x01;								// Дрыгаем ногой порта, для синхронизации логического анализатора и отметок вызова TWI


// Отладочный кусок. Вывод лога работы конечного автомата в буфер памяти, а потом. По окончании работы через UART на волю
if (WorkIndex <99)							// Если лог не переполнен
{
	if (TWSR)								// Статус нулевой?
		{
		WorkLog[WorkIndex]= TWSR;			// Пишем статус в лог
		WorkIndex++;						
		}
	else
		{
		WorkLog[WorkIndex]= 0xFF;			// Если статус нулевой то вписываем FF
		WorkIndex++;
		}
}
*//*
switch(TWSR & 0xF8)						// Отсекаем биты прескалера
	{
	case 0x00:	// Bus Fail (автобус сломался)
			{
			i2c_Do |= i2c_ERR_BF;
			TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
			MACRO_i2c_WhatDo_ErrorOut
			break;
			}

	case 0x08:	// Старт был, а затем мы:
			{
			if( (i2c_Do & i2c_type_msk)== i2c_sarp)							// В зависимости от режима
				{
				i2c_SlaveAddress |= 0x01;									// Шлем Addr+R
				}
			else															// Или 
				{
				i2c_SlaveAddress &= 0xFE;									// Шлем Addr+W
				}
			
			TWDR = i2c_SlaveAddress;													// Адрес слейва
			TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
			break;
			}
				
	case 0x10:	// Повторный старт был, а затем мы
			{
			if( (i2c_Do & i2c_type_msk) == i2c_sawsarp)						// В зависимости от режима
				{
				i2c_SlaveAddress |= 0x01;									// Шлем Addr+R
				}
			else
				{
				i2c_SlaveAddress &= 0xFE;									// Шлем Addr+W
				}

			// To Do: Добавить сюда обработку ошибок 

			TWDR = i2c_SlaveAddress;													// Адрес слейва
			TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
			break;
			}

	case 0x18:	// Был послан SLA+W получили ACK, а затем:
			{
			if( (i2c_Do & i2c_type_msk) == i2c_sawp)						// В зависимости от режима
				{
				TWDR = i2c_Buffer[i2c_index];								// Шлем байт данных
				i2c_index++;												// Увеличиваем указатель буфера
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  // Go! 

				}

			if( (i2c_Do & i2c_type_msk) == i2c_sawsarp)
				{
				TWDR = i2c_PageAddress[i2c_PageAddrIndex];					// Или шлем адрес странцы (по сути тоже байт данных)
				i2c_PageAddrIndex++;										// Увеличиваем указатель буфера страницы
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;	// Go!
				}	 
			}
			break;

	case 0x20:	// Был послан SLA+W получили NACK - слейв либо занят, либо его нет дома.
			{
			i2c_Do |= i2c_ERR_NA;															// Код ошибки
			TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;			// Шлем шине Stop

			MACRO_i2c_WhatDo_ErrorOut 														// Обрабатываем событие ошибки;
			break;
			}

	case 0x28: 	// Байт данных послали, получили ACK!  (если sawp - это был байт данных. если sawsarp - байт адреса страницы)
			{	// А дальше: 
			if( (i2c_Do & i2c_type_msk) == i2c_sawp)							// В зависимости от режима
				{
				if (i2c_index == i2c_ByteCount)												// Если был байт данных последний
					{																		
					TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;	// Шлем Stop
					
					MACRO_i2c_WhatDo_MasterOut												// И выходим в обработку стопа

					}
				else
					{
					TWDR = i2c_Buffer[i2c_index];												// Либо шлем еще один байт
					i2c_index++;
					TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;  	// Go!
					}
				}

			if( (i2c_Do & i2c_type_msk) == i2c_sawsarp)						// В другом режиме мы
				{
				if(i2c_PageAddrIndex == i2c_PageAddrCount)					// Если последний байт адреса страницы
					{
					TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;		// Запускаем Повторный старт!
					}
				else
					{														// Иначе 
					TWDR = i2c_PageAddress[i2c_PageAddrIndex];				// шлем еще один адрес страницы
					i2c_PageAddrIndex++;									// Увеличиваем индекс счетчика адреса страниц
					TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;		// Go!
					}
				}	 
			}
			break;

	case 0x30:	//Байт ушел, но получили NACK причин две. 1я передача оборвана слейвом и так надо. 2я слейв сглючил.
			{
			i2c_Do |= i2c_ERR_NK;				// Запишем статус ошибки. Хотя это не факт, что ошибка. 

			TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;		// Шлем Stop

			MACRO_i2c_WhatDo_MasterOut													// Отрабатываем событие выхода

			break;
			}

	case 0x38:	//  Коллизия на шине. Нашелся кто то поглавней
			{
			i2c_Do |= i2c_ERR_LP;			// Ставим ошибку потери приоритета

			// Настраиваем индексы заново. 
			i2c_index = 0;
			i2c_PageAddrIndex = 0;

			TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;		// Как только шина будет свободна 
			break;																		// попробуем передать снова.
			}

	case 0x40: // Послали SLA+R получили АСК. А теперь будем получать байты
			{
			if(i2c_index+1 == i2c_ByteCount)								// Если буфер кончится на этом байте, то 
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;	// Требуем байт, а в ответ потом пошлем NACK(Disconnect)
				}															// Что даст понять слейву, что мол хватит гнать. И он отпустит шину
			else
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;	// Или просто примем байт и скажем потом ACK
				}

			break;
			}

	case 0x48: // Послали SLA+R, но получили NACK. Видать slave занят или его нет дома. 
			{
			i2c_Do |= i2c_ERR_NA;															// Код ошибки No Answer
			TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;			// Шлем Stop

			MACRO_i2c_WhatDo_ErrorOut														// Отрабатываем выходную ситуацию ошибки
			break;
			}

	case 0x50: // Приняли байт.
			{ 
			i2c_Buffer[i2c_index] = TWDR;			// Забрали его из буфера
			i2c_index++;

			// To Do: Добавить проверку переполнения буфера. А то мало ли что юзер затребует

			if (i2c_index+1 == i2c_ByteCount)		// Если остался еще один байт из тех, что мы хотели считать
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;		// Затребываем его и потом пошлем NACK (Disconnect)
				}
			else
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;		// Если нет, то затребываем следующий байт, а в ответ скажем АСК
				}
			break;
			}

	case 0x58:	// Вот мы взяли последний байт, сказали NACK слейв обиделся и отпал. 
			{
			i2c_Buffer[i2c_index] = TWDR;													// Взяли байт в буфер
			TWCR = 0<<TWSTA|1<<TWSTO|1<<TWINT|slave_mode<<TWEA|1<<TWEN|1<<TWIE;			// Передали Stop
			
			MACRO_i2c_WhatDo_MasterOut														// Отработали точку выхода

			break;
			}

// IIC  Slave ============================================================================

	case 0x68:	// RCV SLA+W Low Priority							// Словили свой адрес во время передачи мастером
	case 0x78:	// RCV SLA+W Low Priority (Broadcast)				// Или это был широковещательный пакет. Не важно
			{
			i2c_Do |= i2c_ERR_LP | i2c_Interrupted;					// Ставим флаг ошибки Low Priority, а также флаг того, что мастера прервали
			
			// Restore Trans after.
			i2c_index = 0;											// Подготовили прерваную передачу заново
			i2c_PageAddrIndex = 0;
			}														// И пошли дальше. Внимание!!! break тут нет, а значит идем в "case 60"

	case 0x60: // RCV SLA+W  Incoming?								// Или просто получили свой адрес
	case 0x70: // RCV SLA+W  Incoming? (Broascast)					// Или широковещательный пакет
			{

			i2c_Do |= i2c_Busy;										// Занимаем шину. Чтобы другие не совались
			i2c_SlaveIndex = 0;										// Указатель на начало буфера слейва, Неважно какой буфер. Не ошибемся

			if (i2c_MasterBytesRX == 1)								// Если нам суждено принять всего один байт, то готовимся принять  его
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;			// Принять и сказать пошли все н... NACK!
				}
			else
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// А если душа шире чем один байт, то сожрем и потребуем еще ACK!
				}
			break;
			}

	case 0x80:	// RCV Data Byte									// И вот мы приняли этот байт. Наш или широковещательный. Не важно
	case 0x90:	// RCV Data Byte (Broadcast)
			{
			i2c_InBuff[i2c_SlaveIndex] = TWDR;						// Сжираем его в буфер.
			
			i2c_SlaveIndex++;										// Сдвигаем указатель

			if (i2c_SlaveIndex == i2c_MasterBytesRX-1) 				// Свободно место всего под один байт? 
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;			// Приянть его и сказать NACK!
				}
			else 
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Места еще дофига? Принять и ACK!
				}
			break;
			} 

	case 0x88: // RCV Last Byte										// Приянли последний байт
	case 0x98: // RCV Last Byte (Broadcast)
			{
			i2c_InBuff[i2c_SlaveIndex] = TWDR;						// Сожрали его в буфер
			
			if (i2c_Do & i2c_Interrupted)							// Если у нас был прерываный сеанс от имени мастера
				{
				TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Влепим в шину свой Start поскорей и сделаем еще одну попытку
				}
			else
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Если не было такого факта, то просто отвалимся и будем ждать
				}

			MACRO_i2c_WhatDo_SlaveOut												// И лениво отработаем наш выходной экшн для слейва
			break;
			}

	
	case 0xA0: // Ой, мы получили Повторный старт. Но чо нам с ним делать? 
			{

			// Можно, конечно, сделать вспомогательный автомат, чтобы обрабатывать еще и адреса внутренних страниц, подобно еепромке. 
			// Но я не стал заморачиваться. В этом случае делается это тут.

			TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// просто разадресуемся, проигнорировав этот посыл	
			break;
			}



	case 0xB0:  // Поймали свой адрес на чтение во время передачи Мастером			
			{
			i2c_Do |= i2c_ERR_LP | i2c_Interrupted;			// Ну чо, коды ошибки и флаг прерваной передачи.
		
			
			// Восстанавливаем индексы
			i2c_index = 0;
			i2c_PageAddrIndex = 0;

			}												// Break нет! Идем дальше						

	case 0xA8:	// // Либо просто словили свой адрес на чтение
			{
			i2c_SlaveIndex = 0;								// Индексы слейвовых массивов на 0

			TWDR = i2c_OutBuff[i2c_SlaveIndex];				// Чтож, отдадим байт из тех что есть.
			
			if(i2c_MasterBytesTX == 1)
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;	// Если он последний, мы еще на NACK в ответ надеемся 
				}
			else
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;	// А если нет, то  ACK ждем
				}

			break;
			} 
			

	case 0xB8: // Послали байт, получили ACK
			{

			i2c_SlaveIndex++;								// Значит продолжаем дискотеку. Берем следующий байт
			TWDR = i2c_OutBuff[i2c_SlaveIndex];				// Даем его мастеру

			if (i2c_SlaveIndex == i2c_MasterBytesTX-1)		// Если он последний был, то
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;	// Шлем его и ждем NACK
				}
			else
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|0<<TWEN|1<<TWIE;	// Если нет, то шлем и ждем ACK
				}

			break;
			}

	case 0xC0: // Мы выслали последний байт, больше у нас нет, получили NACK
	case 0xC8: // или ACK. В данном случае нам пох. Т.к. больше байтов у нас нет.
			{
			if (i2c_Do & i2c_Interrupted)											// Если там была прерваная передача мастера
				{																	// То мы ему ее вернем
				i2c_Do &= i2c_NoInterrupted;										// Снимем флаг прерваности
				TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Сгенерим старт сразу же как получим шину.
				}
			else
				{
				TWCR = 0<<TWSTA|0<<TWSTO|1<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;			// Если мы там одни, то просто отдадим шину
				}

			MACRO_i2c_WhatDo_SlaveOut												// И отработаем выход слейва. Впрочем, он тут
																					// Не особо то нужен. Разве что как сигнал, что мастер
			break;																	// Нас почтил своим визитом.
			}

	default:	break;
	}
}

/*void DoNothing(void)																// Функция пустышка, затыкать несуществующие ссылки
{
}*/
/*
void Init_i2c(void)							// Настройка режима мастера
{
i2c_PORT |= 1<<i2c_SCL|1<<i2c_SDA;			// Включим подтяжку на ноги, вдруг юзер на резисторы пожмотился
i2c_DDR &=~(1<<i2c_SCL|1<<i2c_SDA);

TWBR = 0xFF;         						// Настроим битрейт
TWSR = 0x03;
}

void Init_Slave_i2c(IIC_F Addr)				// Настройка режима слейва (если нужно)
{
TWAR = i2c_MasterAddress;					// Внесем в регистр свой адрес, на который будем отзываться. 
											// 1 в нулевом бите означает, что мы отзываемся на широковещательные пакеты
SlaveOutFunc = Addr;						// Присвоим указателю выхода по слейву функцию выхода

TWCR = 0<<TWSTA|0<<TWSTO|0<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE;		// Включаем агрегат и начинаем слушать шину.
}
*/



#endif