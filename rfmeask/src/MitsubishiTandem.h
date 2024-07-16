/**
    @file
*/

#ifndef MITSUBISHITANDEM_H
#define MITSUBISHITANDEM_H

#include <MitsubishiBoardServo.h>
#include <algorithm>
#include <limits.h>
#include <servos/AbstractServo.h>
#include <tandemcontroller.h>

using namespace std::chrono_literals;
extern std::string runtime_path;

class MitsubishiTandem : public MBoardServoBasic, public TandemController
{
private:
    // types
    using parametr_head = std::pair<int, int>;

    int group;

    // parametrs
    std::vector<int> adrs;
    std::vector<int> nreverse;
    std::vector<int> slaves;

    // runtime
    std::vector<int> m_torque;

    // SavedData
    bool syncro_mode_flag = false;

    // consts
    std::map<parametr_head, int> parametrs_dictionary;

public:
    MitsubishiTandem(const char *name,
                     MitsubishiBoard *board,
                     int channel,
                     const std::vector<int> &adrs,
                     const std::vector<int> &nreverse,
                     int group) :
        MBoardServoBasic(name, board, channel, adrs[0]),
        group(group),
        adrs(adrs),
        nreverse(nreverse),
        slaves(++adrs.begin(), adrs.end()),
        m_torque(adrs.size()),
        parametrs_dictionary{
            // absolute position
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x1102,
             },
             0x0001},

            // emergency stop
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x1103,
             },
             0x2100},

            //Туту сложно. Смотри мануал.
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x0200,
             },
             0x0101},

            // Speed limitation
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x0222,
             },
             0xFFFF},
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x0223,
             },
             0x0001},

            // LSN, LSP, SON
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x0219,
             },
             0x0300},

            // data set home position method
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x0240,
             },
             0x0002},

            // encoder output settings
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x1182,
             },
             0x0010}, // PC03
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x110E,
             },
             50}, // PA15
            {MitsubishiBoard::parametr_head{
                 0xFF,
                 0x110F,
             },
             1}, // PA16
        }
    {
        // topts.save();
        master = adrs[0];
    }

    void init()
    {
        parametrs_dictionary.insert(std::make_pair(
            MitsubishiBoard::parametr_head{
                0xFF,
                0x020A,
            },
            electronicGear_numer & 0xFFFF)); // CMXL
        parametrs_dictionary.insert(std::make_pair(
            MitsubishiBoard::parametr_head{
                0xFF,
                0x020B,
            },
            electronicGear_numer >> 16)); // CMXH
        parametrs_dictionary.insert(std::make_pair(
            MitsubishiBoard::parametr_head{
                0xFF,
                0x020C,
            },
            electronicGear_denum & 0xFFFF)); // CDVL
        parametrs_dictionary.insert(std::make_pair(
            MitsubishiBoard::parametr_head{
                0xFF,
                0x020D,
            },
            electronicGear_denum >> 16)); // CDVH
        board->registerTandem(this);
    }

    int balanceMode(bool en)
    {
        if (en)
        {
            syncroModeEnable(true);
        }

        else
        {
            syncroModeEnable(false);
        }

        return 0;
    }

    void setParams()
    {
        int idx = 0;
        for (auto a : adrs)
        {
            board->setParamsAddress(a, parametrs_dictionary);
            if (nreverse[idx] == 1)
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x110D,
                    },
                    1);
            board->setParam(
                MitsubishiBoard::parametr_head{
                    a,
                    0x0268,
                },
                0);
            board->setParam(
                MitsubishiBoard::parametr_head{
                    a,
                    0x0264,
                },
                group);
            board->setParam(
                MitsubishiBoard::parametr_head{
                    a,
                    0x0265,
                },
                0x0010);

            board->setParam(
                MitsubishiBoard::parametr_head{
                    a,
                    0x1107,
                },
                adjustment.type);
            switch (adjustment.type)
            {
            case 1:
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x1108,
                    },
                    adjustment.table_number);
                break;
            case 2:
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x1108,
                    },
                    adjustment.table_number);
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x1145,
                    },
                    adjustment.inertia_rate * 1000);
                break;
            default:
                BUG()
                break;
            }

            board->setParam(
                MitsubishiBoard::parametr_head{
                    a,
                    0x0210,
                },
                torqLimitBack * 10 + 0.5);
            board->setParam(
                MitsubishiBoard::parametr_head{
                    a,
                    0x0211,
                },
                torqLimitForw * 10 + 0.5);

            if (fully_closed_mode)
            {
                nos::println("FULLY_CLOSED",
                             name(),
                             fully_closed_numer,
                             fully_closed_denum);
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x1100,
                    },
                    0x1010);
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x1203,
                    },
                    fully_closed_numer);
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x1204,
                    },
                    fully_closed_denum);
                board->setParam(
                    MitsubishiBoard::parametr_head{
                        a,
                        0x119A,
                    },
                    fully_closed_reverse[idx] ? 0x0001 : 0x0000);
            }

            idx++;
        }

        restore_home_position();

        if (igroup != 0)
        {
            board->setParam(
                MitsubishiBoard::parametr_head{
                    master,
                    0x0260,
                },
                igroup);
        }
    }

    void preset() override
    {
        syncroModeEnable(true);

        board->servoEnable(channel, adrs[0]);
        for (auto s : slaves)
        {
            board->servoEnable(channel, s);
        }

        board->waitEnabled(channel, adrs[0]);
        for (auto s : slaves)
        {
            board->waitEnabled(channel, s);
        }
    }

    void syncroModeEnable(bool en)
    {
        syncro_mode_flag = en;
        if (!en)
        {
            board->commandBitSignal(1, 0, SSC_CMDBIT_SYS_ASYN1, true);
            board->waitBitSignal(1, 0, SSC_CMDBIT_SYS_ASYN1, 1);
        }
        else
        {
            board->commandBitSignal(1, 0, SSC_CMDBIT_SYS_ASYN1, false);
            board->waitBitSignal(1, 0, SSC_CMDBIT_SYS_ASYN1, 0);
        }
    }

    int micro_move(int slv, int32_t dist)
    {
        if (syncro_mode_flag)
        {
            logger.warn("try to micro move in sync mode!!!!");
            nos::println("WARN: try to micro move in sync mode!!!!");
            return igris::rpc::status::InternalError;
        }
        else
        {
            board->incrementalMove(
                channel, slaves[slv], dist, m_speed, m_accel_ms, m_accel_ms);
        }
        return 0;
    }

    void presetTandem() override
    {
        syncroModeEnable(true);
    }

    int32_t request_position() override
    {
        return (int32_t)board->getCurrentPosition(channel, master);
    }

    int32_t requestSlaveCurrent(int num)
    {
        return (int32_t)board->getElectricalCurrentFeedback(channel,
                                                            slaves[num]);
    }

    uint8_t request_limit_switch() override
    {
        return 0;
    }

    /*uint8_t request_operation_status() override
    {
        auto status = board->getInpositionStatus(channel, master);
        return status ? STOPED : MOVED;
    }*/

    void relative_move(int32_t dist) override
    {
        if (syncro_mode_flag == false)
        {
            return;
        }
        board->incrementalMove(
            channel,
            master,
            igris::math::limitation(dist, INT_MIN + 1, INT_MAX),
            m_speed,
            m_accel_ms,
            m_accel_ms);
    }

    void absolute_move(int32_t pos) override
    {
        if (syncro_mode_flag == false)
            return;
        board->incrementalMove(channel,
                               master,
                               pos - currentPosition(),
                               m_speed,
                               m_accel_ms,
                               m_accel_ms);
    }

    void updaterThreadFunc()
    {
        logger.info("updaterThreadFunc started");
        while (1)
        {
            //            int32_t l_currentPosition = m_currentPosition;
            uint8_t l_limitSwitchStatus = m_limitSwitchStatus;
            uint8_t l_operationStatus = m_operationStatus;

            if (board->started == true)
            {
                int code = board->get_system_code(channel);
                if (code != 10)
                {
                    nos::fprintln("system code : {}", code);
                    transactionRefuseError();
                    board->started = false;
                    goto end;
                }
                transactionConnectionSuccess();
            }

            if (board->started == false)
            {
                transactionRefuseError();
                board->tryToStart();
                goto end;
            }

            try
            {
                m_currentPosition = request_position();
                m_limitSwitchStatus = request_limit_switch();
                m_operationStatus = request_operation_status();

                // board->transient_command(channel, master);

                m_torque[0] = requestCurrent();
                for (unsigned int i = 0; i < slaves.size(); ++i)
                {
                    m_torque[i + 1] = requestSlaveCurrent(i);
                }

                multiTorqueUpdated(m_torque);
            }
            catch (DeviceRefuseException)
            {
                goto end;
            };

            positionUpdated(m_currentPosition);

            if (m_limitSwitchStatus != l_limitSwitchStatus)
            {
                limitSwitchStatusSignal(m_limitSwitchStatus);
            };
            if ((m_operationStatus != l_operationStatus) ||
                m_operationStatusForce ||
                (std::chrono::system_clock::now() - last_opstatus >
                 std::chrono::milliseconds(500)))
            {
                last_opstatus = std::chrono::system_clock::now();
                operationStatusSignal(m_operationStatus);
                m_operationStatusForce = false;
            }

        end:
            std::this_thread::sleep_for(
                std::chrono::milliseconds(m_updaterTimeout));
        }
    }
};

#endif // MITSUBISHITANDEM_H
