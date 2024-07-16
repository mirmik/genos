/**
    @file
*/

#include <chrono>
#include <crow-support.h>
#include <crow/address.h>
#include <crow/gates/udpgate.h>
#include <crow/nodes/publisher_node.h>
#include <crow/nodes/subscriber_node.h>
#include <crow/tower.h>
#include <igris/util/numconvert.h>
#include <igris/util/string.h>
#include <logging.h>
#include <map>
#include <nos/print.h>
#include <thread>
#include <vector>

#define RESUBSCRIBE 1
using namespace std::chrono_literals;

size_t axes_total();
size_t active_axes_total();

crow::hostaddr crowaddr = crow::address(".12.127.0.0.1:10009");

int crow_configure(int crowport)
{
    crow::create_udpgate(12, crowport);
    crow::start_spin_with_select_realtime(0);
    return 0;
}
