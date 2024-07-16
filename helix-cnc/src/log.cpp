#include <crow/addons/logger.h>
#include <crow/nodes/publisher_node.h>
#include <igris/buffer.h>
#include <ralgo/log.h>
#include <zillot/common/uartring.h>

extern crow::publisher_node helix_log_publisher;
extern crow::publish_logger helix_logger;

namespace ralgo
{
    void log(ralgo::LogLevel lvl, const char *a, const char *b, const char *c)
    {
        // return;
        // std::string msg = a;
        // if (b)
        //     msg += b;
        // if (c)
        //     msg += c;

        // helix_logger.log(ralgo::level_to_noslvl(lvl), msg);
    }
}
