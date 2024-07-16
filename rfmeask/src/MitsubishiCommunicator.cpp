/**
    @file
    Протокол управления строится представлении байта в виде пары ASCII символов.
*/

#include "MitsubishiCommunicator.h"
#include "utilxx/ascii_convert.h"
#include "utilxx/charbuf.h"
#include <defs.h>
#include <fcntl.h>
#include <igris/util/dstring.h>
#include <math.h>
#include <modes.h>
#include <sstream>
#include <thread>

extern bool emulate_mode;
std::recursive_mutex rs485_mutex;

#ifdef __WIN32__
#define O_NOCTTY 0
#endif

#define TRACE_COMMUNICATION 0

using namespace std;

MitsubishiCommunicator::MitsubishiCommunicator() :
    logger("MitsubishiCommunicator")
{
    tim = chrono::milliseconds(20);
}

void MitsubishiCommunicator::open(const char *str)
{
    portstr = str;

    int err = serport.open(str, 115200, 'e', 8, 1);
    // file.nodelay(true);

    if (err < 0)
    {
        logger.fault("try open {}: error: {}", str, strerror(errno));

        if (!NOFAULT)
            exit(-1);

        return;
    }

    auto ret = serport.write("\003", 1);
    if (ret.is_error())
    {
        perror("mrs write:");
    }
}

void MitsubishiCommunicator::close()
{
    serport.close();
}

void MitsubishiCommunicator::rawSend(const char *str, uint8_t len)
{
    lock_guard<recursive_mutex> lk(mtx);
    auto ans = serport.write(str, len);
    if (ans.is_error())
    {
        perror("write");
    }

    ans = serport.flush();
    if (ans.is_error())
    {
        perror("flush");
    }
}

void MitsubishiCommunicator::rawSend(const char *str)
{
    lock_guard<recursive_mutex> lk(mtx);
    rawSend(str, strlen(str));
}

/** В текущей версии запрос выполняется с чтением ответа по таймеру.
    Это объясняется тем, что в ранних версиях были проблемы
    с качеством передачи данных. Было мало надежды на терминальный символ.
*/
int MitsubishiCommunicator::rawTimeredRecv(char *data,
                                           int len,
                                           chrono::milliseconds tim)
{
    // Вопрос о том, как правильно забирать данные, строго говоря, требует
    // проработки.
    lock_guard<recursive_mutex> lk(mtx);

    serport.nonblock(true);
    this_thread::sleep_for(chrono::milliseconds(14));
    int size = *serport.read(data, len);
    if (size < 0)
    {
        // perror("mrs read");
    }
    return size;
}

int MitsubishiCommunicator::rawQuery(const char *str, char *ans)
{
    lock_guard<recursive_mutex> lk(mtx);

    rawSend(str);
    int len = rawTimeredRecv(ans, 128, tim);
    return len;
}

uint8_t MitsubishiCommunicator::checksum(const char *str, const char *end)
{
    int sum = 0;
    while (str != end)
    {
        sum += *str++;
    };
    return sum;
}

//Контрольная сумма передается в явном виде в конце пакета.
uint8_t MitsubishiCommunicator::checksum_code(const char *str)
{
    return (hexascii_to_halfbyte(*str) << 4) + hexascii_to_halfbyte(*(str + 1));
}

/** Проверка коректности посылки.
    Расчет контрольной суммы согласно документации протокола управления
   Митсубисши. Не учитывается START символ и поле контрольной суммы.
*/
bool MitsubishiCommunicator::checkcorrectans(const char *str, int len)
{
    //Считаем посылку невалидной, если она короче минимума символов,
    //или не содержит ограничивающих символов.
    if (len < 3)
        return false;
    if (*(str + 0) != 2 || *(str + len - 3) != 3)
        return false;
    //Проверяем контрольную сумму.
    unsigned cs1 = checksum(str + 1, str + len - 2);
    unsigned cs2 = checksum_code(str + len - 2);
    return cs1 == cs2;
}

