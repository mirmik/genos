/**
    @file
    Коммуникатор для общения с платой синхронизатора.
*/

#ifndef DRIVER_RS232
#define DRIVER_RS232

#include "stdio.h"
#include "unistd.h"
#include "utilxx/ReadLine.h"
#include "utilxx/charbuf.h"
#include <chrono>
#include <logging.h>
#include <mutex>
#include <nos/io/serial_port.h>

void rs232listener_func();

class Synchronizer;

class RS232Listener
{
public:
    nos::serial_port file = {};
    igris::ReadLine<128> rl = {};

private:
    std::string path = {};
    // std::mutex mtx = {};
    // std::mutex qmtx = {};
    charbuf<14> answer = {};
    volatile uint8_t status = {};
    int64_t brokenPackageTotal = 0;
    uint8_t waitanswer = 0;
    struct timespec com_last_time = {};
    struct timespec com_now_time = {};
    int mode = 0;

public:
    Synchronizer *point_information_listener = nullptr;

public:
    RS232Listener()
    {
        rl.init();
    }
    void open(const char *str);
    void close();
    void rawSend(const char *str, uint8_t len);
    uint8_t checksum(char *str);
    uint8_t checksum(const char *str, uint8_t len);
    charbuf<14> Form(uint8_t code, uint32_t data);
    int rawTimeredRecv(char *data, int len);
    std::string rawQuery(const char *str);
    bool check_correct_frame(std::string str);
    std::string Query(const char *str);
    void exec(char c);
    void impulse(int xy, int32_t imp);
    void speed(int xy, int32_t imp);
    void stop(int xy);
    void scan_start(int xy);
    void cmpscan_start(int xy);
    void enc_numer(int xy, uint32_t arg);
    void gen_numer(int xy, uint32_t arg);
    void enc_denum(int xy, uint32_t arg);
    void gen_denum(int xy, uint32_t arg);
    void spdmul_numer(int xy, uint32_t arg);
    void spdmul_denum(int xy, uint32_t arg);
    int32_t get_encoder_position(int xy);
    void incpos_strt_zone(int xy, uint32_t arg);
    void incpos_stop_zone(int xy, uint32_t arg);
    void incpos_move(int xy, uint32_t arg);
    void incpos_points(int xy, uint32_t arg);
    void generate_trigger(int xy);
    void manual_trigger_mode(int xy, uint32_t arg);
    int32_t get_return_time(int xy, uint32_t arg);
    void wait_trigger_mode(int xy, uint32_t arg);
    void bitrigmode(int xy, uint32_t arg);
    std::string test(int xy);
    void eventctr_table_reset(int xy, uint32_t size);
    void eventctr_table_reset_prefix(int xy, uint32_t size);
    void eventctr_table_reset_cycle(int xy, uint32_t size);
    void eventctr_table_reset_postfix(int xy, uint32_t size);
    void eventctr_set_loop_total(int xy, uint32_t arg);
    void eventctr_push_back(int xy, uint32_t no);
    void eventctr_save_table(int xy, uint32_t saveno);
    void eventctr_load_table(int xy, uint32_t saveno);
    void clear_compare_table(int xy);
    void add_compare_table_point(int xy, uint32_t arg);
    void add_compare_table(int xy, const std::vector<double> &points);
    void enable_compare_table(int xy, uint32_t arg);
    void reset(int xy, uint32_t arg);
    void enable_external_trigger_mode(int xy);

    void com_trigger_polarity(int val);
    void com_trigger_duration(int val);
};

extern RS232Listener drv;

#endif
