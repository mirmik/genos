#include <crow/warn.h>
#include <igris/dprint.h>

#if !defined(MEMORY_ECONOMY)
void crow::warn(nos::buffer msg)
{
    dpr("WARN: ");
    debug_write(msg.data(), msg.size());
    dln();
}
#endif