/**
   @file RS232Licstener.cpp

   @comment В коде используется два мьютекса. Один синхронизует запрос, а
   другой процедуру чтения данных из порта. Это связано с тем, что кроме
   протокола обмена, используется передача сообщений методом инкрементирующегося
   байта..
*/

#define NOTRACE 1

#include <PointInformationBuffer.h>
#include <RS232Listener.h>
#include <Synchronizer.h>
#include <chrono>
#include <exception.h>
#include <igris/container/ring.h>
#include <igris/util/dstring.h>
#include <igris/util/numconvert.h>
#include <igris/util/string.h>
#include <iostream>
#include <memory>
#include <nos/log/logger.h>
#include <nos/trace.h>
#include <thread>
#include <utilxx/ascii_convert.h>

//#include <igris/serial/serial.h>
// RS232Listener drv;

#ifdef __WIN32__
#define O_NOCTTY 0
#endif

using namespace std;
// using namespace serial;

int rs232_packages = 0;

static inline int64_t longtime(struct timespec *t)
{
    return t->tv_nsec + t->tv_sec * 1000000000;
}

void RS232Listener::open(const char *str)
{
    nos::log::info("try port open: {}", str);
    int sts = file.open(str, 115200, 'e', 8, 1);
    nos::log::info("done");
    path = str;

    if (sts)
    {
        nos::println("open error: {}", str);
        exit(-1);
    }
}

void RS232Listener::close()
{
    file.close();
}

uint8_t RS232Listener::checksum(const char *str, uint8_t len)
{
    int sum = 0;
    const char *end = str + len;
    while (str != end)
    {
        sum += *str++;
    };
    return sum;
}

charbuf<14> RS232Listener::Form(uint8_t code, uint32_t data)
{
    charbuf<14> cbuf;
    char *buf = cbuf.to_buf();
    cbuf[0] = 1;
    cbuf[13] = 3;
    uint8_hcode(buf + 1, code);
    uint32_hcode(buf + 3, data);
    uint8_hcode(buf + 11, checksum(buf + 1, 10));
    return cbuf;
}

void RS232Listener::rawSend(const char *buf, uint8_t len)
{

    auto ret = file.write(buf, len);
    if (ret.is_error())
    {
        perror("file.write");
    }
}

int RS232Listener::rawTimeredRecv(char *data, int len)
{
    waitanswer = true;
    auto strt = std::chrono::system_clock::now();

    while (rs232_packages == 0)
    {
        this_thread::sleep_for(std::chrono::microseconds(100));
        if (std::chrono::system_clock::now() - strt > 100ms)
            return 0;
    }

    waitanswer = false;

    const char *str = rl.c_str();
    const char *epack = strchr(str, '\x03');

    if (epack == nullptr)
    {
        nos::println("RSListener: START byte is not found");
        return 0;
    }

    memcpy(data, str, epack - str + 1);

    return epack - str + 1;
}

std::string RS232Listener::rawQuery(const char *str)
{
    char buf[128];

    rl.init();
    rs232_packages = 0;
    nos::println("rawSend:", nos::buffer(str, strlen(str)));
    rawSend(str, strlen(str));
    int len = rawTimeredRecv(buf, 128);

    return std::string(buf, len);
}

bool RS232Listener::check_correct_frame(std::string str)
{
    const char *buf = str.c_str();
    if (str.size() != 14)
    {
        nos::println("str.size() != 14");
        return false;
    }
    if (*buf != 2)
    {
        nos::println("*buf != 2");
        return false;
    }
    if (checksum(buf + 1, 10) != hexcode_to_uint8(buf + 11))
    {
        nos::println("checksum error");
        nos::println("chs: ", checksum(buf + 1, 10));
        nos::println("ctr: ", hexcode_to_uint8(buf + 11));
        return false;
    }
    return true;
}

