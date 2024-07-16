#ifndef CROW_SELF_DRIVEN_GSTUFF_H
#define CROW_SELF_DRIVEN_GSTUFF_H

#include <crow/gateway.h>
#include <crow/tower.h>
#include <igris/protocols/gstuff.h>

namespace crow
{
    class self_driven_gstuff : public crow::gateway
    {
        dlist_head to_send = DLIST_HEAD_INIT(to_send);

        int reserved_size = 48;

        crow_packet *insend = nullptr;
        char *send_buffer;
        char *send_it;
        char *send_eit;

        struct gstuff_autorecv recver;

        int (*write_callback)(void *, const char *data, unsigned int size);
        void *write_privdata;

      public:
        void set_write_callback(int (*write_callback)(void *, const char *data,
                                                      unsigned int size),
                                void *write_privdata)
        {
            this->write_callback = write_callback;
            this->write_privdata = write_privdata;
        }

        void set_send_buffer(char *sbuf) { send_buffer = sbuf; }

        void newdata(char c) { gstuff_autorecv_newchar(&recver, c); }

        void newline_handler() {}

        void start_send()
        {
            if (dlist_empty(&to_send))
                return;

            insend = dlist_first_entry(&to_send, crow_packet, lnk);
            dlist_del_init(&insend->lnk);

            int size = gstuffing((const char *)&insend->header,
                                 insend->fullsize(), send_buffer);

            send_it = send_buffer;
            send_eit = send_buffer + size;

            continue_send();
        }

        void finish_send()
        {
            system_lock();
            crow::return_to_tower(insend, CROW_SENDED);
            insend = nullptr;
            system_unlock();

            start_send();
        }

        void continue_send()
        {
            int needwrite = send_eit - send_it;
            int writed = write_callback(write_privdata, send_it, needwrite);

            send_it += writed;

            if (send_it == send_eit)
            {
                finish_send();
            }
        }

        void send(crow_packet *pack) override
        {
            system_lock();
            dlist_move(&pack->lnk, &to_send);
            system_unlock();

            if (insend == nullptr)
            {
                start_send();
            }
        }

        void nblock_onestep() override {}
        void finish() override {}
    };
}

#endif