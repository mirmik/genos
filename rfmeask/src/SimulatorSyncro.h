/**
    @file
*/

#ifndef SIMULATOR_SYNCRO_H
#define SIMULATOR_SYNCRO_H

#include <AbstractAxis.h>
#include <AbstractSyncro.h>
#include <stdint.h>

extern igris::multiple_delegate<int> g_scanPointSignal;
extern igris::multiple_delegate<> descanSignal;
extern igris::multiple_delegate<> wrongScanPointSignal;

class SimulatorSyncro : public AbstractSyncro
{
    double scanzone = 0;
    double scanprefix = 0;
    double scanpostfix = 0;
    double step = 0;
    int scanpoints = 0;

    double nextpos = 0;
    int idx = 0;

    bool inscan = false;
    double start_position = 0;

public:
    SimulatorSyncro(const char *name) : AbstractSyncro(name) {}

    std::string declared_type() const override
    {
        return "simulator_syncro";
    }

    void preset() override {}
    void init() {}

    void point_notify()
    {
        g_scanPointSignal(idx + 1);
    }

    void descan()
    {
        descanSignal();
    }

    void setScanZone(int32_t arg) override
    {
        scanzone = arg;
    }
    void setScanPoints(int32_t arg) override
    {
        scanpoints = arg;
    }
    void setScanStartZone(int32_t arg) override
    {
        scanprefix = arg;
    }
    void setScanStopZone(int32_t arg) override
    {
        scanpostfix = arg;
    }

    void setLoadGain(double gain) override {}
    void setSpeed(int32_t imp) override {}
    void stop() override
    {
        descan();
    }

    void test() override
    {
        transactionReady();
        transactionConnectionSuccess();
    }

    void scanStart()
    {
        start_position = ax2local(binded_ax->last_unit_position());

        step = std::abs(scanzone / (scanpoints - 1));
        idx = 0;
        nextpos = std::abs(scanprefix + binded_ax->last_unit_position() -
                           start_position);
        inscan = true;
    }

    void cmpscanStart() override
    {
        scanStart();
    }

    void set_enc_numer(uint32_t arg) override {}
    void set_gen_numer(uint32_t arg) override {}
    void set_enc_denum(uint32_t arg) override {}
    void set_gen_denum(uint32_t arg) override {}

    void generate_trigger() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        point_notify();
        return;
    }
    void manual_trigger_mode(int32_t arg) override {}
    void wait_trigger_mode(int arg) override {}
    void bitrigmode(bool arg) override {}
    int32_t synchro_speed_test() override
    {
        return 0;
    }
    int32_t trigreturn_time() override
    {
        return 10;
    }

    void updaterThreadFunc() override;

    double ax2local(double in)
    {
        return in;
    }

    void eventctr_table_reset(int tblsize) override {}
    void eventctr_push_back(int tblsize) override {}
    void eventctr_table_reset_prefix(int size) override {}
    void eventctr_table_reset_cycle(int size) override {}
    void eventctr_table_reset_postfix(int size) override {}

    void eventctr_load_table(int no) override {}
    void eventctr_save_table(int no) override {}
    void eventctr_set_loop_total(int size) override {}
    
    void com_trigger_polarity(int val) override {} 
    void com_trigger_duration(int val) override {}

    int32_t discrete_output_set_state(int num, int state) override
    {
        return 0;
    }

    int32_t discrete_output_set_unical(int num) override
    {
        return 0;
    }

    int32_t discrete_output_disable() override
    {
        return 0;
    }

    int32_t get_encoder_position() override
    {
        BUG();
        return 0;
    }

    int32_t scanApiVersion()
    {
        return 100;
    }

    virtual void
    start_comparing_scan_with_distance_table(const std::vector<double> &points)
    {
        std::runtime_error(
            "Scanning with distance table is not supported for simulator");
    }

    virtual void start_comparing_scan_classic_mode()
    {
        cmpscanStart();
    }
};

#endif