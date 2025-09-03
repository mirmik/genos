#include <MitsubishiCommunicator.h>
#include <servos/MRJ5Servo.h>

MRJ5Servo::MRJ5Servo(const char *name,
                     MitsubishiCommunicator *mrs,
                     uint8_t rs485addr) :
    MitsubishiServo(name, mrs, rs485addr)

{
}

uint8_t MRJ5Servo::request_operation_status()
{
    uint8_t errstat = 0;
    uint8_t ret;
    auto outs = m_mrs->output_signals(errstat, m_rs485addr);
    uint8_t zsp = outs & (1 << 2);

    if (!zsp)
        ret = MOVED;
    else
        ret = STOPED;

    errstatAnalyze(errstat, NONCRITICAL);

    return ret;
}