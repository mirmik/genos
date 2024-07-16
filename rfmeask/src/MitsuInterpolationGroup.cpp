/**
    @file
*/

#if (HAVE_INTERPOLATION)

#include <MitsubishiBoardServo.h>
#include <MitsubishiInterpolationGroup.h>

#define SSC_SUBCMD_PNT_LOOP_START (0x0800)
#define SSC_SUBCMD_PNT_LOOP_END (0x1000)

const char *ssc_strerr(int err)
{
    switch (err)
    {
    case 1:
        return "SSC_FUNC_ERR_ARGUMENT_01";
    case 2:
        return "SSC_FUNC_ERR_ARGUMENT_02";
    case 3:
        return "SSC_FUNC_ERR_ARGUMENT_03";
    case 4:
        return "SSC_FUNC_ERR_ARGUMENT_04";
    case 5:
        return "SSC_FUNC_ERR_ARGUMENT_05";
    case 6:
        return "SSC_FUNC_ERR_ARGUMENT_06";
    case 7:
        return "SSC_FUNC_ERR_ARGUMENT_07";
    case 8:
        return "SSC_FUNC_ERR_ARGUMENT_08";
    case 9:
        return "SSC_FUNC_ERR_ARGUMENT_09";
    case 0x000600A3:
        return "SSC_FUNC_ERR_STS_BIT_IPCH_OFF";
    default:
        return "UNKNOWN ERROR";
    }
}

void MitsubishiInterpolationGroup::upload_points(int start, int end)
{
    const Axis *ax;
    PNT_DATA_EX ipnt;
    uint8_t sts;

    memset(&ipnt, 0, sizeof(ipnt));
    for (unsigned int a = 0; a < _dim; ++a)
    {
        ax = axes[a];

        ipnt.actime = 200;
        ipnt.dctime = 200;
        sscSetPointOffset(0,
                          1,
                          static_cast<MBoardServoBasic *>(ax->mover)->master,
                          a * points_by_axis);
        for (int i = start; i < end; ++i)
        {
            ipnt.subcmd =
                (is_absolute ? SSC_SUBCMD_POS_ABS : SSC_SUBCMD_POS_INC) |
                SSC_SUBCMD_STOP_CONTINUE;
            if (i == 0)
            {
                ipnt.subcmd |= SSC_SUBCMD_PNT_LOOP_START;
            }
            if ((unsigned)i == mline.size() - 1)
            {
                ipnt.subcmd |= SSC_SUBCMD_PNT_LOOP_END;
            }
            ipnt.position = mline.mat.row_view(i)[a] * ax->m_unitRatio *
                            (ax->m_reversed ? -1 : 1);
            ipnt.speed = static_cast<MBoardServoBasic *>(ax->mover)
                             ->m_speed; // spds[i] * ax->mover->cupmToSpdKoeff()
                                        // * ax->m_unitRatio;

            sts = sscSetPointDataEx(
                0,
                1,
                static_cast<MBoardServoBasic *>(ax->mover)->master,
                (i % points_by_axis),
                &ipnt);
            (void)sts;
        }
        // if (a == 0)
        //	sscSetLatestPointNumber(
        //		0, 1, static_cast<MBoardServoBasic *>(ax->mover)->master,
        //		(end % points_by_axis == 0) ? points_by_axis : part_size);
    }
}

void MitsubishiInterpolationGroup::upload_part(int part)
{
    upload_points(part_size * part, part_size * part + part_size);
    uploaded_part = part;
}

void MitsubishiInterpolationGroup::upload_if_need(int driven)
{
    int curpart = part_by_point(driven + (loops)*points_by_axis);
    while (uploaded_part != parts && uploaded_part <= curpart)
    {
        upload_part(uploaded_part + 1);
    }
}

//Возвращает номер части, к которой принадлежит точка.
int MitsubishiInterpolationGroup::part_by_point(int point)
{
    return point / part_size;
}

int MitsubishiInterpolationGroup::relative_move(
    const igris::ngeom::multiline &mline,
    uint8_t base,
    const std::vector<double> &spds)
{
    nos::println("start_interpolaion_operation");

    this->mline = mline;
    this->base = base;
    this->spds = spds;
    this->is_absolute = false;
    start_interpolation_operation();
    return 0;
}

int MitsubishiInterpolationGroup::absolute_move(
    const igris::ngeom::multiline &mline,
    uint8_t base,
    const std::vector<double> &spds)
{
    return 0;
}

