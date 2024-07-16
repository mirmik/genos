/** @file */

#ifndef CROW_PACKET_PTR_H
#define CROW_PACKET_PTR_H

#include <crow/packet.h>
#include <igris/sync/syslock.h>

namespace crow
{
    class packet_ptr
    {
      protected:
        crow_packet *pack;

      public:
        packet_ptr(crow_packet *pack_) : pack(pack_)
        {
            if (pack == nullptr)
                return;
            assert(pack->refs >= 0);

            system_lock();
            pack->refs++;
            system_unlock();
        }

        packet_ptr(const crow::packet_ptr &oth) : pack(oth.pack)
        {
            assert(pack->refs >= 0);
            system_lock();
            pack->refs++;
            system_unlock();
        }

        packet_ptr(crow::packet_ptr &&oth) : pack(oth.pack)
        {
            assert(pack->refs >= 0);
            oth.pack = nullptr;
        }

        crow_packet *get() { return pack; }

        crow_packet *get() const { return pack; }

        crow_packet *operator->() { return pack; }

        crow_packet &operator*() { return *pack; }

        crow::packet_ptr &operator=(const crow::packet_ptr &oth)
        {
            clear();

            pack = oth.pack;
            system_lock();
            pack->refs++;
            system_unlock();

            return *this;
        }

        crow::packet_ptr &operator=(crow::packet_ptr &&oth)
        {
            clear();

            pack = oth.pack;
            oth.pack = nullptr;

            return *this;
        }

        ~packet_ptr();

        operator bool() { return pack != nullptr; }

        bool operator==(decltype(nullptr)) { return pack == nullptr; }

        void clear();
    };
}

#endif
