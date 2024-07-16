#ifndef MRJ5_SERVO
#define MRJ5_SERVO

#include <servos/MitsubishiServo.h>


class MRJ5Servo : public MitsubishiServo
{

    static constexpr int c_drive_pulse_per_rotation = 67108864; //< 2**26

 public:   
    double drive_impulse_per_round() const override
    {
        return c_drive_pulse_per_rotation;
    }

    std::string declared_type() const override
    {
        return "mitsuservo-MR_J5_A";
    }

    MRJ5Servo(const char *name,
                    MitsubishiCommunicator *mrs,
                    uint8_t rs485addr);
    MRJ5Servo(const MitsubishiServo &) = delete;
    MRJ5Servo &operator=(const MitsubishiServo &) = delete;

};

#endif // MRJ5_SERVO