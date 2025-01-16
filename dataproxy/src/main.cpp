#define NOTRACE 1

#include <NetworkAnalyzer.h>
#include <buffered_channels.h>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstring>
#include <getopt.h>
#include <igris/dprint.h>
#include <igris/time/systime.h>
#include <main.h>
#include <mutex>
#include <nos/argparse.h>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <nos/io/buffer_reader.h>
#include <nos/io/stdfile.h>
#include <nos/print.h>
#include <nos/trent/json.h>
#include <nos/trent/trent.h>
#include <queue>
#include <signal.h>
#include <sstream>
#include <string>
#include <thread>
#include <version.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool DUMP_BLOCKS = false;

int axnum;
int before_package_delay = 0;

int packageindex;

enum class GetMode
{
    AmpPhs,
    RawMode,
    BufferMode,
    BufferedChannels,
    LocalMode,
    SignalAnalyzerBasic,
    SignalAnalyzerMarker
};

GetMode getmode = GetMode::AmpPhs;
// bool rawget = false;

std::mutex netanlock;

int points;
int packno;
std::string netantype;
std::vector<std::pair<std::string, std::string>> traces;

int hello(const nos::trent &tr);
int connect(const nos::trent &tr);
int disconnect();

nos::inet::tcp_client informer;
nos::inet::tcp_client client;

std::queue<
    std::pair<std::shared_ptr<std::string>, std::shared_ptr<std::string>>>
    datq;
std::mutex datmutx;

std::string last_sended_amp;
std::string last_sended_phs;

NetworkAnalyzer netan;
bool discStart = false;

float swapbytes(float f)
{
    union
    {
        float ret;
        uint8_t retarr[4];
    };

    union
    {
        float in;
        uint8_t inarr[4];
    };

    in = f;

    retarr[3] = inarr[0];
    retarr[2] = inarr[1];
    retarr[1] = inarr[2];
    retarr[0] = inarr[3];

    return ret;
}

int hello(const nos::trent &tr)
{
    nos::println("received trent:", tr);
    return 0;
}

int k = 0;
void listenerThread_BufferMode_Func(void *)
{
    try
    {
        nos::println("BufferMode active");

        char buf[128];
        int sts;

        if (netan.send("FORM REAL,32") < 0)
            goto __error__;
        if (netan.send("FORM:BORD SWAP") < 0)
            goto __error__;
        ;
        nos::fprintln("idn request: {}", netan.query("*IDN?"));

        while (true)
        {
            if (discStart)
                return;

            auto exret = nos::read_until_from(informer, {buf, 128}, "\n");

            if (exret.is_error())
                goto __error__;

            int ret = *exret;

            std::this_thread::sleep_for(
                std::chrono::milliseconds(before_package_delay));

            if (ret <= 0)
            {
                goto __error__;
            }
            if (discStart)
                return;

            std::shared_ptr<std::string> ampdata;
            if (netantype == "pna")
            {
                std::vector<rawtype> storage;

                for (int trace_index = 0; trace_index < traces.size();
                     ++trace_index)
                {
                    netanlock.lock();
                    int repeats = 0;
                __readdata__:
                    if (netan.send(
                            nos::format("SYST:FIFO:DATA? {}", points).c_str()) <
                        0)
                        goto __error__;

                    ampdata = netan.read_block();

                    if (!ampdata)
                    {
                        goto __error__;
                    }

                    //				if (ampdata->size() == 0 && repeats < 5)
                    //				{
                    //					nos::println("nulldata. another one
                    // request."); 					repeats++; goto
                    // __readdata__;
                    //				}

                    netanlock.unlock();

                    storage.push_back(ampdata);
                }

                datmutx.lock();
                for (int i = 0; i < storage.size(); ++i)
                {
                    datq.push(std::make_pair(storage[i], storage[i]));
                }
                datmutx.unlock();
            }
            else if (netantype == "ena")
            {
                dprln("ena point");
            }
            else
            {
                dprln("wrong netan type");
            }
        }
    }
    catch (const std::exception &ex)
    {
        goto __error__;
    }

__error__:
    try
    {
        client.write("err", 3);
    }
    catch (const std::exception &ex)
    {
    }
    dprln("listenerThreadFunc::dataerror");
    disconnect();
    dprln("listenerThreadFunc::after_disc");
    return;
}

