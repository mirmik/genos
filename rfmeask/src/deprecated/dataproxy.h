#ifndef DATAPROXY_H
#define DATAPROXY_H

//#include <igris/container.h>
#include <AxisSynchro.h>
#include <deprecated/scpi_error.h>
#include <igris/result.h>
#include <nos/inet/tcpspam_server.h>
#include <tables.h>

using namespace igris::result_type;
// extern std::vector<AbstractAxis *> registered_axis_list;
// extern nos::inet::tcpspam_server dataSpam;

class DataProxy
{
public:
    bool binded = false;
    nos::inet::tcpspam_server dataSpam;

    // virtual void do_measure(int num) = 0;
    virtual nos::expected<void, std::string> do_bind() = 0;
    virtual void do_unbind() = 0;

    AxisSynchro *current_axis;

    int bind(int num)
    {
        if ((uint32_t)num >= axes_total())
            return -1;
        if (!get_axis(num)->haveScan())
            return -1;

        current_axis = (AxisSynchro *)get_axis(num);
        do_bind();

        return 0;
    }

    int unbind()
    {
        do_unbind();
        return 0;
    }
};

#endif // DATAPROXY_H
