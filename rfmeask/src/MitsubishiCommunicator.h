/**
    @file
*/

#ifndef MITSUBISHICOMMUNICATOR_H
#define MITSUBISHICOMMUNICATOR_H

#include "utilxx/charbuf.h"
#include <chrono>
#include <defs.h>
#include <fstream>
#include <igris/util/bug.h>
#include <mutex>
#include <nos/io/file.h>
#include <nos/io/serial_port.h>
#include <nos/log/logger.h>

#define MRS_NOTCONNECTION 1
#define MRS_ALARMERROR 2

extern std::recursive_mutex rs485_mutex;

class MitsubishiCommunicator
{
public:
    std::recursive_mutex &mtx = rs485_mutex;

private:
    nos::serial_port serport = {};
    std::chrono::milliseconds tim = {};
    const char *portstr = nullptr;
    volatile uint64_t brokenPackage = 0;
    nos::log::logger logger = {};

    std::chrono::time_point<std::chrono::system_clock> last_error_time = {};
    int count_of_skipped_errors = 0;

public:
    MitsubishiCommunicator();
    MitsubishiCommunicator(const MitsubishiCommunicator &) = delete;
    MitsubishiCommunicator &operator=(const MitsubishiCommunicator &) = delete;

    void open(const char *str);
    void close();

    void rawSend(const char *str, uint8_t len);
    void rawSend(const char *str);

    int rawTimeredRecv(char *data, int len, std::chrono::milliseconds tim);

    int rawQuery(const char *str, char *ans);
    int ackRawQuery(const char *str, char *ans);

    bool checkcorrectans(const char *str, int len);

    void error();

    uint8_t checksum(const char *str, const char *end);
    uint8_t checksum_code(const char *str);

    std::string Query(uint8_t &errstat, int stantion, int command, int datano);
    std::string Query4(
        uint8_t &errstat, int stantion, int command, int datano, int16_t data);
    std::string Query4(uint8_t &errstat,
                       int stantion,
                       int command,
                       int datano,
                       const char *data);
    std::string Query8(
        uint8_t &errstat, int stantion, int command, int datano, int32_t data);
    std::string Query12(uint8_t &errstat,
                        int stantion,
                        int command,
                        int datano,
                        int16_t predata,
                        int32_t data);

    charbuf<10> Form(int stantion, int command, int datano);
    charbuf<14> Form4(int stantion, int command, int datano, int16_t data);
    charbuf<14> Form4(int stantion, int command, int datano, const char *data);
    charbuf<18> Form8(int stantion, int command, int datano, int32_t data);
    charbuf<22> Form12(
        int stantion, int command, int datano, int16_t predata, int32_t data);

    int32_t Parse8(std::string str, int bias);
    int16_t Parse4(std::string str, int bias);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void jog(uint8_t &errstat,
             int stantion,
             int speed,
             int acceleration,
             int direction);
    void epos(uint8_t &errstat,
              int stantion,
              int32_t distance,
              int speed,
              int acceleration,
              int direction,
              DistanceUnit distunit);

    void impmode(uint8_t &errstat, int stantion);
    void temporary_stop(uint8_t &errstat, int stantion);
    void stop(uint8_t &errstat, int stantion);

    int32_t
    abspos_encoder_side(uint8_t &errstat, int stantion, DistanceUnit distunit);
    int32_t output_signals(uint8_t &errstat, int stantion);
    int32_t input_signals(uint8_t &errstat, int stantion);
    void set_input_signals(uint8_t &errstat, int stantion, int32_t data);

    int16_t parametr_group_read(uint8_t &errstat, int stantion);
    void parametr_group_set(uint8_t &errstat, int stantion, int16_t group);
    void write_parametr(uint8_t &errstat,
                        int stantion,
                        int16_t param,
                        int save,
                        uint8_t decimal_point_position,
                        int32_t data);
    void write_string_parametr(uint8_t &errstat,
                               int stantion,
                               int16_t param,
                               int save,
                               const char *data);
    uint32_t read_parametr(uint8_t &errstat, int stantion, int16_t param);
    int8_t read_lastAlarm(uint8_t &errstat, int stantion);

    void gear_numerator(uint8_t &errstat, int stantion, int32_t data);
    void gear_denumerator(uint8_t &errstat, int stantion, int32_t data);

    void clear_signal(uint8_t &errstat, int stantion);

    struct read_parameter_s
    {
        uint8_t stantion;
        uint8_t code;
        uint8_t sign;
        uint8_t wrtype;
        uint8_t disptype;
        uint8_t dpoint;
        uint32_t data;
    };

    struct write_parameter_s
    {
        uint8_t stantion;
        uint8_t code;
        uint8_t save;
        uint8_t dpoint;
        uint32_t data;
    };

    int read_parameter_2(uint8_t &errstat,
                         int stantion,
                         int16_t param,
                         struct read_parameter_s *rets);

    int write_parameter_2(struct write_parameter_s *sets);
};

std::string datastring(struct MitsubishiCommunicator::read_parameter_s *rets);

void debug_print_read_parameter_s(
    struct MitsubishiCommunicator::read_parameter_s *rets);

#endif
