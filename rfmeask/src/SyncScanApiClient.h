/**
    @file
*/

#ifndef SCAN_API_CLIENT_H
#define SCAN_API_CLIENT_H

#include <ScanApiClient.h>
#include <set>
#include <string>

class AbstractAxis;
class AbstractSyncro;
class AbstractServo;

class SyncScanApiClient : public ScanApiClient
{
    AbstractAxis *parent = nullptr;

public:
    bool reverse = false;
    AbstractSyncro *synchro = nullptr;
    AbstractServo *syncsrv = nullptr; // если есть связанный сервоусилитель

    uint32_t settedScanPoints = 0;
    int32_t settedScanMove = 0;
    uint32_t settedScanFZone = 0;
    uint32_t settedScanBZone = 0;

    bool scanStop = false;
    int lastPoint = 0;

    std::string gain_strategy;
    double _gain = 0;

public:
    double gain() const;

    SyncScanApiClient(AbstractAxis *ax, AbstractSyncro *sync) :
        parent(ax), synchro(sync)
    {
    }
    SyncScanApiClient(const SyncScanApiClient &) = delete;
    SyncScanApiClient &operator=(const SyncScanApiClient &) = delete;

    std::set<Device *> get_devices();
    void init() override;
    void preset() override;

    void evaluate_gain(const std::string &strategy);

    void set_reverse(bool r) override;
    void scanMove(int32_t arg) override;
    void scanUnitMove(double arg) override;
    void scanForwardZone(int32_t arg) override;
    void scanUnitForwardZone(double arg) override;
    void scanBackwardZone(int32_t arg) override;
    void scanUnitBackwardZone(double arg) override;
    void scanCompareStart() override;
    void scanPoints(int32_t arg) override;
    int32_t scanMove() override;
    double scanUnitMove() override;
    int32_t scanForwardZone() override;
    double scanUnitForwardZone() override;
    int32_t scanBackwardZone() override;
    double scanUnitBackwardZone() override;
    int32_t scanPoints() override;
    int32_t trigreturn_time() override;
    int32_t get_encoder_position() override;
    void stopScanHandler();
    void scanPointHandler(int num);
    void wrongScanPointHandler();
    void unsubscribe() override;

    void generate_trigger() override;
    void manual_trigger_mode(int32_t en) override;
    void no_trigger_mode(int32_t en) override;
    void bitrigmode(int32_t en) override;

    void stop() override;

    int32_t discrete_output_set_state(int num, int state) override;
    int32_t discrete_output_set_unical(int num) override;
    int32_t discrete_output_disable() override;
    std::string typestr() override
    {
        return "Radioline synchronizer v2";
    }

    bool command(const nos::argv &args, nos::ostream &os) override;
    std::string info() override;
    std::string command_help() override;

    int enable_external_trigger_notify() override;

    double magic_multiplier();

    std::vector<double>
    scan_points_position(int offset, int size, bool from_end) override;
};

#endif
