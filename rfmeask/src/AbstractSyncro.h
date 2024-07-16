/**
    @file
*/

#ifndef ABSTRACT_SYNCRO_H
#define ABSTRACT_SYNCRO_H

#include <devices/Device.h>

class AbstractAxis;

class AbstractSyncro : public Device
{
public:
    AbstractAxis *binded_ax = nullptr;
    int64_t initial_encoder_position;

    AbstractSyncro(const char *name) : Device(name) {}
    AbstractSyncro(const AbstractSyncro &) = delete;
    AbstractSyncro &operator=(const AbstractSyncro &) = delete;

    virtual void start_comparing_scan_with_distance_table(
        const std::vector<double> &points) = 0;
    virtual void start_comparing_scan_classic_mode() = 0;
    virtual void setScanZone(int32_t arg) = 0;
    virtual void setScanPoints(int32_t arg) = 0;
    virtual void setScanStartZone(int32_t arg) = 0;
    virtual void setScanStopZone(int32_t arg) = 0;
    virtual void setLoadGain(double gain) = 0;
    virtual void setSpeed(int32_t imp) = 0;
    virtual void stop() = 0;
    virtual void test() = 0;
    virtual void cmpscanStart() = 0;
    virtual void set_enc_numer(uint32_t arg) = 0;
    virtual void set_gen_numer(uint32_t arg) = 0;
    virtual void set_enc_denum(uint32_t arg) = 0;
    virtual void set_gen_denum(uint32_t arg) = 0;
    virtual int32_t get_encoder_position() = 0;
    // virtual void generate_trigger() = 0;
    // virtual void manual_trigger_mode(bool arg) = 0;
    virtual void wait_trigger_mode(int arg) = 0;
    virtual void bitrigmode(bool arg) = 0;
    virtual int32_t synchro_speed_test() = 0;
    virtual int32_t trigreturn_time() = 0;
    virtual void eventctr_load_table(int tblsize) = 0;
    virtual void eventctr_save_table(int tblsize) = 0;
    virtual void eventctr_set_loop_total(int tblsize) = 0;
    virtual void eventctr_table_reset(int tblsize) = 0;
    virtual void eventctr_push_back(int tblsize) = 0;
    virtual void eventctr_table_reset_prefix(int size) = 0;
    virtual void eventctr_table_reset_cycle(int size) = 0;
    virtual void eventctr_table_reset_postfix(int size) = 0;
    virtual int32_t discrete_output_set_state(int num, int state) = 0;
    virtual int32_t discrete_output_set_unical(int num) = 0;
    virtual int32_t discrete_output_disable() = 0;

    virtual void com_trigger_polarity(int val) = 0; 
    virtual void com_trigger_duration(int val) = 0; 
    
    virtual int32_t enable_external_trigger_mode()
    {
        return 0;
    }

    virtual std::vector<int64_t>
    scan_points_position(int offset, int size, bool from_end)
    {
        return std::vector<int64_t>();
    }
};

#endif