std::string raw_mode_parameter_choose(const std::string &netantype,
                                      const std::string &tracearg)
{
    if (netantype == "pna")
    {
        return nos::format("CALC1:PAR:SEL \'{}\'", tracearg);
    }
    else if (netantype == "planar")
    {
        return nos::format("CALC1:PAR{}:SEL", tracearg);
    }
    else
        throw std::runtime_error("unresolved netan type");
}

std::string raw_mode_getdata_choose(const std::string &netantype)
{
    if (netantype == "pna")
    {
        return "CALC1:DATA? SDATA";
    }
    else if (netantype == "planar")
    {
        return "CALC1:DATA:SDAT?";
    }
    else
        throw std::runtime_error("unresolved netan type");
}

std::string raw_mode_formatdata_choose(const std::string &netantype)
{
    if (netantype == "pna")
    {
        return "FORM REAL,32";
    }
    else if (netantype == "planar")
    {
        return "FORM:DATA REAL32";
    }
    else
        throw std::runtime_error("unresolved netan type");
}

void listenerThread_RawMode_Func(void *)
{
    __label__ __error__;

    try
    {
        nos::println("RawMode active");
        static std::shared_ptr<std::string> last_ampdata;

        char buf[128];
        int sts;

        if (netan.send(raw_mode_formatdata_choose(netantype).c_str()) < 0)
            goto __error__;

        while (true)
        {
            if (discStart)
                return;

            auto ret = read_until_from(informer, {buf, 128}, "\n");
            if (ret.is_ok() && *ret > 0)
                nos::println(nos::buffer(buf, *ret));

            if (before_package_delay != 0)
            {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(before_package_delay));
            }

            if (ret.is_error())
            {
                goto __error__;
            }
            if (discStart)
                return;

            std::shared_ptr<std::string> ampdata;
            std::vector<rawtype> storage;

            for (int trace_index = 0; trace_index < traces.size();
                 ++trace_index)
            {
                netanlock.lock();
                if (netan.send(raw_mode_parameter_choose(
                                   netantype, traces[trace_index].first)
                                   .c_str()) < 0)
                    goto __error__;
                if (netan.send(raw_mode_getdata_choose(netantype).c_str()) < 0)
                    goto __error__;

                ampdata = netan.read_block();
                if (last_ampdata && *ampdata == *last_ampdata)
                {
                    nos::println("ampdata equaled last_ampdata");
                }
                last_ampdata = ampdata;

                if (!ampdata)
                {
                    goto __error__;
                }

                netanlock.unlock();

                storage.push_back(ampdata);
            }

            datmutx.lock();
            for (int i = 0; i < storage.size(); ++i)
            {
                datq.push(std::make_pair(storage[i], storage[i]));
            }
            datmutx.unlock();
        }
    }
    catch (const std::exception &ex)
    {
        goto __error__;
    }

__error__:

    try
    {
        client.write("err", 3);
    }
    catch (const std::exception &ex)
    {
    }

    try
    {
        disconnect();
    }
    catch (const std::exception &ex)
    {
    }

    return;
}

