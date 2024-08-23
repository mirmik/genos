#ifndef NETWORK_ANALYZER_H
#define NETWORK_ANALYZER_H

#include <igris/dprint.h>
#include <memory>
#include <nos/inet/tcp_client.h>
#include <nos/input.h>
#include <nos/log.h>
#include <nos/print.h>
#include <nos/util/hexascii.h>
#include <igris/time/systime.h>
#include <stdio.h>

extern int packno;
extern bool DUMP_BLOCKS;

class NetworkAnalyzer
{
public:
    nos::inet::tcp_client sock;
    std::string _ip;
    int _port;

public:
    int connect(const std::string & ip, int port)
    {
        _ip = ip;
        _port = port;
        int ret = sock.connect(nos::inet::hostaddr(ip), port);
        sock.nodelay(true);
        sock.reusing(false);
        return ret;
    }

    std::string host_ip () { return _ip; }
    int host_port() { return _port; }

    int disconnect()
    {
        return sock.close();
    }

    int send(const char *str)
    {
        std::string sstr = str;
        nos::log::debug("SEND: {}", sstr);
        nos::print_to(sock, sstr + "\n\r");
        return 0;
    }

    std::string recv()
    {
        char buf[128];
        auto exret = nos::read_until_from(sock, {buf, 128}, "\n");

        if (exret.is_error())
            return "";

        int ret = *exret;

        buf[ret] = 0;

        auto result = std::string(buf, ret - 1);
        nos::log::debug("RECV: {}", result);
        return result;
    }

    std::string query(const char *str)
    {
        send(str);
        return recv();
    }

    std::shared_ptr<std::string> lastblock = nullptr;
    std::shared_ptr<std::string> read_block()
    {
        int sts;

        char buf[16];
        std::shared_ptr<std::string> pdata(new std::string);

        if (*sock.read(buf, 2) < 0)
            return nullptr;
        if (buf[0] != '#')
        {
            dprln("wrong package");
            debug_print_dump(buf, 2);
            return nullptr;
        }

        int ret = *sock.read(buf, hex2half(buf[1]));
        if (ret < 0)
            return nullptr;
        buf[ret] = 0;

        int packlen = atoi(buf);
        pdata->resize(packlen);

        int readed = 0;
        while (readed != packlen)
        {
            sts = *sock.read(const_cast<char *>(pdata->data()) + readed,
                             packlen - readed);
            if (sts < 0)
                return nullptr;
            readed += sts;
        }

        sts = *sock.read(buf, 2);
        if (sts < 0)
            return nullptr;

        packno += 1;
        if (DUMP_BLOCKS)
        {
            int dumpsize = 40 > packlen ? packlen : 40;
            nos::fprintln("RecvBlock: length: {0}\r\nFirst {1} bytes:",
                          packlen,
                          dumpsize);
            nos::print_dump(pdata->data(), dumpsize);
        }

        if (lastblock != nullptr)
        {
            for (int i = 0; i < pdata->size() / 8; ++i)
            {
                if (nos::buffer(lastblock->data() + i * 8, 8) ==
                    nos::buffer(pdata->data() + i * 8, 8))
                    nos::println("WARN: REPEATED VALUE DETECTED");
            }
        }

        lastblock = pdata;

        return pdata;
    }

    void read_raw_block(int sz)
    {

        char buf[1024];

        int ret = *sock.read(buf, sz);

        debug_print_dump(buf, sz);
    }
};

#endif
