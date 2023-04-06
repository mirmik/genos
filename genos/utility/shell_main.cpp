#include <fcntl.h>
#include <genos/utility/shell.h>
#include <igris/shell/vtermxx.h>
#include <unistd.h>

void write_callback(const char *str, unsigned int sz)
{
    write(STDOUT_FILENO, str, sz);
}

int shell_main(int argc, char **argv)
{
    igris::vtermxx rl;
    rl.init(48, 5);
    rl.set_write_callback(write_callback);

//    int a = open("/dev/ttyS0", O_RDONLY);
//    int b = open("/dev/ttyS0", O_WRONLY);

//    char c;
//    while (1)
//    {
//        int sz = read(STDIN_FILENO, &c, 1);
//        rl.newdata(c);
        return 0;
//    }
}
