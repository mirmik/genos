/**
    @file
*/

#include <crow-support.h>

#include <chrono>
#include <logging.h>
#include <thread>
#include <vector>

#include <igris/util/numconvert.h>
#include <igris/util/string.h>

#include <nos/print.h>

#include <crow/address.h>
#include <crow/gates/udpgate.h>
#include <crow/nodes/publisher_node.h>
#include <crow/nodes/subscriber_node.h>
#include <crow/tower.h>

#include <igris/util/dstring.h>
#include <logging.h>   
#include <map>
#include <thread>

#define RESUBSCRIBE 1
extern bool DISABLE_REMOTER;

using namespace std::chrono_literals;

int remote_axis_jog(unsigned int ax, double spd);
int remote_axis_incpos(unsigned int ax, double spd);
int remoter_axes_total();
int remote_stop();
size_t axesTotal();
size_t activeAxesTotal();

crow::hostaddr crowaddr = crow::address(".12.127.0.0.1:10009");
//void remote_cmd_handler(crow::packet *pack);

crow::subscriber_node remote_cmd_subscriber;
crow::subscriber_node remote_pos_subscriber;
crow::subscriber_node remote_stop_subscriber;

crow::publisher_node  remote_cmd_publisher;

void remote_cmd_handler(nos::buffer data)
{
    nos::println("remote_cmd_handler:", igris::dstring(std::string(data.data(), data.size())));

    struct
    {
        float spd;
        int16_t ax;
    } __attribute__((packed)) s;

    memcpy(&s, data.data(), data.size());
    std::thread(remote_axis_jog, (int16_t)s.ax, (double)s.spd).detach();
}

void remote_pos_handler(nos::buffer data)
{
    nos::println("remote_pos_handler:", igris::dstring(std::string(data.data(), data.size())));
    struct
    {
        float pos;
        int16_t ax;
    } __attribute__((packed)) s;

    memcpy(&s, data.data(), data.size());
    std::thread(remote_axis_incpos, (int16_t)s.ax, (double)s.pos).detach();
}


void remote_stop_handler(nos::buffer data)
{
    nos::println("remote_stop_handler");
    std::thread(remote_stop).detach();
}

void lock_mode_notify()
{
    remote_cmd_publisher.publish("lock_on");
}

void axpublish_task()
{
    while (1)
    {
        char buf[3] = "A1";
        buf[1] = half2hex(remoter_axes_total());
        remote_cmd_publisher.publish(buf);
        std::this_thread::sleep_for(1000ms);
    }
}

int crow_configure(int crow_port)
{
    crow::create_udpgate(12, crow_port);

    if (!DISABLE_REMOTER)
    {
        remote_cmd_subscriber.set_handle(remote_cmd_handler); 
        remote_cmd_subscriber.init_subscribe(crowaddr, "remoter_spd_button", 2, 200, 
                                        2, 200);
        remote_cmd_subscriber.install_keepalive(2000);

        remote_pos_subscriber.set_handle(remote_pos_handler);
        remote_pos_subscriber.init_subscribe(crowaddr, "remoter_pos_button", 2, 200,
                                        2, 200);
        remote_pos_subscriber.install_keepalive(2000);

        remote_stop_subscriber.set_handle(remote_stop_handler);
        remote_stop_subscriber.init_subscribe(crowaddr, "remoter_stop", 2, 200, 2,
                                         200);
        remote_stop_subscriber.install_keepalive(2000);

        remote_cmd_publisher.init(crowaddr, "remoter_cmd", 2, 200);

        std::thread(axpublish_task).detach();
    }

    //syslogger.info("CROW_START_SPIN");
    crow::start_spin_with_select();
    return 0;
}
