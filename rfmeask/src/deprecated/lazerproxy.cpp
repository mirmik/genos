#include <deprecated/lazerproxy.h>
#include <utilxx/ascii_convert.h>

#include <chrono>
#include <fcntl.h>
#include <thread>
#include <unistd.h>

#include <nos/trace.h>

#if LAZER_EMULATION == 0 && (HAVE_LAZER_PROXY)

LaserProxy laser_proxy;

uint16_t LaserProxy::rawMeasRequest()
{

    int n = 0;
    char sendBuf[1];
    char readBuf[2];
    uint16_t rawValue = 0;
    uint8_t rawValueL = 0;
    uint8_t rawValueH = 0;

    readBuf[0] = 0;
    readBuf[1] = 0;
    sendBuf[0] = DEV_ADDR;

    n = lazport.write(sendBuf, 1);

    if (n != 1)
    {
        std::cout << "Transmition failure\n";
        return -1;
    }

    n = lazport.read(readBuf, 2);

    if (n == 0)
    {
        std::cout << "There is nothing..."
                  << "\n";
        return -1;
    }

    else
    {
        rawValueH = readBuf[0];
        rawValueL = readBuf[1];

        rawValue = rawValueH;
        rawValue = rawValue << 8;
        rawValue = rawValue | rawValueL;

        return rawValue;
    }
}

double LaserProxy::convert_to_mm(uint16_t RAW_VALUE)
{
    double dist_mm = 0;
    dist_mm = (((double)(RAW_VALUE - 2768)) * 400) / 60000;
    return dist_mm;
}

nos::expected<void, std::string> LaserProxy::do_bind()
{
    dataSpam.drop_all();
    cvar.notify_all();

    if (binded)
    {
        return std::string("Internal error: binded");
    }
    stop_token = false;

    current_axis->bind_scan_point_handler(
        igris::make_delegate(&LaserProxy::do_measure, this));
    measure_thread = std::thread(&LaserProxy::measure_thread_function, this);

    binded = true;

    return nos::expected<void, std::string>();
}

void LaserProxy::measure_thread_function()
{
    if (started != false)
        nos::log::warn("LaserProxy::measure_thread_function: started != false");

    started = true;

    while (true)
    {
        if (stop_token)
            break;
        cvar.wait(lock);
        if (stop_token)
            break;
        else
        {
            double result = convert_to_mm(rawMeasRequest());
            dataSpam.write((char *)&result, sizeof(double));
        }
    }
    stop_token = true;
    started = false;
    return;
}

void LaserProxy::do_unbind()
{
    if (binded)
    {
        stop_token = true;
        cvar.notify_one();

        measure_thread.join();
        binded = false;
        dataSpam.drop_all();
        current_axis->unbind_scan_point_handler(
            igris::make_delegate(&LaserProxy::do_measure, this));
    }
    dataSpam.drop_all();
    return;
}

void LaserProxy::do_measure(int num)
{
    cvar.notify_one();
}

nos::expected<double, std::string> LaserProxy::rpc_do_measure_test()
{
    double res = convert_to_mm(rawMeasRequest());
    // dataSpam.write((char*)&res, sizeof(double));
    return res;
}

#else

nos::expected<void> LaserProxy::do_bind()
{
    current_axis->scanPointSignal +=
        igris::make_delegate(&LaserProxy::do_measure, this);
    return nos::expected<void>();
}

void LaserProxy::do_unbind()
{
    current_axis->scanPointSignal -=
        igris::make_delegate(&LaserProxy::do_measure, this);
}

nos::expected<std::string> LaserProxy::rpc_do_measure_test()
{
    return "42";
}

void LaserProxy::do_measure(int num)
{
    double result = 42;
    dataSpam.write((char *)&result, sizeof(double));
}

#endif