void listenerThread_LocalMode_Func(void *)
{
    try
    {
        nos::println("LocalMode active");

        char buf[128];
        int sts;

        while (true)
        {
            if (discStart)
                return;

            nos::println("Warn: Maybe need to remove INIT:IMM;*OPC?");
            netan.query("INIT:IMM;*OPC?");

            std::shared_ptr<std::string> ampdata;
            if (netantype == "pna")
            {
                std::vector<rawtype> storage;

                for (int trace_index = 0; trace_index < traces.size();
                     ++trace_index)
                {
                    netanlock.lock();
                    if (netan.send(nos::format("CALC1:PAR:SEL \'{}\'",
                                               traces[trace_index].first)
                                       .c_str()) < 0)
                        goto __error__;
                    if (netan.send("CALC1:DATA:NSW:FIRS? SDATA, 1") < 0)
                        goto __error__;

                    ampdata = netan.read_block();
                    if (!ampdata)
                    {
                        goto __error__;
                    }

                    netanlock.unlock();

                    storage.push_back(ampdata);
                }

                datmutx.lock();
                for (int i = 0; i < storage.size(); ++i)
                {
                    datq.push(std::make_pair(storage[i], storage[i]));
                }
                datmutx.unlock();
            }
            else if (netantype == "ena")
            {
                dprln("ena point");
            }
            else
            {
                dprln("wrong netan type");
            }
        }
    }
    catch (const std::exception &ex)
    {
        goto __error__;
    }

__error__:
    try
    {
        client.write("err", 3);
    }
    catch (const std::exception &ex)
    {
    }
    dprln("listenerThreadFunc::dataerror");
    disconnect();
    dprln("listenerThreadFunc::after_disc");
    return;
}

void listenerThread_AmpPhs_Func(void *)
{
    nos::println("AmpPhs mode active");

    char buf[128];
    int sts;

    if (netan.send("FORM REAL,32") < 0)
        goto __error__;
    if (netan.send("FORM:BORD SWAP") < 0)
        goto __error__;
    ;

    while (true)
    {
        if (discStart)
            return;
        auto exret = nos::read_until_from(informer, {buf, 128}, "\n");

        if (exret.is_error())
            goto __error__;

        int ret = *exret;

        if (ret == 0)
        {
            goto __error__;
        }
        if (discStart)
            return;

        std::shared_ptr<std::string> ampdata;
        std::shared_ptr<std::string> phsdata;

        if (netantype == "pna")
        {
            std::vector<ptype> storage;

            for (int trace_index = 0; trace_index < traces.size();
                 ++trace_index)
            {
                netanlock.lock();
                if (netan.send(nos::format("CALC1:PAR:SEL \'{}\'",
                                           traces[trace_index].first)
                                   .c_str()) < 0)
                    goto __error__;
                if (netan.send("CALC1:DATA? FDATA") < 0)
                    goto __error__;

                ampdata = netan.read_block();
                if (!ampdata)
                {
                    goto __error__;
                }

                if (netan.send(nos::format("CALC1:PAR:SEL \'{}\'",
                                           traces[trace_index].second)
                                   .c_str()) < 0)
                    goto __error__;
                if (netan.send("CALC1:DATA? FDATA") < 0)
                    goto __error__;

                phsdata = netan.read_block();
                if (!phsdata)
                {
                    goto __error__;
                }
                netanlock.unlock();

                storage.push_back(std::make_pair(ampdata, phsdata));
            }

            datmutx.lock();
            for (int i = 0; i < storage.size(); ++i)
            {
                datq.push(storage[i]);
            }
            datmutx.unlock();
        }
        else if (netantype == "ena")
        {
            dprln("ena point");
        }
        else
        {
            dprln("wrong netan type");
        }
    }

__error__:
    client.write("err", 3);
    dprln("listenerThreadFunc::dataerror");
    disconnect();
    dprln("listenerThreadFunc::after_disc");
    return;
}

bool need_convert()
{
    switch (getmode)
    {
    case GetMode::AmpPhs:
        return false;
    case GetMode::RawMode:
        return true;
    case GetMode::BufferMode:
        return true;
    case GetMode::BufferedChannels:
        return true;
    case GetMode::LocalMode:
        return true;
    case GetMode::SignalAnalyzerBasic:
        return false;
    default:
        throw std::runtime_error("Unresolved Mode");
    }
}