std::string RS232Listener::Query(const char *str)
{
    // std::lock_guard<std::mutex> lk(qmtx);
    bool status = false;
    for (int i = 0; i < 8; i++)
    {
        std::string ans = rawQuery(str);
        status = check_correct_frame(ans);
        if (status == true)
        {
            return ans;
        }
        else
        {
            if (ans.size() != 0)
                brokenPackageTotal++;
            auto ret = file.write("\003", 1);

            if (ret.is_error())
            {
                perror("file.write(\003,1)");
            }

            auto diff = std::chrono::system_clock::now() - last_error_time;
            if (diff > 1s)
            {
                auto msg = nos::format("wrongpack: q:{} a:{} as:{}",
                                       igris::dstring(str, strlen(str)),
                                       igris::dstring(ans),
                                       ans.size());
                if (count_of_skipped_errors > 1)
                {
                    msg +=
                        nos::format(" (+skipped: {})", count_of_skipped_errors);
                }
                count_of_skipped_errors = 0;
                nos::log::warn(msg);
                last_error_time = std::chrono::system_clock::now();
            }
            else
            {
                count_of_skipped_errors++;
            }
        }
    }
    nos::log::warn("error on request: {}", str);
    throw DeviceRefuseException("DeviceRefuse: RS232Listener::Query");
}

void RS232Listener::exec(char c)
{
    // std::lock_guard<std::mutex> lk(mtx);
    if (waitanswer == true)
    {
        rl.putc(c);

        if (c == 3)
        {
            rs232_packages++;
        }
    }
}

//Нижеследующие функции уместнее было бы реализовать в виде одной функции и
//карты регистров. Может быть неактуально в свете вероятной смены протокола.
void RS232Listener::impulse(int xy, int32_t imp)
{
    auto mes = Form(xy * 128 + 3, imp);
    Query(mes.to_buf());
}

void RS232Listener::speed(int xy, int32_t spd)
{
    auto mes = Form(xy * 128 + 4, spd);
    Query(mes.to_buf());
}

void RS232Listener::stop(int xy)
{
    auto mes = Form(xy * 128 + 1, 0);
    Query(mes.to_buf());
}

void RS232Listener::scan_start(int xy)
{
    auto mes = Form(xy * 128 + 14, 0);
    Query(mes.to_buf());
}

void RS232Listener::cmpscan_start(int xy)
{
    nos::println("cmpscan_start:");
    auto mes = Form(xy * 128 + 15, 0);
    Query(mes.to_buf());
}

void RS232Listener::enc_numer(int xy, uint32_t arg)
{
    nos::println("RS232Listener::enc_numer", xy, arg);
    auto mes = Form(xy * 128 + 22, arg);
    Query(mes.to_buf());
}

void RS232Listener::gen_numer(int xy, uint32_t arg)
{
    nos::println("RS232Listener::gen_numer", xy, arg);
    auto mes = Form(xy * 128 + 20, arg);
    Query(mes.to_buf());
}

void RS232Listener::enc_denum(int xy, uint32_t arg)
{
    nos::println("RS232Listener::enc_denum", xy, arg);
    auto mes = Form(xy * 128 + 23, arg);
    Query(mes.to_buf());
}

void RS232Listener::gen_denum(int xy, uint32_t arg)
{
    nos::println("RS232Listener::gen_denum", xy, arg);
    auto mes = Form(xy * 128 + 21, arg);
    Query(mes.to_buf());
}

void RS232Listener::spdmul_numer(int xy, uint32_t arg)
{
    nos::println("RS232Listener::spdmul_numer", xy, arg);
    auto mes = Form(xy * 128 + 18, arg);
    Query(mes.to_buf());
}

void RS232Listener::spdmul_denum(int xy, uint32_t arg)
{
    nos::println("RS232Listener::spdmul_denum", xy, arg);
    auto mes = Form(xy * 128 + 19, arg);
    Query(mes.to_buf());
}

void RS232Listener::incpos_strt_zone(int xy, uint32_t arg)
{
    nos::println("RS232Listener::incpos_strt_zone", xy, arg);
    auto mes = Form(xy * 128 + 24, arg);
    Query(mes.to_buf());
}

int32_t RS232Listener::get_encoder_position(int xy)
{
    auto mes = Form(xy * 128 + 56, 0);
    std::string ret = Query(mes.to_buf());
    std::string str = ret.substr(3, 8);

    uint32_t val = hex_to_uint32(str.c_str());
    int32_t vval = val;
    return vval;
}

void RS232Listener::incpos_stop_zone(int xy, uint32_t arg)
{
    nos::println("RS232Listener::incpos_stop_zone", xy, arg);
    auto mes = Form(xy * 128 + 25, arg);
    Query(mes.to_buf());
}

