#ifndef GENOS_UTIL_COMPAT_I2C
#define GENOS_UTIL_COMPAT_I2C

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

//Misc:
#define I2C_NO_INFO			0xF8 //no state information available
#define I2C_BUS_ERROR		0x00 //illegal start or stop condition

//Master
#define I2C_START			0x08 //start condition transmitted
#define I2C_REP_START		0x10 //repeated start condition transmitted

//Master Transmitter
#define I2C_MT_SLA_ACK		0x18 //SLA+W transmitted, ACK received
#define I2C_MT_SLA_NACK		0x20 //SLA+W transmitted, NACK received
#define I2C_MT_DATA_ACK		0x28 //data transmitted, ACK received
#define I2C_MT_DATA_NACK	0x30 //data transmitted, NACK received
#define I2C_MT_ARB_LOST		0x38 //arbitration lost in SLA+W or data

//Master Receiver	
#define I2C_MR_ARB_LOST		0x38 //arbitration lost in SLA+R or NACK
#define I2C_MR_SLA_ACK		0x40 //SLA+R transmitted, ACK received
#define I2C_MR_SLA_NACK		0x48 //SLA+R transmitted, NACK received	
#define I2C_MR_DATA_ACK		0x50 //data received, ACK returned
#define I2C_MR_DATA_NACK	0x58 //data received, NACK returned

//Slave Transmitter
#define I2C_ST_SLA_ACK			0xA8 //SLA+R received, ACK returned	
#define I2C_ST_ARB_LOST_SLA_ACK	0xB0 //arbitration lost in SLA+RW, SLA+R received, ACK returned	
#define I2C_ST_DATA_ACK			0xB8 //data transmitted, ACK received
#define I2C_ST_DATA_NACK		0xC0 //data transmitted, NACK received	
#define I2C_ST_LAST_DATA		0xC8 //last data byte transmitted, ACK received

//Slave Receiver
#define I2C_SR_SLA_ACK				0x60 //SLA+W received, ACK returned
#define I2C_SR_ARB_LOST_SLA_ACK		0x68 //arbitration lost in SLA+RW, SLA+W received, ACK returned
#define I2C_SR_GCALL_ACK			0x70 //general call received, ACK returned	
#define I2C_SR_ARB_LOST_GCALL_ACK 	0x78 //arbitration lost in SLA+RW, general call received, ACK returned
#define I2C_SR_DATA_ACK				0x80 //data received, ACK returned
#define I2C_SR_DATA_NACK			0x88 //data received, NACK returned	
#define I2C_SR_GCALL_DATA_ACK		0x90 //general call data received, ACK returned
#define I2C_SR_GCALL_DATA_NACK		0x98 //general call data received, NACK returned
#define I2C_SR_STOP					0xA0 //stop or repeated start condition received while selected

#endif