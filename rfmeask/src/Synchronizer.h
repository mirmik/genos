/**
    @file
*/

#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

/*
    Класс для работы с платой синхронизатором.
    Является наследником класса Device.

    Реализует функции контроллера непрерывного сканирования.
*/

#include <AbstractSyncro.h>
#include <RS232Listener.h>
#include <devices/Device.h>
#include <igris/container/ring.h>
#include <include_helper.h>

class Synchronizer : public AbstractSyncro
{
public:
    uint8_t rs232_addr = 0;
    RS232Listener *drv = nullptr;
    AbstractServo *mover = nullptr;
    bool connected = false;
    int api_version;
    static bool scanmode;
    static std::mutex mtx;

    int32_t encoder_numerator = 0;
    int32_t encoder_denumerator = 0;

    igris::ring<int64_t> point_information_ring = igris::ring<int64_t>(100000);

public:
    Synchronizer(const char *name, RS232Listener *com, uint8_t addr);
    Synchronizer(const Synchronizer &) = delete;
    Synchronizer &operator=(const Synchronizer &) = delete;

    std::string declared_type() const override
    {
        return "sync";
    }

    double encoder_multiplier();

    void push_point_information(int64_t point);

    void updaterThreadFunc() override;
    void preset() override;
    int getAlarmStatus() override;
    void setScanZone(int32_t arg) override;
    void setScanPoints(int32_t arg) override;
    void setScanStartZone(int32_t arg) override;
    void setScanStopZone(int32_t arg) override;
    void setLoadGain(double gain) override;
    void setSpeed(int32_t imp) override;
    void stop() override;
    void test() override;
    void set_enc_numer(uint32_t arg) override;
    void set_gen_numer(uint32_t arg) override;
    void set_enc_denum(uint32_t arg) override;
    void set_gen_denum(uint32_t arg) override;
    void generate_trigger() override;
    void manual_trigger_mode(int32_t argwa) override;
    void wait_trigger_mode(int arg) override;
    void bitrigmode(bool arg) override;
    int32_t synchro_speed_test() override;
    int32_t trigreturn_time() override;
    int32_t get_encoder_position() override;
    int32_t discrete_output_set_state(int num, int state) override;
    int32_t discrete_output_set_unical(int num) override;
    int32_t discrete_output_disable() override;
    int32_t scanApiVersion();
    int32_t enable_external_trigger_mode() override;

    void start_comparing_scan_with_distance_table(
        const std::vector<double> &points) override;
    void start_comparing_scan_classic_mode() override;
    void cmpscanStart() override;

    void invokeDecorator(std::function<void()> func)
    {
        try
        {
            func();
            transactionConnectionSuccess();
            transactionReady();
        }
        catch (const DeviceRefuseException &excpt)
        {
            transactionRefuseError();
            throw DeviceRefuseException("DeviceRefuse: invoke decorator");
        }
        catch (const DeviceAlarmException &excpt)
        {
            transactionConnectionSuccess();
            transactionAlarmError("undefined");
            throw DeviceAlarmException();
        }
    }

    void eventctr_save_table(int no) override
    {
        drv->eventctr_save_table(rs232_addr, no);
    }

    void eventctr_load_table(int no) override
    {
        drv->eventctr_load_table(rs232_addr, no);
    }

    void eventctr_set_loop_total(int total) override
    {
        drv->eventctr_set_loop_total(rs232_addr, total);
    }

    void eventctr_table_reset(int tblsize) override
    {
        drv->eventctr_table_reset(rs232_addr, tblsize);
    }
    void eventctr_push_back(int tblsize) override
    {
        drv->eventctr_push_back(rs232_addr, tblsize);
    }
    void eventctr_table_reset_prefix(int size) override
    {
        drv->eventctr_table_reset_prefix(rs232_addr, size);
    }
    void eventctr_table_reset_cycle(int size) override
    {
        drv->eventctr_table_reset_cycle(rs232_addr, size);
    }
    void eventctr_table_reset_postfix(int size) override
    {
        drv->eventctr_table_reset_postfix(rs232_addr, size);
    }

    std::vector<int64_t>
    scan_points_position(int offset, int size, bool from_end) override;

    void command(const nos::argv &args, nos::ostream &os) override;
    std::string info() override;
    std::string command_help() override;

    void com_trigger_polarity(int val) override
    {
        drv->com_trigger_polarity(val);
    }

    void com_trigger_duration(int val) override
    {
        drv->com_trigger_duration(val);
    }
};

#endif
