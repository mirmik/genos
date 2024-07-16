/** @file */

#ifndef CROW_TOWER_H
#define CROW_TOWER_H

#include <crow/defs.h>
#include <crow/gateway.h>
#include <crow/packet_ptr.h>
#include <igris/event/delegate.h>
#include <nos/buffer.h>

#define CROW_SENDED 0
#define CROW_WRONG_ADDRESS -1

namespace crow
{
    extern uint16_t debug_data_size;

    extern dlist_head protocols;
    extern bool diagnostic_noack;
    extern unsigned int total_travelled;

    extern void (*default_incoming_handler)(crow::packet *pack);
    extern void (*undelivered_handler)(crow::packet *pack);
    extern igris::delegate<void> unsleep_handler;

    extern bool retransling_allowed;

    // Передать пакет в обработку.
    packet_ptr travel(crow::packet *pack);
    void nocontrol_travel(crow::packet *pack, bool fastsend);

    // Включить трассировку пакетов.
    void enable_diagnostic();
    bool diagnostic_enabled();

    // Включить диагностику прохождения, жизни пакетов
    void diagnostic_setup(bool en);

    // Отправить пакет.
    crow::packet_ptr send(const crow::hostaddr_view &addr,
                          nos::buffer data,
                          uint8_t type,
                          uint8_t qos,
                          uint16_t ackquant,
                          bool async = false);

    crow::packet_ptr send_v(const crow::hostaddr_view &addr,
                            const nos::buffer *vec,
                            size_t veclen,
                            uint8_t type,
                            uint8_t qos,
                            uint16_t ackquant,
                            bool async = false);

    crow::packet_ptr send_vv(const crow::hostaddr_view &addr,
                             const nos::buffer *vec,
                             size_t veclen,
                             const nos::buffer *vec2,
                             size_t veclen2,
                             uint8_t type,
                             uint8_t qos,
                             uint16_t ackquant,
                             bool async = false);

    crow::packet_ptr send_vvv(const crow::hostaddr_view &addr,
                              const nos::buffer *vec,
                              size_t veclen,
                              const nos::buffer *vec2,
                              size_t veclen2,
                              const nos::buffer *vec3,
                              size_t veclen3,
                              uint8_t type,
                              uint8_t qos,
                              uint16_t ackquant,
                              bool async = false);

    // Эта функция вызывается вратами после обработки отсылаемого пакета.
    void return_to_tower(crow::packet *pack, uint8_t sts);

    // Используется пользовательским кодом для освобождения пакета.
    void release(crow::packet *pack);
    void utilize(crow::packet *pack);

    void onestep();
    void onestep_travel_only();

    void spin();
    void spin_with_select();

    int stop_spin(bool wait = true);
    int start_spin_with_select();
    int start_spin_without_select();
    int start_spin();

    [[deprecated]] void spin_join();
    void join_spin();

    bool has_untravelled();
    bool has_untravelled_now();
    void print_list_counts();

    bool fully_empty();

    int64_t get_minimal_timeout();

    // Завершить гейты.
    void finish();

    int incomming_stage_count();
    int outers_stage_count();
    void unsleep();

    void set_spin_cancel_token();

    void spin_with_select_realtime(int abort_on_fault);
    int start_spin_with_select_realtime(int abort_on_fault);
    int start_spin_realtime(int abort_on_fault);

    void tower_release(crow::packet *pack);
}

#endif