int MitsubishiCommunicator::ackRawQuery(const char *query, char *answer)
{
    int len;
    int sendlen = strlen(query);
    int iter = 0;
    lock_guard<recursive_mutex> lk(mtx);
    if (!serport.good())
        return 0;
resend:
    answer[0] = 0;
    if (TRACE_COMMUNICATION)
    {
        nos::println("query:", igris::dstring(query, strlen(query)));
    }
    len = rawQuery(query, answer);
    if (TRACE_COMMUNICATION)
    {
        nos::println("answer:", igris::dstring(answer, strlen(answer)));
    }
    //Проверяем корректность посылки.
    //Если посылка невалидна, пробуем еще раз установленное количество раз.
    if (!checkcorrectans(answer, len))
    {
        brokenPackage = brokenPackage + 1;
        nos::fprintln("mrs485: unvalid package. (q:{}) (len:{}) Reopen port",
                      std::string(query, sendlen),
                      len);
        close();
        open(portstr);
        ++iter;
        if (iter == 10)
        {
            return -1;
        };
        goto resend;
    };
    return len;
}

charbuf<10> MitsubishiCommunicator::Form(int stantion, int command, int datano)
{
    charbuf<10> cbuf;
    char *buf = cbuf.to_buf();
    cbuf[0] = 1;
    cbuf[1] = halfbyte_to_hexascii(stantion);
    cbuf[4] = 2;
    cbuf[7] = 3;
    uint8_hcode(buf + 2, command);
    uint8_hcode(buf + 5, datano);
    uint8_hcode(buf + 8, checksum(buf + 1, buf + 8));
    return cbuf;
}

charbuf<14> MitsubishiCommunicator::Form4(int stantion,
                                          int command,
                                          int datano,
                                          int16_t data)
{
    charbuf<14> cbuf;
    char *buf = cbuf.to_buf();
    cbuf[0] = 1;
    cbuf[1] = halfbyte_to_hexascii(stantion);
    cbuf[4] = 2;
    cbuf[7 + 4] = 3;
    uint8_hcode(buf + 2, command);
    uint8_hcode(buf + 5, datano);
    uint16_hcode(buf + 7, data);
    uint8_hcode(buf + 8 + 4, checksum(buf + 1, buf + 8 + 4));
    return cbuf;
}

charbuf<14> MitsubishiCommunicator::Form4(int stantion,
                                          int command,
                                          int datano,
                                          const char *data)
{
    charbuf<14> cbuf;
    char *buf = cbuf.to_buf();
    cbuf[0] = 1;
    cbuf[1] = halfbyte_to_hexascii(stantion);
    cbuf[4] = 2;
    cbuf[7 + 4] = 3;
    uint8_hcode(buf + 2, command);
    uint8_hcode(buf + 5, datano);
    memcpy(buf + 7, data, 4);
    uint8_hcode(buf + 8 + 4, checksum(buf + 1, buf + 8 + 4));
    return cbuf;
}

charbuf<18> MitsubishiCommunicator::Form8(int stantion,
                                          int command,
                                          int datano,
                                          int32_t data)
{
    charbuf<18> cbuf;
    char *buf = cbuf.to_buf();
    cbuf[0] = 1;
    cbuf[1] = halfbyte_to_hexascii(stantion);
    cbuf[4] = 2;
    cbuf[7 + 8] = 3;
    uint8_hcode(buf + 2, command);
    uint8_hcode(buf + 5, datano);
    uint32_hcode(buf + 7, data);
    uint8_hcode(buf + 8 + 8, checksum(buf + 1, buf + 8 + 8));
    return cbuf;
}

charbuf<22> MitsubishiCommunicator::Form12(
    int stantion, int command, int datano, int16_t predata, int32_t data)
{
    charbuf<22> cbuf;
    memset(cbuf.to_buf(), 0, 22);
    char *buf = cbuf.to_buf();
    cbuf[0] = 1;
    cbuf[1] = halfbyte_to_hexascii(stantion);
    cbuf[4] = 2;
    cbuf[7 + 8 + 4] = 3;
    uint8_hcode(buf + 2, command);
    uint8_hcode(buf + 5, datano);
    uint16_hcode(buf + 7, predata);
    uint32_hcode(buf + 7 + 4, data);
    uint8_hcode(buf + 8 + 8 + 4, checksum(buf + 1, buf + 8 + 8 + 4));
    return cbuf;
}

std::string MitsubishiCommunicator::Query(uint8_t &errstat,
                                          int stantion,
                                          int command,
                                          int datano)
{
    char answer[1024];
    memset(answer, 0, 1024);
    lock_guard<recursive_mutex> lk(mtx);
    charbuf<10> buf = Form(stantion, command, datano);
    int len = ackRawQuery(buf.to_buf(), answer);
    if (len == -1)
    {
        errstat = MRS_NOTCONNECTION;
        return std::string();
    }
    std::string ret = std::string(answer, len);

    errstat = islower(answer[2]) ? MRS_ALARMERROR : 0;
    return ret;
}

