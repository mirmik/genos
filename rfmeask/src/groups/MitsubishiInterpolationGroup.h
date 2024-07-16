/**
    @file
*/

#ifndef MITSUBISHIINTERPOLATIONGROUP_H
#define MITSUBISHIINTERPOLATIONGROUP_H

#if (HAVE_INTERPOLATION)

#include <Axis.h>
#include <groups/InterpolationGroup.h>
#include <vector>

class MitsubishiInterpolationGroup : public InterpolationGroup
{
    std::vector<Axis *> axes;
    std::vector<Axis *> allaxes;

    bool _is_operated;
    bool checker_thread_cancel_token;
    std::thread checker_thread;

    int points_by_axis = 50;
    int part_size = 25;

    int uploaded_part = 0;
    int parts = 0;
    int loops;

    uint8_t base;
    igris::ngeom::multiline mline;
    std::vector<double> spds;
    bool is_absolute;

    void upload_points(int start, int end);
    void upload_part(int part);
    void upload_if_need(int driven);
    int need_to_by_uploaded(int driven);
    int part_by_point(int pnt);

public:
    void checker_thread_function();

    void start_one_point_operation(const std::vector<double> &vec) override;

    void init_operation();
    void finalize();

    void start_interpolation_operation();
    int relative_move(const igris::ngeom::multiline &mline,
                      uint8_t base,
                      const std::vector<double> &spds) override;
    int absolute_move(const igris::ngeom::multiline &mline,
                      uint8_t base,
                      const std::vector<double> &spds) override;

    // int correction_move_by_axis(int ax, int64_t dist, int32_t
    // spd) override; int correction_unit_move_by_axis(int ax,
    // double dist, int32_t spd) override;

    void stop() override;

    MitsubishiInterpolationGroup(std::string idn,
                                 int dim,
                                 std::vector<Axis *> &axes,
                                 nos::trent_settings &stgs) :
        InterpolationGroup(idn, dim, stgs), axes(axes)
    {
    }

    void set_allaxes(std::vector<Axis *> v)
    {
        allaxes = v;
        _dim_allaxes = allaxes.size();
    }
};

#endif

#endif // MITSUBISHIINTERPOLATIONGROUP_H
