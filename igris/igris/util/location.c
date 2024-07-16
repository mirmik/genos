#include <igris/dprint.h>
#include <igris/util/location.h>

void debug_print_location(struct location location)
{
    debug_print("file: ");
    debug_print(location.file);
    dln();
    debug_print("func: ");
    debug_print(location.func);
    dln();
    debug_print("line: ");
    debug_printdec_unsigned_int(location.line);
    dln();
}