std::string MitsubishiCommunicator::Query4(
    uint8_t &errstat, int stantion, int command, int datano, int16_t data)
{
    char answer[1024];
    memset(answer, 0, 1024);
    lock_guard<recursive_mutex> lk(mtx);
    charbuf<14> buf = Form4(stantion, command, datano, data);
    int len = ackRawQuery(buf.to_buf(), answer);
    if (len == -1)
    {
        errstat = MRS_NOTCONNECTION;
        return std::string();
    }
    std::string ret = std::string(answer, len);
    errstat = islower(answer[2]) ? MRS_ALARMERROR : 0;
    return ret;
}

std::string MitsubishiCommunicator::Query4(
    uint8_t &errstat, int stantion, int command, int datano, const char *data)
{
    char answer[1024];
    memset(answer, 0, 1024);
    lock_guard<recursive_mutex> lk(mtx);
    charbuf<14> buf = Form4(stantion, command, datano, data);
    int len = ackRawQuery(buf.to_buf(), answer);
    if (len == -1)
    {
        errstat = MRS_NOTCONNECTION;
        return std::string();
    }
    std::string ret = std::string(answer, len);
    errstat = islower(answer[2]) ? MRS_ALARMERROR : 0;
    return ret;
}

std::string MitsubishiCommunicator::Query8(
    uint8_t &errstat, int stantion, int command, int datano, int32_t data)
{
    char answer[1024];
    lock_guard<recursive_mutex> lk(mtx);
    charbuf<18> buf = Form8(stantion, command, datano, data);
    int len = ackRawQuery(buf.to_buf(), answer);
    if (len == -1)
    {
        errstat = MRS_NOTCONNECTION;
        return std::string();
    }
    std::string ret = std::string(answer, len);
    errstat = islower(answer[2]) ? MRS_ALARMERROR : 0;
    return ret;
}

std::string MitsubishiCommunicator::Query12(uint8_t &errstat,
                                            int stantion,
                                            int command,
                                            int datano,
                                            int16_t predata,
                                            int32_t data)
{
    char answer[1024];
    lock_guard<recursive_mutex> lk(mtx);
    charbuf<22> buf = Form12(stantion, command, datano, predata, data);
    nos::println("message:", buf.to_buf());
    int len = ackRawQuery(buf.to_buf(), answer);
    if (len == -1)
    {
        errstat = MRS_NOTCONNECTION;
        return std::string();
    }
    std::string ret = std::string(answer, len);
    errstat = islower(answer[2]) ? MRS_ALARMERROR : 0;
    return ret;
}

void MitsubishiCommunicator::jog(
    uint8_t &errstat, int stantion, int speed, int acceleration, int direction)
{
    lock_guard<recursive_mutex> lk(mtx);

    Query4(errstat, stantion, 0x8B, 0x00, (int16_t)0x0000);
    if (errstat)
        return;
    Query4(errstat, stantion, 0x8B, 0x00, (int16_t)0x0001);
    if (errstat)
        return;
    Query4(errstat, stantion, 0xA0, 0x10, (int16_t)speed);
    if (errstat)
        return;
    Query8(errstat, stantion, 0xA0, 0x11, acceleration);
    if (errstat)
        return;
    if (direction == FORWARD)
        Query8(errstat, stantion, 0x92, 0x00, 0x00000801);
    else
        Query8(errstat, stantion, 0x92, 0x00, 0x00001001);
    if (errstat)
        return;
}

void MitsubishiCommunicator::epos(uint8_t &errstat,
                                  int stantion,
                                  int32_t distance,
                                  int speed,
                                  int acceleration,
                                  int direction,
                                  DistanceUnit distunit)
{
    lock_guard<recursive_mutex> lk(mtx);

    Query4(errstat, stantion, 0x8B, 0x00, 0x0002);
    Query4(errstat, stantion, 0xA0, 0x10, speed);
    Query8(errstat, stantion, 0xA0, 0x11, acceleration);
    Query8(errstat, stantion, 0xA0, 0x20, distance);
    if (direction == FORWARD)
        Query4(errstat,
               stantion,
               0xA0,
               0x21,
               distunit == DistanceUnit::ServoSideEncoderUnit ? 0x0100
                                                              : 0x0000);
    else
        Query4(errstat,
               stantion,
               0xA0,
               0x21,
               distunit == DistanceUnit::ServoSideEncoderUnit ? 0x0101
                                                              : 0x0001);
    Query4(errstat, stantion, 0xA0, 0x40, 0x1EA5);
}

