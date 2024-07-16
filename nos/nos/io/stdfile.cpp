#include <nos/io/stdfile.h>
#include <nos/util/init_priority.h>

namespace nos
{
    nos::buffered_file cout(stdout);
    nos::buffered_file cerr(stderr);
    nos::buffered_file cin(stdin);
}
