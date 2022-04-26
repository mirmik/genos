#ifndef I2C_AUTOMATE_H
#define I2C_AUTOMATE_H

#include <drivers/i2c/i2c_driver.h>

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

namespace genos
{
	class i2c_automate : public i2c_device
	{
		//dlist_head wait_list = DLIST_INIT(wait_list);


	public:
		i2c_driver * dev;

		uint8_t * _rxdata;
		int _rxsize;

		uint8_t * _txdata;
		int _txsize;

		int it;

		void begin()
		{
			dev->set_irq_handler(&i2c_automate::_irq_handler, this);
		}

		void lock_bus() 
		{
			dev->lock_bus();
		}

		void unlock_bus() 
		{
			dev->unlock_bus();
		}

		void write(uint8_t addr,
		                   const uint8_t * data, int size) override
		{
			BUG();
		}

		void read(uint8_t addr,
		                  uint8_t * data, int size) override 
		{
			BUG();
		}

		void exchange(uint8_t addr,
		                      uint8_t * rxdata, int rxsize,
		                      const uint8_t * txdata, int txsize) override 
		{
			system_lock();

			//if (!device_empty(wait_list))
			//	wait_current_schedee(&dev->wait_list, 0);
			sema_down();

			_rxdata = rxdata;
			_txdata = txdata;
			_rxsize = rxsize;
			_txsize = txsize;
			it = 0;

			dev->send_start();

			sema_wait(&dev->lock);
			//wait_current_schedee(&dev->wait_list, 1);

			if (!device_empty(wait_list))
				unwait_one(&dev->wait_list);

			system_unlock();
		}


		static void _irq_handler(void* arg, int code)
		{
			switch (code)	 // Отсекаем биты прескалера
			{
				case i2cCode::BusFail: 	// Bus Fail (автобус сломался)
					break;

				case i2cCode::StartSended: // Старт был, а затем мы:
					break;

				case i2cCode::RestartSended: // Повторный старт был, а затем мы
					break;

				case i2cCode::AckAfterSLAW:   // Был послан SLA+W получили ACK, а затем:
					break;

				case i2cCode::NackAfterSLAW: // Был послан SLA+W получили NACK - слейв либо занят, либо его нет дома.
					break;

				case i2cCode::AckAfterData:   // Байт данных послали, получили ACK!  (если sawp - это был байт данных. если sawsarp - байт адреса страницы)
					break;

				case i2cCode::NackAfterData:   //Байт ушел, но получили NACK причин две. 1я передача оборвана слейвом и так надо. 2я слейв сглючил.
					break;

				case i2cCode::BusCollision:	//  Коллизия на шине. Нашелся кто то поглавней
					break;

				case i2cCode::AckAfterSLAR:   // Послали SLA+R получили АСК. А теперь будем получать байты
					break;

				case i2cCode::NackAfterSLAR:   // Послали SLA+R, но получили NACK. Видать slave занят или его нет дома.
					break;

				case i2cCode::RecvByte:     // Приняли байт.
					break;

				case i2cCode::RecvLastByte: // Вот мы взяли последний байт, сказали NACK слейв обиделся и отпал.
					break;

				/*
				/		IC  Slave ============================================================================

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
					//dpr("unresolved i2c code hex: ");
					//dprhex(code);
					//dpr(" dec:");
					//dprln(code);
					//BUG();
					break;
			}
		}

	}
}

#endif