void MitsubishiCommunicator::impmode(uint8_t &errstat, int stantion)
{
    lock_guard<recursive_mutex> lk(mtx);
    Query4(errstat, stantion, 0x8B, 0x00, (int16_t)0x0000);
}

void MitsubishiCommunicator::temporary_stop(uint8_t &errstat, int stantion)
{
    lock_guard<recursive_mutex> lk(mtx);
    Query4(errstat, stantion, 0xA0, 0x41, "STOP");
}

void MitsubishiCommunicator::stop(uint8_t &errstat, int stantion)
{
    lock_guard<recursive_mutex> lk(mtx);
    Query8(errstat, stantion, 0x92, 0x00, 0x00000007);
}

int32_t MitsubishiCommunicator::Parse8(std::string str, int bias)
{
    return static_cast<int32_t>(hexcode_to_uint32(str.c_str() + bias));
}

int16_t MitsubishiCommunicator::Parse4(std::string str, int bias)
{
    return static_cast<int16_t>(hexcode_to_uint16(str.c_str() + bias));
}

int32_t MitsubishiCommunicator::abspos_encoder_side(uint8_t &errstat,
                                                    int stantion,
                                                    DistanceUnit distunit)
{
    lock_guard<recursive_mutex> lk(mtx);
    std::string str =
        Query(errstat,
              stantion,
              0x02,
              distunit == DistanceUnit::CommandUnit ? 0x91 : 0x90);
    // if (errstat)
    //    return 0;
    return Parse8(str, 3);
}

/**
    Функции для чтения и изменения параметров СУ.
*/
int16_t MitsubishiCommunicator::parametr_group_read(uint8_t &errstat,
                                                    int stantion)
{
    lock_guard<recursive_mutex> lk(mtx);
    std::string str = Query(errstat, stantion, 0x04, 0x01);
    if (errstat == MRS_NOTCONNECTION)
        return 0;
    return Parse4(str, 3);
}

void MitsubishiCommunicator::parametr_group_set(uint8_t &errstat,
                                                int stantion,
                                                int16_t group)
{
    lock_guard<recursive_mutex> lk(mtx);
    Query4(errstat, stantion, 0x85, 0x00, group);
}

void MitsubishiCommunicator::write_string_parametr(
    uint8_t &errstat, int stantion, int16_t param, int save, const char *data)
{
    int len;
    int ddata;
    uint16_t hdata;
    int decimal_point_position = 0;
    char *cp;
    lock_guard<recursive_mutex> lk(mtx);

    printf("write_string_parametr: %s", data);

    len = strlen(data);

    if ((cp = strchr((char *)data, 'h')))
    {
        if (cp != data + len - 1)
            goto error;

        hdata = hex_to_uint16(data);
        write_parametr(errstat, stantion, param, save, 0, hdata);
        return;
    }

    if ((cp = strchr((char *)data, '.')))
    {
        if (cp == data + len - 1)
            goto error;

        decimal_point_position = cp - data;
        decimal_point_position = len - 1 - decimal_point_position;

        //Сдвигаем строку, закрывая убирая десятичную точку.
        memmove(cp, cp + 1, data + len - cp);
    }

    ddata = strtoul(data, NULL, 10);
    write_parametr(
        errstat, stantion, param, save, decimal_point_position, ddata);

    BUG();

    return;

error:
    errstat = -1;
    printf("wrong parametr's data");
}

//@save - запомнить установленный параметр в eeprom.
//при false параметр будет сброшен при перезагрузке.
void MitsubishiCommunicator::write_parametr(uint8_t &errstat,
                                            int stantion,
                                            int16_t param,
                                            int save,
                                            uint8_t decimal_point_position,
                                            int32_t data)
{
    lock_guard<recursive_mutex> lk(mtx);

    int16_t predata;

    if (!(decimal_point_position <= 5))
        return;
    if (!(save == 0 || save == 1))
        return;

    if (save == 0)
        save = 3;
    else
        save = 0;

    predata = (save << 4) | (decimal_point_position << 0);

    nos::println(stantion, param, save, decimal_point_position, data);

    Query12(errstat, stantion, 0x94, param, predata, data);
}

uint32_t MitsubishiCommunicator::read_parametr(uint8_t &errstat,
                                               int stantion,
                                               int16_t param)
{
    lock_guard<recursive_mutex> lk(mtx);
    std::string str = Query(errstat, stantion, 0x15, param);
    if (errstat == MRS_NOTCONNECTION)
        return 0;
    return Parse8(str, 7);
}