void RS232Listener::incpos_move(int xy, uint32_t arg)
{
    nos::println("RS232Listener::incpos_move", xy, (int32_t)arg);
    auto mes = Form(xy * 128 + 26, arg);
    Query(mes.to_buf());
}

void RS232Listener::incpos_points(int xy, uint32_t arg)
{
    nos::println("RS232Listener::incpos_points", xy, arg);
    auto mes = Form(xy * 128 + 27, arg);
    Query(mes.to_buf());
}

std::string RS232Listener::test(int xy)
{

    auto mes = Form(64, 0);
    std::string ret = Query(mes.to_buf());
    return ret;
}

void RS232Listener::generate_trigger(int xy)
{
    auto mes = Form(63, 0);
    // Query(mes.to_buf());
    auto b = mes.to_buf();
    rawSend(b, strlen(b));
}

void RS232Listener::manual_trigger_mode(int xy, uint32_t arg)
{
    auto mes = Form(62, arg);
    Query(mes.to_buf());
}

int32_t RS232Listener::get_return_time(int xy, uint32_t arg)
{
    auto mes = Form(61, arg);
    auto ret = Query(mes.to_buf());

    std::string str = ret.substr(3, 8);

    return strtol(str.c_str(), nullptr, 16);
}

void RS232Listener::wait_trigger_mode(int xy, uint32_t arg)
{
    auto mes = Form(28, arg);
    Query(mes.to_buf());
}