void MitsubishiInterpolationGroup::start_interpolation_operation()
{
    init_operation(); // Поддержка логического контроллера.

    loops = 0;
    uploaded_part = -1;
    parts = part_by_point(mline.size() - 1);

    upload_if_need(0);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    uint8_t sts = sscLinearStart(
        0,
        static_cast<MBoardServoBasic *>(axes[base]->mover)->channel,
        static_cast<MBoardServoBasic *>(axes[base]->mover)->master,
        static_cast<MBoardServoBasic *>(axes[base]->mover)->igroup,
        0,
        49);

    this->base = base;
    checker_thread_cancel_token = false;
    _is_operated = true;
    auto checker_thread = std::thread(
        &MitsubishiInterpolationGroup::checker_thread_function, this);
    checker_thread.detach();
}

void MitsubishiInterpolationGroup::start_one_point_operation(
    const std::vector<double> &prevec)
{
    Axis *ax;
    PNT_DATA_EX ipnt;
    uint8_t sts;

    init_operation();

    std::vector<double> vec;
    std::map<Axis *, double> submap;

    for (unsigned int i = 0; i < allaxes.size(); ++i)
    {
        bool insubax = false;
        for (unsigned int j = 0; j < subaxes.size(); ++j)
        {
            if (allaxes[i] == subaxes[j])
                insubax = true;
        }

        if (insubax)
            submap.insert(std::make_pair(allaxes[i], prevec[i]));
        else
            vec.push_back(prevec[i]);
    }

    for (auto p : submap)
    {
        p.first->absoluteUnitMove(p.second);
    }

    memset(&ipnt, 0, sizeof(ipnt));
    for (unsigned int a = 0; a < _dim; ++a)
    {
        ax = axes[a];

        if (uspeed != 0)
            ax->setSpeed_unitPerSecond(uspeed);

        ipnt.actime = 8000;
        ipnt.dctime = 8000;
        sscSetPointOffset(0,
                          1,
                          static_cast<MBoardServoBasic *>(ax->mover)->master,
                          a * points_by_axis);

        ipnt.subcmd = SSC_SUBCMD_POS_ABS;
        ipnt.position = vec[a] * ax->m_unitRatio * (ax->m_reversed ? -1 : 1);
        ipnt.speed = ax->mover->settedSpeed_machine();

        sts = sscSetPointDataEx(
            0, 1, static_cast<MBoardServoBasic *>(ax->mover)->master, 0, &ipnt);
        (void)sts;
    }

    sts = sscLinearStart(
        0,
        static_cast<MBoardServoBasic *>(axes[base]->mover)->channel,
        static_cast<MBoardServoBasic *>(axes[base]->mover)->master,
        static_cast<MBoardServoBasic *>(axes[base]->mover)->igroup,
        0,
        0);
    (void)sts;
}

void MitsubishiInterpolationGroup::checker_thread_function()
{
    int latest_printed = -1;
    while (1)
    {
        if (checker_thread_cancel_token)
            break;
        short last;
        sscGetDrivingPointNumber(
            0,
            1,
            static_cast<MBoardServoBasic *>(axes[base]->mover)->master,
            &last);
        if (latest_printed > last)
            loops += 1;
        if (last % part_size >= part_size / 2)
            upload_if_need(last);
        if (latest_printed != last)
        {
            latest_printed = last;
        }
        int fin_type = SSC_FIN_TYPE_SMZ;
        int fin_status;
        sscGetDriveFinStatus(
            0,
            1,
            static_cast<MBoardServoBasic *>(axes[base]->mover)->master,
            fin_type,
            &fin_status);
        if (fin_status == 1)
        {
            nos::println("BREAK!!!!!!!!!!!!!!!!!!!");
            finalize();
            break;
        }
    }
}

void MitsubishiInterpolationGroup::init_operation()
{
    uploaded_part = 0;
    parts = 0;
    loops = 0;

    for (Axis *ax : axes)
    {
        // ax->stop();
        ax->op = Axis::Operation::USERMOVE;
        ax->controller = this;
    }
}

void MitsubishiInterpolationGroup::finalize()
{
    checker_thread_cancel_token = true;
    for (Axis *ax : axes)
    {
        ax->op = Axis::Operation::NONE;
        ax->controller = nullptr;
    }
}

void MitsubishiInterpolationGroup::stop()
{
    for (Axis *ax : axes)
    {
        ax->stopLastOperation(Axis::MANUAL);
    }
    finalize();
}

#endif