bool need_swap()
{
    switch (getmode)
    {
    case GetMode::AmpPhs:
        return false;
    case GetMode::RawMode:
        return true;
    case GetMode::BufferedChannels:
        return true;
    case GetMode::BufferMode:
        return false;
    case GetMode::LocalMode:
        return false;
    case GetMode::SignalAnalyzerBasic:
        return false;
    default:
        throw std::runtime_error("Unresolved Mode");
    }
}

// return : true -> error
bool send_ampphs_strings(const std::string &amp, const std::string &phs)
{
    if (last_sended_amp == amp)
        nos::println("Last sended amp equaled");

    if (last_sended_phs == phs)
        nos::println("Last sended phs equaled");

    last_sended_amp = amp;
    last_sended_phs = phs;

    int32_t fsize = amp.size();
    int32_t ssize = phs.size();

    if (DUMP_BLOCKS)
    {
        nos::println("send ampphs: ampsize:{} phssize:{}", fsize, ssize);
    }

    if (client.write((char *)&packageindex, sizeof(int32_t)).is_error())
        return true;
    if (client.write("amp", 3).is_error())
        return true;
    if (client.write((char *)&fsize, sizeof(int32_t)).is_error())
        return true;
    if (client.write(amp.data(), fsize).is_error())
        return true;
    if (client.write("phs", 3).is_error())
        return true;
    if (client.write((char *)&ssize, sizeof(int32_t)).is_error())
        return true;
    if (client.write(phs.data(), ssize).is_error())
        return true;
    if (client.write("end", 3).is_error())
        return true;

    packageindex++;
    return false;
}

