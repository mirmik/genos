#include "igris/util/bits.h"
#include "stdio.h"
#include <cankollm/cankollm.h>
#include <cankollm/canopen.h>
#include <errno.h>
#include <fcntl.h>
#include <logging.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <utilxx/ReadLine.h>

void Can::open(const char *str)
{
    struct termios options;

    portname = str;
    file.open(str, O_RDWR);
    int port = file.fd();

    tcgetattr(port, &options);

    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);

    bits_assign(options.c_cflag, CSIZE, CS8);
    bits_clr(options.c_cflag, CSTOPB);
    bits_clr(options.c_cflag, PARENB);
    // bits_clr(options.c_cflag, CNEW_RTSCTS);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // options.c_lflag |= ICANON;
    // options.c_iflag &= ~(IXON | IXOFF | IXANY | ECHO | ECHOE | ISIG);

    // options.c_oflag &= ~OPOST;
    // options.c_cc[VMIN] = 1;
    // options.c_cc[VTIME] = 0;

    tcsetattr(port, TCSANOW, &options);
    logger.info("CAN port open: {} fd: {}", portname, file.fd());
}

void Can::reconnect()
{
    open(portname);
}

void Can::close()
{
    int sts = file.close();
    if (sts < 0)
    {
        perror("Can::close");
    }
}

void Can::send(const char *str, size_t sz)
{
    auto sts = file.write(str, sz);
    if (sts.is_error())
    {
        perror("Can::send");
    }
}

void Can::send_test()
{
    char msg[64] = "S\r";
    file.write(msg, strlen(msg));
}

void Can::reset()
{
    char msg[64] = "RA\r";
    file.write(msg, strlen(msg));
}

void Can::parse(char *str)
{
    if (driver == nullptr)
        throw "Can::parse: driver is nullptr";

    int status = driver->Parse(str);
    if (status != 0)
    {
        printf("parse status: %d\r\n", status);
    };
}

void Can::recv_func()
{
    char c;
    igris::ReadLine<128> rl;

    logger.info("Can recv thread started");

    while (1)
    {
        auto code = file.read(&c, 1);
        if (code.is_error())
        {
            perror("Can::recv_function:");
            nos::log::trace("Connection refused\r\n");
            reconnect();
            break;
        };
        if (code.value() == 0)
            continue;
        if (c == '\n' || c == '\r')
        {
            parse((char *)rl.c_str());
            logger.trace("Can recv: {}", rl.c_str());
            rl.init();
        }
        else
            rl.putc(c);
    }
}
