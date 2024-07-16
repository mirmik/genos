#include <NetworkAnalyzer.h>
#include <igris/util/bug.h>
#include <main.h>

void listenerThread_SignalAnalyzerBasic_Func(void *)
{
    char buf[128];
    int sts;

    nos::println("SignalAnalyzerBasic active");

    if (traces.size() != 1)
        nos::println("Warn SignalAnalyzerBasic mode, but: traces.size() != 1");

    if (netan.send("FORM REAL,32") < 0)
        goto __error__;
    if (netan.send("FORM:BORD SWAP") < 0)
        goto __error__;

    while (true)
    {
        std::shared_ptr<std::string> ampdata;

        if (discStart)
            return;

        auto exret = nos::read_until_from(informer, {buf, 128}, "\n");
        if (exret.is_error())
            goto __error__;

        int ret = *exret;

        if (ret > 0)
            nos::println(nos::buffer(buf, ret));

        if (ret <= 0)
            goto __error__;

        if (discStart)
            return;

        netanlock.lock();
        if (netan.send("TRAC:DATA? TRACE1") < 0)
            goto __error__;

        ampdata = netan.read_block();

        if (!ampdata)
        {
            goto __error__;
        }

        netanlock.unlock();

        datmutx.lock();
        datq.push(std::make_pair(ampdata, ampdata));
        datmutx.unlock();
    }

__error__:
    client.write("err", 3);
    dprln("listenerThreadFunc::dataerror");
    disconnect();
    dprln("listenerThreadFunc::after_disc");
    return;
}

void listenerThread_SignalAnalyzerMarker_Func(void *)
{
    BUG();
}