int MitsubishiCommunicator::read_parameter_2(uint8_t &errstat,
                                             int stantion,
                                             int16_t param,
                                             struct read_parameter_s *rets)
{
    lock_guard<recursive_mutex> lk(mtx);
    std::string str = Query(errstat, stantion, 0x15, param);

    if (errstat == MRS_NOTCONNECTION)
        return -1;

    const char *cstr = str.c_str();
    const char *strt = strchr(cstr, '\x02') + 1;
    const char *fini = strchr(strt, '\x03');
    int len = fini - strt;

    std::string_view buffer(strt, len);

    if (len != 14)
        return -2;

    rets->stantion = hex2half(buffer[0]);
    rets->code = hex2half(buffer[1]);
    rets->sign = hex2half(buffer[3]) & 0x01;
    rets->disptype = hex2half(buffer[4]) & 0x01;
    rets->wrtype = hex2half(buffer[4]) & 0x02 >> 1;
    rets->dpoint = hex2half(buffer[5]);
    rets->data = hex_to_uint32(&buffer[6]);

    return 0;
}

int MitsubishiCommunicator::write_parameter_2(
    struct MitsubishiCommunicator::write_parameter_s *wr)
{
    lock_guard<recursive_mutex> lk(mtx);
    uint8_t errstat;

    uint16_t predata;
    UINT16_LO(predata) = ((wr->save ? 0 : 3) << 4) | wr->dpoint;
    UINT16_HI(predata) = 0;

    uint32_t data = wr->data;
    auto ret = Query12(errstat, wr->stantion, 0x94, wr->code, predata, data);

    return -1;
}

std::string datastring(struct MitsubishiCommunicator::read_parameter_s *rets)
{
    std::stringstream ddat;

    if (rets->disptype)
    {
        if (rets->dpoint == 0)
        {
            ddat << std::dec << rets->data;
        }
        else
        {
            ddat << std::dec << rets->data / pow(10, rets->dpoint - 1);
        }
    }
    else
    {
        ddat << std::uppercase << std::hex << rets->data;
    }

    return ddat.str();
}

int8_t MitsubishiCommunicator::read_lastAlarm(uint8_t &errstat, int stantion)
{
    lock_guard<recursive_mutex> lk(mtx);
    std::string str = Query(errstat, stantion, 0x02, 0x00);
    if (str.length() >= 7)
        return std::stoi(str.substr(5, 2), nullptr, 16);
    else
    {
        throw std::length_error("Received data length error!");
        return 0;
    }
}

//Настройка виртуального редуктора.
void MitsubishiCommunicator::gear_numerator(uint8_t &errstat,
                                            int stantion,
                                            int32_t data)
{
    lock_guard<recursive_mutex> lk(mtx);
    parametr_group_set(errstat, stantion, 0);
    write_parametr(errstat, stantion, 6, 1, 0, data);
}

void MitsubishiCommunicator::gear_denumerator(uint8_t &errstat,
                                              int stantion,
                                              int32_t data)
{
    lock_guard<recursive_mutex> lk(mtx);
    parametr_group_set(errstat, stantion, 0); // set group PA
    write_parametr(errstat, stantion, 7, 1, 0, data);
}

void MitsubishiCommunicator::clear_signal(uint8_t &errstat, int stantion)
{
    Query4(errstat, stantion, 0x8B, 0x00, (int16_t)0x0000);
    Query8(errstat, stantion, 0x92, 0x60, 0x00000080);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    Query8(errstat, stantion, 0x92, 0x60, 0x00000000);
}

//Чтение выходных сигналов СУ. см. Документацию на СУ
int32_t MitsubishiCommunicator::output_signals(uint8_t &errstat, int stantion)
{
    std::string str = Query(errstat, stantion, 0x12, 0x80);
    if (errstat == MRS_NOTCONNECTION)
        return 0;
    return Parse8(str, 3);
}

//Чтение входных сигналов СУ. см. Документацию на СУ
int32_t MitsubishiCommunicator::input_signals(uint8_t &errstat, int stantion)
{
    std::string str = Query(errstat, stantion, 0x12, 0x00);
    if (errstat == MRS_NOTCONNECTION)
        return 0;
    return Parse8(str, 3);
}

void MitsubishiCommunicator::set_input_signals(uint8_t &errstat,
                                               int stantion,
                                               int32_t data)
{
    Query8(errstat, stantion, 0x92, 0x60, data);
}
