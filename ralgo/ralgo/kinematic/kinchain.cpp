#include <ralgo/kinematic/kinchain.h>

void ralgo::kinematic_chain_sensivities(ralgo::pose3<double> *constants,
                                        ralgo::screw3<double> *locsenses,
                                        double *coords,
                                        int dim,
                                        ralgo::screw3<double> *outsenses)
{
    ralgo::pose3<double> temp;
    for (int i = dim - 1; i >= 0; --i)
    {
        auto W = ralgo::pose3<double>::from_screw(locsenses[i] * coords[i]);
        auto C = constants[i + 1];
        temp = W * C * temp;

        outsenses[i] = locsenses[i].kinematic_carry(temp);
    }

    temp = constants[0];
    for (int i = 0; i < dim; ++i)
    {
        outsenses[i] = temp.rotate_screw(outsenses[i]);

        if (i == dim - 1)
            break;

        auto W = ralgo::pose3<double>::from_screw(locsenses[i] * coords[i]);
        auto C = constants[i + 1];
        temp = temp * W * C;
    }
}
