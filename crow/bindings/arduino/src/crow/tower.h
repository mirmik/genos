/** @file */

#ifndef CROW_TOWER_H
#define CROW_TOWER_H

#include <crow/defs.h>
#include <crow/gateway.h>
#include <crow/packet_ptr.h>

#define CROW_SENDED 0
#define CROW_WRONG_ADDRESS -1

namespace crow
{
    extern uint16_t debug_data_size;

    extern dlist_head protocols;
    extern bool diagnostic_noack;
    extern unsigned int total_travelled;

    extern void (*user_incoming_handler)(crow_packet *pack);
    extern void (*user_type_handler)(crow_packet *pack);
    extern void (*undelivered_handler)(crow_packet *pack);

    extern void (*unsleep_handler)();

    extern bool retransling_allowed;

    // Передать пакет в обработку.
    packet_ptr travel(crow_packet *pack);
    void nocontrol_travel(crow_packet *pack, bool fastsend);

    // Включить трассировку пакетов.
    void enable_diagnostic();

    // Включить трассировку аллокации.
    void enable_live_diagnostic();

    // Включить диагностику прохождения, жизни пакетов
    void diagnostic_setup(bool en, bool len = false);

    // Отправить пакет.
    crow::packet_ptr send(const crow::hostaddr_view &addr,
                          nos::buffer data,
                          uint8_t type,
                          uint8_t qos,
                          uint16_t ackquant,
                          bool fastsend = CROW_FASTSEND_DEFAULT);

    crow::packet_ptr send_v(const crow::hostaddr_view &addr,
                            const nos::buffer *vec,
                            size_t veclen,
                            uint8_t type,
                            uint8_t qos,
                            uint16_t ackquant,
                            bool fastsend = CROW_FASTSEND_DEFAULT);

    crow::packet_ptr send_vv(const crow::hostaddr_view &addr,
                             const nos::buffer *vec,
                             size_t veclen,
                             const nos::buffer *vec2,
                             size_t veclen2,
                             uint8_t type,
                             uint8_t qos,
                             uint16_t ackquant,
                             bool fastsend = CROW_FASTSEND_DEFAULT);

    // Эта функция вызывается вратами после обработки отсылаемого пакета.
    void return_to_tower(crow_packet *pack, uint8_t sts);

    // Используется пользовательским кодом для освобождения пакета.
    void release(crow_packet *pack);

    // Взятие отметки времени.
    uint16_t millis();

    void onestep();
    void onestep_travel_only();

    void spin();
    void spin_with_select();
    void spin_with_select_realtime(int abort_on_fault);

    int stop_spin(bool wait = true);
    int start_spin_with_select();
    int start_spin_with_select_realtime(int abort_on_fault);
    int start_spin_without_select();

    int start_spin();
    int start_spin_realtime(int abort_on_fault);

    void start_resubscribe_thread(int millis);

    [[deprecated]] void spin_join();
    void join_spin();

    bool has_untravelled();
    bool has_untravelled_now();
    void print_list_counts();

    bool fully_empty();

    int64_t get_minimal_timeout();

    // Завершить гейты.
    void finish();
}

void crow_tower_release(crow_packet *pack);

#endif
