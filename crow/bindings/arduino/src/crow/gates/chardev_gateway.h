/**
@file
*/

#ifndef CROW_GATES_CHANNEL_GATE_H
#define CROW_GATES_CHANNEL_GATE_H

#include <crow/gateway.h>
#include <crow/packet.h>
#include <igris/buffer.h>

namespace crow
{
    class chardev_protocol;
    class chardev_driver;
    class chardev_gateway;

    class chardev_protocol
    {
      public:
        chardev_gateway *gate;
        chardev_driver *driver; // fast access

      public:
        virtual void send_automate_reset(crow_packet *pack) = 0;
        virtual int send_automate_getdata(char *buf, int maxlen) = 0;

        virtual int receive_automate_newdata(char c) = 0;
        virtual void receive_automate_reset() = 0;
        virtual void receive_automate_setbuf(char *data, unsigned int size) = 0;
    };

    class chardev_driver
    {
      public:
        chardev_gateway *gateway;

        // virtual int room() = 0; // количество символов, которые устройство
        // может
        // принять. на передачу
        // virtual void send(const char *data, unsigned int size) = 0;

        // bool ready_to_send();
        // virtual bool ready_for_recv() = 0;
        virtual void start_send() = 0;

        virtual void read(char *data, int size) = 0;
        virtual void nonblock_tryread() = 0;
    };

    class chardev_gateway : public crow::gateway
    {
      public:
        chardev_driver *driver;
        chardev_protocol *protocol;

        dlist_head to_send = DLIST_HEAD_INIT(to_send);
        int packet_dataaddr_size = 48;
        crow_packet *insend;
        crow_packet *rpack;

      public:
        chardev_gateway(chardev_driver *driver, chardev_protocol *protocol);

        void dosend(struct crow_packet *pack);
        void init_recv();

        void send(crow_packet *) override;
        void nblock_onestep() override;
        void finish() override;

        void newline_handler();
        void packet_sended_handler();

      public: // driver callback
        void newdata(char c);
    };
}

#endif