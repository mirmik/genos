#include <functional>
#include <nos/cancel.h>
#include <nos/io/istream.h>
#include <nos/io/ostream.h>
#include <nos/shell/executor.h>

namespace nos
{
    void
    shell_tcp_thread_function(int port,
                              std::reference_wrapper<nos::executor> executor,
                              nos::cancel_token cancel);

    void
    shell_io_thread_function(std::reference_wrapper<nos::istream> is,
                             std::reference_wrapper<nos::ostream> os,
                             std::reference_wrapper<nos::executor> executor,
                             nos::cancel_token cancel);
}
