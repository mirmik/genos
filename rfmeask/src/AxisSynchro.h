/**
    @file
    Расширенный вариант класса Axis, работающий с дополнительным устройством,
    реализующим режим сканирования.
*/

#ifndef AXISSYNCHRO_H
#define AXISSYNCHRO_H

#include <AbstractSyncro.h>
#include <Axis.h>
#include <Synchronizer.h>
#include <comm/Notify.h>
#include <igris/event/multiple_delegate.h>
#include <rxcpp/rx.hpp>
#include <servos/AbstractServo.h>

class AxisSynchro : public Axis
{
    uint32_t settedScanPoints = 0;
    int32_t settedScanMove = 0;
    uint32_t settedScanFZone = 0;
    uint32_t settedScanBZone = 0;

public:
    AbstractSyncro *synchro = nullptr;
    int gen_numer = 0;
    int gen_denum = 0;
    bool scan_mode = false;

    int64_t initial_encoder_position = 0;

public:
    void on_ready_status_changed(bool ready) override;
    AxisSynchro(const std::string &name,
                AbstractServo *_mover,
                AbstractSyncro *_synchro,
                bool spSignalEnable);
    AxisSynchro(const AxisSynchro &) = delete;
    AxisSynchro &operator=(const AxisSynchro &) = delete;
    void operationStatusSlot(uint8_t arg) override;
    void stopLastOperation(StopType stoptype) override;
    void operationCorrectEnd();
    void preset() override;
    bool haveScan() override;
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
    void generate_trigger() override;
    void manual_trigger_mode(int32_t en) override;
    void no_trigger_mode(int32_t en) override;
    void bitrigmode(int32_t en) override;
    int32_t trigreturn_time() override;
    void stopScanHandler();
    void scanPointHandler(int num);
    void wrongScanPointHandler();
    int32_t synchroSpeedTest() override;
    int32_t scan_discrete_output_set_state(int num, int state) override;
    int32_t scan_discrete_output_set_unical(int num) override;
    int32_t scan_discrete_output_disable() override;
    std::vector<double> prepare_correction_table_for_scan_device();

    std::string declared_type() const override
    {
        return "AxisSynchro";
    }

    std::vector<double>
    scan_points_position(int offset, int size, bool from_end) override
    {
        std::vector<double> result;
        auto intres = synchro->scan_points_position(offset, size, from_end);

        for (auto &i : intres)
        {
            int rev = m_reversed ? -1 : 1;
            double val = (double)i / unitRatio() * rev;
            result.push_back(val);
        }

        return result;
    }

public:
    void com_trigger_polarity(int val) 
    {
        synchro->com_trigger_polarity(val);
    }
    
    void com_trigger_duration(int val) 
    {
        synchro->com_trigger_duration(val);
    }

private:
    void operationCorrectStart();
    void operationCorrectFinish_SyncAxis();
    void operationCorrectFinish_SyncAxis_from_scanPointHandler();
};

#endif