bool flag_dataSenderFuncFinish = true;
bool flag = false;
void dataSenderFunc(void *)
{
    nos::println("start dataSenderFunc");

    int sz;
    while (true)
    {
        if (flag_dataSenderFuncFinish)
            return;

        datmutx.lock();
        sz = datq.size();
        if (sz)
        {
            std::pair<std::shared_ptr<std::string>,
                      std::shared_ptr<std::string>>
                pair = datq.front();
            datq.pop();
            datmutx.unlock();

            if (need_swap())
            {
                float *fptr = (float *)pair.first->data();
                size_t sz = pair.first->size() / sizeof(float);

                for (int i = 0; i < sz; ++i)
                {
                    fptr[i] = swapbytes(fptr[i]);
                }
            }

            if (need_convert())
            {
                std::complex<float> *cptr =
                    (std::complex<float> *)pair.first->data();

                int sz = pair.first->size() / (sizeof(float) * 2);

                std::string amp;
                std::string phs;

                amp.resize(sz * sizeof(float));
                phs.resize(sz * sizeof(float));

                float *ampptr = (float *)amp.data();
                float *phsptr = (float *)phs.data();

                for (int i = 0; i < sz; ++i)
                {
                    *ampptr = std::abs(*cptr);
                    *phsptr = std::arg(*cptr);

                    *ampptr = 20 * std::log10(*ampptr);

                    double rphs = *phsptr;
                    *phsptr += M_PI;
                    int rounds = std::floor(*phsptr / (2 * M_PI));
                    *phsptr -= 2 * M_PI * rounds;
                    *phsptr -= M_PI;
                    *phsptr *= 180.0 / M_PI;

                    ampptr++;
                    phsptr++;
                    cptr++;
                }

                bool is_error = send_ampphs_strings(amp, phs);
                if (is_error)
                    goto __error__;
            }

            else
            {
                bool is_error = send_ampphs_strings(*pair.first, *pair.second);
                if (is_error)
                    goto __error__;
            }
        }
        else
        {
            datmutx.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

__error__:
    dprln("dataSenderFunc::dataerror");
    return;
}

std::thread listenerThread;
std::thread dataSenderThread;

void set_delay(const nos::trent &tr)
{
    before_package_delay = tr["msdelay"].as_integer();
    set_buffered_channels_timelapse(before_package_delay);
}

void get_version(const nos::trent &tr)
{
    nos::println_to(client, VERSION);
}

int connect(const nos::trent &tr)
{
    nos::log::debug("connect method invoked");
    packageindex = 0;
    netantype = tr["type"].as_string();
    if (netantype == "zva")
        netantype = "pna";

    std::string netan_ip = tr["netan_ip"].as_string();
    int netan_port = tr["netan_port"].as_integer();

    std::string scan_ip = tr["scan_ip"].as_string();
    int scan_port = tr["scan_port"].as_integer();

    axnum = tr["axnum"].as_integer();
    std::string gmstr = tr["getmode"].as_string();

    points = tr["points"].as_integer();

    nos::log::debug("choose mode");
    if (gmstr == "AmpPhs")
        getmode = GetMode::AmpPhs;
    else if (gmstr == "RawMode")
        getmode = GetMode::RawMode;
    else if (gmstr == "BufferedChannels")
        getmode = GetMode::BufferedChannels;
    else if (gmstr == "BufferMode")
        getmode = GetMode::BufferMode;
    else if (gmstr == "LocalMode")
        getmode = GetMode::LocalMode;
    else if (gmstr == "SignalAnalyzerBasic")
        getmode = GetMode::SignalAnalyzerBasic;
    else if (gmstr == "SignalAnalyzerMarker")
        getmode = GetMode::SignalAnalyzerMarker;
    else
    {
        nos::println("wrong getmode");
        return -1;
    }

    nos::trent trace_trent = tr["traces"];
    traces.resize(0);
    for (auto &tr_trace : trace_trent.as_list())
    {
        traces.push_back(std::make_pair(tr_trace.as_list()[0].as_string(),
                                        tr_trace.as_list()[1].as_string()));
    }

    while (!datq.empty())
        datq.pop();
    if (flag)
        return 0;

    if (informer.connect(scan_ip, scan_port))
    {
        perror("2");
        return 0;
    }

    nos::log::debug("subscribe to server notification");
    // Подписываемся на уведомления от сервера.
    nos::fprint_to(informer, "NOT:AXIS{}:SCAN:POINT 0\n", axnum);
    nos::fprint_to(informer, "NOT:AXIS{}:SCAN:POINT 1\n", axnum);

    nos::log::debug("invoke network analyzer connect");
    int sts = netan.connect(netan_ip.c_str(), netan_port);

    if (sts)
    {
        perror("netan");
    }

    flag_dataSenderFuncFinish = false;
    discStart = false;

    switch (getmode)
    {
    case GetMode::AmpPhs:
        nos::log::debug("start AmpPhs controller");
        listenerThread = std::thread(listenerThread_AmpPhs_Func, nullptr);
        break;
    case GetMode::RawMode:
        nos::log::debug("start RawMode controller");
        listenerThread = std::thread(listenerThread_RawMode_Func, nullptr);
        break;
    case GetMode::BufferMode:
        nos::log::debug("start BufferMode controller");
        listenerThread = std::thread(listenerThread_BufferMode_Func, nullptr);
        break;
    case GetMode::BufferedChannels:
        nos::log::debug("start BufferChannels controller");
        start_buffered_channels_automate();
        break;
    case GetMode::LocalMode:
        nos::log::debug("start LocalMode controller");
        listenerThread = std::thread(listenerThread_LocalMode_Func, nullptr);
        break;
    case GetMode::SignalAnalyzerBasic:
        nos::log::debug("start SignalAnalyzerBasic controller");
        listenerThread =
            std::thread(listenerThread_SignalAnalyzerBasic_Func, nullptr);
        break;
    case GetMode::SignalAnalyzerMarker:
        nos::log::debug("start SignalAnalyzerMarker controller");
        listenerThread =
            std::thread(listenerThread_SignalAnalyzerMarker_Func, nullptr);
        break;
    }

    dataSenderThread = std::thread(dataSenderFunc, nullptr);

    flag = true;
    return 0;
}

int disconnect()
{
    if (discStart)
        return 0;

    datmutx.lock();
    netanlock.lock();

    discStart = true;
    flag_dataSenderFuncFinish = true;

    informer.close();
    netan.disconnect();

    netanlock.unlock();
    datmutx.unlock();

    if (getmode == GetMode::BufferedChannels)
    {
        try
        {
            stop_buffered_channels_automate();
        }
        catch (const std::system_error &ex)
        {
            std::cout << "stop_buffered_channels_automate except" << std::endl;
        }
    }
    else
    {
        try
        {
            listenerThread.join();
        }
        catch (const std::system_error &ex)
        {
            std::cout << "join: exception in listenerThread" << std::endl;
        }
    }

    try
    {
        dataSenderThread.join();
    }
    catch (const std::system_error &ex)
    {
        std::cout << "join: exception in dataSenderThread" << std::endl;
    }

    flag = false;
    return -1;
}

void clean()
{
    nos::println("clean");
    packno = 0;
    if (getmode == GetMode::BufferedChannels)
        buffered_channels_clean();
    datmutx.lock();
    packageindex = 0;
    datq = decltype(datq)();
    datmutx.unlock();
}

int main(int argc, char *argv[])
{
    nos::cout.flush_on_write(true);

    auto parser = nos::argparse(
        "dataproxy", "Utility for network analyzer data bufferization");
    parser.add_argument("blockdump", 'b', "Dump blocks to file");
    parser.add_argument("version", 'v', "Print version");
    auto args = parser.parse_args(argc, argv);

    if (!args)
        return 0;

    if (args.flag("blockdump"))
        DUMP_BLOCKS = true;

    if (args.flag("version"))
    {
        nos::println(VERSION);
        return 0;
    }

    nos::inet::tcp_server dataserver;
    dataserver.init();
    dataserver.reusing(true);

    if (dataserver.bind("0.0.0.0", 5028))
    {
        perror("dataserver.bind");
        exit(-1);
    }

    if (dataserver.listen())
    {
        perror("dataserver.listen");
        exit(-1);
    }

    while (true)
    {
        int sts;
        nos::println("start accepting");

        auto newclient = dataserver.accept();
        client = newclient;

        std::thread thr = std::thread([]() {
            client.nodelay(true);
            client.reusing(true);

            datq = decltype(datq)();

            nos::println("client: New connection");

            char buf[10000];
            while (true)
            {
                memset(buf, 0, sizeof(buf));
                auto reterr = nos::read_paired_from(
                    client, buf, std::size(buf), '{', '}');

                if (reterr.is_error())
                {
                    nos::println("client: Read error. Will be clossed.");
                    break;
                }

                int ret = *reterr;
                if (ret <= 0)
                {
                    nos::println("client: Read error. Will be clossed.");
                    break;
                }
                buf[ret] = 0;
                nos::fprintln("client: Input: {}", nos::buffer(buf, ret));

                // auto reader = std::stringstream(std::string(buf, ret));
                nos::trent tr = nos::json::parse(std::string(buf, ret));

                if (!tr.contains("cmd"))
                {
                    nos::println("Warn: trent without cmd");
                    continue;
                }

                PRINT(tr);
                nos::fprintln("cmd:{}", tr["cmd"].as_string());

                std::string cmd = tr["cmd"].as_string();

                if (cmd == "hello")
                    hello(tr);
                else if (cmd == "connect")
                    connect(tr);
                else if (cmd == "disconnect")
                    disconnect();
                else if (cmd == "clean")
                    clean();
                else if (cmd == "set_delay")
                    set_delay(tr);
                else if (cmd == "set_buffered_channels")
                    set_buffered_channels(tr);
                else if (cmd == "get_version")
                    get_version(tr);
                else
                {
                    nos::println("Unregistred function");
                }
            }

            nos::println("client: Close this connection.");
            client.close();
            if (flag)
                disconnect();
        });

        try
        {
            thr.detach();
        }
        catch (const std::system_error &e)
        {
            std::cout << "Caught system_error with code " << e.code()
                      << " meaning " << e.what() << '\n';
        }
    }
}
