#ifndef CROW_SELF_DRIVEN_GSTUFF_H
#define CROW_SELF_DRIVEN_GSTUFF_H

#include <crow/gateway.h>
#include <crow/tower.h>
#include <igris/iovec.h>
#include <igris/protocols/gstuff.h>
#include <igris/sync/semaphore.h>
#include <igris/util/crc.h>

#include <nos/print.h>

namespace crow
{
    template <typename Header> class self_driven_gstuff : public crow::gateway
    {
        gstuff_context gctx;

        dlist_head to_send = DLIST_HEAD_INIT(to_send);
        crow::packet *insend = nullptr;
        /// Максимальная длина пакета, какой мы готовы принять.
        int received_maxpack_size = 0;
        crow::packet *recvpack = nullptr;
        gstuff_autorecv recver;

        int (*write_callback)(void *,
                              const char *data,
                              unsigned int size) = nullptr;
        int (*room_callback)(void *) = nullptr;
        void *priv = nullptr;

        uint8_t send_state = 0;
        uint8_t data_state = 0;
        uint16_t data_index = 0;
        uint8_t crc = 0;

    public:
        self_driven_gstuff() : gctx(), recver(gctx) {}
        self_driven_gstuff(const self_driven_gstuff &) = delete;
        self_driven_gstuff &operator=(const self_driven_gstuff &) = delete;

        bool in_send()
        {
            return insend != nullptr;
        }

        void
        init(int (*write_callback)(void *, const char *data, unsigned int size),
             int (*room_callback)(void *),
             void *privdata,
             int received_maxpack_size)
        {
            this->received_maxpack_size = received_maxpack_size;
            this->write_callback = write_callback;
            this->room_callback = room_callback;
            this->priv = privdata;

            init_receiver();
            invalidate_sender();
        }

        void newdata(char c)
        {
            int status;
            status = recver.newchar(c);
            switch (status)
            {
            case GSTUFF_NEWPACKAGE:
                newline_handler();
                break;

            default:
                break;
            }
        }

        void newline_handler()
        {
            crow::packet *pack = recvpack;
            recvpack = nullptr;
            pack->revert_gate(this->id);
            crow::packet_initialization(pack, this);
            crow::nocontrol_travel(pack, false);
            init_receiver();
        }

        void init_receiver()
        {
            assert(recvpack == nullptr);
            recvpack =
                crow::allocate_packet<crow::header_v1>(received_maxpack_size);
            recver.setbuf((uint8_t *)recvpack->header_addr(),
                          received_maxpack_size);
        }

        size_t room()
        {
            return room_callback(priv);
        }

        size_t write(char *data, size_t size)
        {
            return write_callback(priv, data, size);
        }

        size_t sendbyte(char c)
        {
            return write(&c, 1);
        }

        void start_send(crow::packet *pack)
        {
            insend = pack;
            crc = 0xFF;
            send_state = 0;
            data_state = 0;
            data_index = 0;
            continue_send();
        }

        bool data_section(char *data, size_t size)
        {
            while (room() >= 2 && data_index < size)
            {
                char arr[2];
                char c = data[data_index++];
                igris_strmcrc8(&crc, c);
                int len = gstuff_byte(c, arr, gctx);
                write(arr, len);
            }
            if (data_index == size)
            {
                send_state++;
                data_index = 0;
                return true;
            }
            return false;
        }

        void continue_send()
        {
            bool fallthrow;

            do
            {
                fallthrow = false; /// < отвечает за переход в следующую секцию
                                   /// без возврата из функции путём возвращения
                                   /// в цикле do-while

                switch (send_state)
                {
                case 0: /// Отправка старта
                {
                    if (room() >= 1)
                    {
                        sendbyte(gctx.GSTUFF_START);
                        send_state = 1;
                        fallthrow = true;
                    }
                }
                break;
                case 1: /// Отправка заголовка
                {
                    Header header = crow::extract_header<Header>(insend);
                    fallthrow = data_section((char *)&header, sizeof(Header));
                }
                break;
                case 2: /// Отправка адреса
                {
                    fallthrow = data_section((char *)insend->addrptr(),
                                             insend->addrsize());
                }
                break;
                case 3: /// Отправка данных
                {
                    fallthrow = data_section((char *)insend->dataptr(),
                                             insend->datasize());
                }
                break;
                case 4: /// Отправка контрольной суммы
                {
                    if (room() >= 2)
                    {
                        char arr[2];
                        int len = gstuff_byte(crc, arr, gctx);
                        write(arr, len);
                        send_state = 5;
                        fallthrow = true;
                    }
                }
                break;
                case 5: /// Отправка стоп сигнала
                {
                    if (room() >= 1)
                    {
                        sendbyte(gctx.GSTUFF_STOP);
                        fallthrow = false;
                        finish_send();
                    }
                }
                break;
                }
            } while (fallthrow);
        }

        void finish_send()
        {
            system_lock();
            crow::return_to_tower(insend, CROW_SENDED);
            invalidate_sender();
            system_unlock();

            /// Отправка завершена. Попытаться отправить следующий в очереди
            /// пакет.
            if (!dlist_empty(&to_send))
            {
                auto pack = dlist_first_entry(&to_send, crow::packet, ulnk);
                dlist_del_init(&insend->ulnk);
                start_send(pack);
            }
        }

        void send(crow::packet *pack) override
        {
            /// Начать отправку пакета, если в очереди никого нет.
            /// Или добавить в очередь.
            if (insend == nullptr)
                start_send(pack);
            else
            {
                system_lock();
                dlist_move(&pack->ulnk, &to_send);
                system_unlock();
            }
        }

        void invalidate_sender()
        {
            insend = nullptr;
        }

        void nblock_onestep() override
        {
            if (insend)
            {
                /// Продолжаем незаверщенную отправку, если это необходимо.
                continue_send();
            }
        }

        void finish() override
        {
            if (recvpack)
            {
                recvpack->invalidate();
                recvpack = nullptr;
            }
        }

        ~self_driven_gstuff()
        {
            finish();
        }
    };
}

#endif