void RS232Listener::bitrigmode(int xy, uint32_t arg)
{
    auto mes = Form(29, arg);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_table_reset(int xy, uint32_t size)
{
    auto mes = Form(66, size);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_table_reset_prefix(int xy, uint32_t size)
{
    auto mes = Form(69, size);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_table_reset_cycle(int xy, uint32_t size)
{
    auto mes = Form(70, size);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_table_reset_postfix(int xy, uint32_t size)
{
    auto mes = Form(71, size);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_set_loop_total(int xy, uint32_t size)
{
    auto mes = Form(72, size);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_push_back(int xy, uint32_t no)
{
    auto mes = Form(65, no);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_save_table(int xy, uint32_t no)
{
    auto mes = Form(67, no);
    Query(mes.to_buf());
}

void RS232Listener::eventctr_load_table(int xy, uint32_t no)
{
    auto mes = Form(68, no);
    Query(mes.to_buf());
}

void RS232Listener::start_listener_thread()
{
    std::thread rs232_thread(&RS232Listener::rs232listener_func, this);
    rs232_thread.detach();
}

char RS232Listener::rsymb()
{
    char c;
    auto ret = file.read(&c, 1);

    if (ret.is_error())
    {
        nos::println("rsymb::ret", *ret);
        exit(0);
    }

    return c;
}

// TODO
#include "STUFF.h"
int scanPointNumber = 0;
igris::multiple_delegate<int> g_scanPointSignal;
igris::multiple_delegate<> descanSignal;
igris::multiple_delegate<> wrongScanPointSignal;

int32_t external_trigger_number = 0;
bool start_point_information = false;
bool start_point_information_2 = false;
std::string point_information = {};
std::string point_information_2 = {};

// PointInformationBuffer point_information_buffer;
void handle_point_information(std::string information)
{
    try
    {
        //     int64_t dbl = std::stol(information);
        //     //    point_information_buffer.push(dbl);
        //     if (drv.point_information_listener)
        //         drv.point_information_listener->push_point_information(dbl);
    }
    catch (...)
    {
    }
}

void handle_external_trigger_point_information(std::string information)
{
    nos::println(information);

    try
    {
        // int64_t dbl = std::stol(information);
        if (true) // drv.point_information_listener)
        {
            nos::println("TODO: point_information_listener add point");
            // drv.point_information_listener->push_point_information(dbl);
        }
    }
    catch (...)
    {
    }
}

void RS232Listener::rs232listener_func()
{
    char c;
    char sparc_lsymb = 'Z';
    char sparc_lsymb2 = 'M';
    char sparc_lsymb3 = 'R';
    char sparc_lsymb4 = 'V';
    struct timespec sparc_last_time;
    struct timespec sparc_last_time2;
    struct timespec sparc_last_time3;
    struct timespec sparc_last_time4;
    struct timespec sparc_now_time;

    int32_t external_trigger_number = 0;

    clock_gettime(CLOCK_REALTIME, &sparc_now_time);
    clock_gettime(CLOCK_REALTIME, &sparc_last_time);
    clock_gettime(CLOCK_REALTIME, &sparc_last_time2);
    clock_gettime(CLOCK_REALTIME, &sparc_last_time3);

    rl.init();

    while (1)
    {
        c = rsymb();

        clock_gettime(CLOCK_REALTIME, &sparc_now_time);

        // NewPoint
        if (c == 'W' || c == 'X')
        {
            start_point_information = true;
            if (c < sparc_lsymb)
            {
                ++scanPointNumber;
                g_scanPointSignal(scanPointNumber);
                nos::log::info("ScanSparc {}, {} \r\n",
                               scanPointNumber,
                               ((double)longtime(&sparc_now_time) -
                                (double)longtime(&sparc_last_time)) /
                                   1000000);
                sparc_last_time = sparc_now_time;
            };
            sparc_lsymb = c;
            continue;
        };

        if (c == 'Y' || c == 'Z')
        {
            start_point_information = false;
            handle_point_information(point_information);
            point_information = "";
            sparc_lsymb = c;
        }

        if (c == 'S' || c == 'T')
        {
            start_point_information_2 = true;
            if (c < sparc_lsymb4)
            {
                ++external_trigger_number;
                // g_scanPointSignal(scanPointNumber);
                nos::log::info("ExternalTrigger {}, {} \r\n",
                               external_trigger_number,
                               ((double)longtime(&sparc_now_time) -
                                (double)longtime(&sparc_last_time)) /
                                   1000000);
                sparc_last_time4 = sparc_now_time;
            };
            sparc_lsymb4 = c;
            continue;
        };

        if (c == 'U' || c == 'V')
        {
            start_point_information_2 = false;
            handle_external_trigger_point_information(point_information_2);
            point_information_2 = "";
            sparc_lsymb4 = c;
        }

        // StopCtr
        if (c == 'J' || c == 'K' || c == 'L' || c == 'M')
        {
            if (c < sparc_lsymb2)
            {
                sparc_last_time2 = sparc_now_time;
                std::thread(descanSignal).detach();
                nos::log::info("StopCtr");
            };
            sparc_lsymb2 = c;
            continue;
        };

        // Wrong Scan Trigger
        if (c == 'N' || c == 'O' || c == 'P' || c == 'R')
        {
            if (c < sparc_lsymb3)
            {
                sparc_last_time3 = sparc_now_time;
                std::thread(wrongScanPointSignal).detach();
                nos::log::info("WrongScanPoint");
            };
            sparc_lsymb3 = c;
            continue;
        };

        if (start_point_information)
        {
            point_information += c;
            continue;
        }

        if (start_point_information_2)
        {
            point_information_2 += c;
            continue;
        }

        exec(c);
    };
}

void RS232Listener::clear_compare_table(int xy)
{
    auto mes = Form(xy * 128 + 78, 1);
    Query(mes.to_buf());
}

void RS232Listener::add_compare_table_point(int xy, uint32_t no)
{
    auto mes = Form(xy * 128 + 79, no);
    Query(mes.to_buf());
}

void RS232Listener::enable_compare_table(int xy, uint32_t no)
{
    auto mes = Form(xy * 128 + 80, no);
    Query(mes.to_buf());
}

void RS232Listener::reset(int xy, uint32_t no)
{
    nos::println("RS232Listener::reset", xy, no);
    auto mes = Form(81, no);
    Query(mes.to_buf());
}

void RS232Listener::add_compare_table(int xy, const std::vector<double> &points)
{
    for (auto p : points)
        add_compare_table_point(xy, p);
}

void RS232Listener::enable_external_trigger_mode(int xy)
{
    // Что такое external_trigger?
    external_trigger_number = 0;
    auto mes = Form(xy * 128 + 57, 1);
    Query(mes.to_buf());
}

void RS232Listener::com_trigger_polarity(int val)
{
    auto mes = Form(82, val);
    Query(mes.to_buf());
}

void RS232Listener::com_trigger_duration(int val)
{
    auto mes = Form(83, val);
    Query(mes.to_buf());
}