#ifndef igris_LAZER_PROXY_H
#define igris_LAZER_PROXY_H

#include <condition_variable>
#include <deprecated/dataproxy.h>
#include <deprecated/scpi_error.h>

#include <nos/io/serial_port.h>

#if LAZER_EMULATION == 0

// using namespace igris::serial;

class LaserProxy : public DataProxy
{
public:
    bool started = false;
    std::thread measure_thread;

    std::mutex mutex;
    std::unique_lock<std::mutex> lock;
    std::condition_variable cvar;

    LaserProxy() : lock(mutex) {}

    void init(const std::string &path)
    {
        int sts;
        nos::fprintln("LAZER PROXY OPEN {}", path);
        // file.open(path.c_str());
        sts = lazport.open(path.c_str(), 115200, 'n', 8, 2);
        if (sts < 0)
        {
            perror("lazer open");
            exit(-1);
        }
    }

    bool stop_token;
    char DEV_ADDR = 1;
    nos::serial_port lazport;
    // Serial* lazport;

    uint16_t rawMeasRequest();
    double convert_to_mm(uint16_t RAW_VALUE);

    nos::expected<void, std::string> do_bind() override;
    void do_unbind() override;
    void do_measure(int num);
    nos::expected<double, std::string> rpc_do_measure_test();
    void measure_thread_function();
};

#else

struct LaserProxy : public DataProxy
{
    nos::expected<std::string> rpc_do_measure_test();
    nos::expected<void> do_bind() override;
    void do_unbind() override;
    void do_measure(int num);
    void init(const std::string &path) {}
};

#endif

#if (HAVE_LAZER_PROXY)
extern LaserProxy laser_proxy;
static LaserProxy &rangefinder = laser_proxy;
#endif

#endif
