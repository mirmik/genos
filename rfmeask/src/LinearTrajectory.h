#ifndef LINEAR_TRAJECTORY_H
#define LINEAR_TRAJECTORY_H

class LinearTrajectory
{
    bool absolute;
    std::vector<std::vector<double>> coords;
    PolymorphSpeed vel;
    PolymorphAccel acc;
};

#endif