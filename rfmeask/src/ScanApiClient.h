#ifndef ABSTRACT_SCAN_API_CLIENT_H
#define ABSTRACT_SCAN_API_CLIENT_H

#include <nos/shell/argv.h>
#include <stdint.h>

class ScanApiClient
{
public:
    virtual void init() = 0;
    virtual void preset() = 0;
    virtual void set_reverse(bool r) = 0;
    // virtual void evaluate_gain(const std::string &strategy) = 0;
    virtual void scanMove(int32_t arg) = 0;
    virtual void scanUnitMove(double arg) = 0;
    virtual void scanForwardZone(int32_t arg) = 0;
    virtual void scanUnitForwardZone(double arg) = 0;
    virtual void scanBackwardZone(int32_t arg) = 0;
    virtual void scanUnitBackwardZone(double arg) = 0;
    virtual void scanCompareStart() = 0;
    virtual void scanPoints(int32_t arg) = 0;
    virtual int32_t scanMove() = 0;
    virtual double scanUnitMove() = 0;
    virtual int32_t scanForwardZone() = 0;
    virtual double scanUnitForwardZone() = 0;
    virtual int32_t scanBackwardZone() = 0;
    virtual double scanUnitBackwardZone() = 0;
    virtual int32_t scanPoints() = 0;

    virtual void unsubscribe() = 0;
    virtual void generate_trigger() = 0;
    virtual void manual_trigger_mode(int32_t en) = 0;
    virtual void no_trigger_mode(int32_t en) = 0;
    virtual void bitrigmode(int32_t en) = 0;

    virtual int32_t trigreturn_time() = 0;
    virtual int32_t get_encoder_position() = 0;
    virtual int32_t discrete_output_set_state(int32_t channel,
                                              int32_t state) = 0;
    virtual int32_t discrete_output_set_unical(int32_t num) = 0;
    virtual int32_t discrete_output_disable() = 0;
    virtual void stop() = 0;

    virtual std::string typestr()
    {
        return "undefined scan client";
    }

    virtual ~ScanApiClient() = default;

    // @return true if command founded. false outerwise.
    virtual bool command(const nos::argv &args, nos::ostream &os) = 0;
    virtual std::string info() = 0;
    virtual std::string command_help() = 0;

    virtual int enable_external_trigger_notify() = 0;

    virtual std::vector<double>
    scan_points_position(int offset, int size, bool from_end) = 0;
};

#endif