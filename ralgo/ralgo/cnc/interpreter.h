#ifndef RALGO_CNC_INTERPRETER_H
#define RALGO_CNC_INTERPRETER_H

#include <cstdlib>
#include <igris/container/array_view.h>
#include <igris/container/flat_map.h>
#include <igris/container/ring.h>
#include <igris/container/static_vector.h>
#include <igris/datastruct/argvc.h>
#include <igris/shell/callable_collection.h>
#include <igris/sync/syslock.h>
#include <igris/util/numconvert.h>
#include <nos/ilist.h>
#include <nos/io/string_writer.h>
#include <nos/log/logger.h>
#include <nos/print/stdtype.h>
#include <nos/shell/executor.h>
#include <nos/util/string.h>
#include <ralgo/cnc/control_task.h>
#include <ralgo/cnc/feedback_guard.h>
#include <ralgo/cnc/planblock.h>
#include <ralgo/cnc/planner.h>
#include <ralgo/cnc/revolver.h>
#include <ralgo/cnc/util.h>
#include <ralgo/global_protection.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/log.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace cnc
{
    class interpreter
    {
    public:
        cnc::planner *planner = nullptr;
        cnc::revolver *revolver = nullptr;
        cnc::feedback_guard *feedback_guard = nullptr;

    private:
        igris::ring<planner_block> *blocks = nullptr;
        // igris::ring<cnc::control_shift> *shifts = nullptr;
        int total_axes = 0;
        cnc_float_type revolver_frequency = 0;

        // Мультипликатор на входе интерпретатора.
        // Применяется к позициям, скоростям и ускорениям во входном потоке
        // комманд. Есть подозрение, что механика gains переусложнена и его надо
        // заменить на единый множитель для всех осей.
        igris::static_vector<cnc_float_type, NMAX_AXES> gains = {};

        igris::static_vector<cnc_float_type, NMAX_AXES> _final_position = {};
        igris::static_vector<cnc_float_type, NMAX_AXES> max_axes_velocities =
            {};
        igris::static_vector<cnc_float_type, NMAX_AXES> max_axes_accelerations =
            {};
        int blockno = 0;
        cnc_float_type saved_acc = 0;
        cnc_float_type saved_feed = 0;
        cnc_float_type _smooth_stop_acceleration = 0;
        igris::delegate<void> _external_final_shift_handle = {};
        planner_block lastblock = {};
        bool with_cleanup = true;
        volatile bool stop_procedure_started = false;

    public:
        interpreter(igris::ring<planner_block> *blocks,
                    cnc::planner *planner,
                    cnc::revolver *revolver,
                    cnc::feedback_guard *feedback_guard)
            : planner(planner), revolver(revolver),
              feedback_guard(feedback_guard), blocks(blocks)
        {
        }

        interpreter(const interpreter &) = delete;
        interpreter(interpreter &&) = delete;
        interpreter &operator=(const interpreter &) = delete;
        interpreter &operator=(interpreter &&) = delete;

        void cleanup()
        {
            blockno = 0;
            lastblock = {};
        }

        cnc_float_type smooth_stop_acceleration(
            const ralgo::vector<cnc_float_type> &direction) const
        {
            cnc_float_type maximum_acceleration =
                _smooth_stop_acceleration != 0 ? _smooth_stop_acceleration
                                               : saved_acc;
            return evaluate_external_accfeed_2(
                direction, maximum_acceleration, max_axes_accelerations);
        }

        const igris::static_vector<cnc_float_type, NMAX_AXES> &final_position()
        {
            return _final_position;
        }

        planner_block last_block()
        {
            return lastblock;
        }

        void set_final_shift_handler(const igris::delegate<void> &dlg)
        {
            _external_final_shift_handle = dlg;
        }

        void set_start_shift_handler(const igris::delegate<void> &dlg)
        {
            planner->set_start_operation_handle(dlg);
        }

        void init_axes(int total_axes)
        {
            this->total_axes = total_axes;
            gains.resize(total_axes);
            _final_position.resize(total_axes);
            max_axes_velocities.resize(total_axes);
            max_axes_accelerations.resize(total_axes);
            planner->set_axes_count(total_axes);
            ralgo::vecops::fill(gains, 1);
            ralgo::vecops::fill(_final_position, 0);
            ralgo::vecops::fill(max_axes_accelerations, 0);
            ralgo::vecops::fill(max_axes_velocities, 0);

            planner->final_shift_pushed =
                igris::make_delegate(&interpreter::final_shift_handle, this);
        }

        void restore_finishes()
        {
            auto steps = revolver->current_steps();
            for (int i = 0; i < total_axes; ++i)
                _final_position[i] = steps[i] * planner->revolver->get_gear(i);
        }

        void final_shift_handle()
        {
            if (blocks->avail() == 0)
            {
                restore_finishes();
                stop_procedure_started = false;

                if (with_cleanup)
                {
                    planner->cleanup();
                    revolver->cleanup();
                    cleanup();
                }

                if (_external_final_shift_handle)
                    _external_final_shift_handle();
            }
            if (feedback_guard)
                feedback_guard->finish_operations();
        }

        int check_correctness(nos::ostream &os)
        {
            assert(total_axes == (int)planner->get_total_axes());
            assert(total_axes == revolver->steppers_total);

            if (ralgo::global_protection)
            {
                nos::println_to(os, "need_to_disable_global_protection");
                return 1;
            }
            if (revolver_frequency == 0)
            {
                nos::println_to(os, "revolver_frequency is null");
                return 1;
            }
            for (auto &i : gains)
            {
                if (i != 1)
                {
                    nos::println_to(os,
                                    "gains must be 1, becourse another variant "
                                    "is not tested");
                    return 1;
                }
            }
            return 0;
        }

        control_task
        create_task_for_relative_move(const std::vector<idxpos> poses,
                                      cnc_float_type feed,
                                      cnc_float_type acc)
        {
            control_task task(total_axes);
            task.feed = feed == 0 ? saved_feed : feed;
            task.acc = acc == 0 ? saved_acc : acc;
            for (auto &i : poses)
            {
                task.poses()[i.idx] = i.pos;
            }
            task.isok = true;
            task.set_active_axes(poses);
            return task;
        }

        control_task
        create_task_for_absolute_move(const std::vector<idxpos> poses,
                                      cnc_float_type feed,
                                      cnc_float_type acc)
        {
            control_task task(total_axes);
            task.feed = feed == 0 ? saved_feed : feed;
            task.acc = acc == 0 ? saved_acc : acc;
            auto finalpos = _final_position;
            task.set_poses({finalpos.data(), finalpos.size()});
            for (auto &i : poses)
            {
                task.poses()[i.idx] = i.pos;
            }
            for (int i = 0; i < total_axes; ++i)
            {
                task.poses()[i] -= finalpos[i];
            }
            task.isok = true;
            task.set_active_axes(poses);
            return task;
        }

        /*control_task g1_parse_task(const nos::argv &argv)
        {
            control_task task(total_axes);

            task.feed = saved_feed;
            task.acc = saved_acc;
            int sts = task.parse(argv);
            if (sts)
            {
                ralgo::warn("wrong task format");
                task.isok = false;
                return task;
            }

            saved_feed = task.feed;
            saved_acc = task.acc;
            task.isok = true;
            return task;
        }*/

        /*control_task g1_parse_task(const nos::argv &argv,
                                   const igris::array_view<cnc_float_type>
        &fposes)
        {
            control_task task(total_axes);
            task.set_poses(igris::array_view<cnc_float_type>((cnc_float_type
        *)fposes.data(), fposes.size()));

            task.feed = saved_feed;
            task.acc = saved_acc;
            int sts = task.parse(argv);
            if (sts)
            {
                ralgo::warn("wrong task format");
                task.isok = false;
                return task;
            }

            saved_feed = task.feed;
            saved_acc = task.acc;
            task.isok = true;
            return task;
        }*/

        /// Расщитывает ускорение или скорость для блока на основании
        /// запрошенных скоростей или ускорений для отдельных осей и
        /// скоростей или ускорений для точки в евклидовом пространстве.
        static cnc_float_type evaluate_external_accfeed(
            const ralgo::vector<cnc_float_type> &direction,
            cnc_float_type absolute_maximum,
            const igris::static_vector<cnc_float_type, NMAX_AXES>
                &element_maximums)
        {
            int total_axes = (int)direction.size();
            cnc_float_type minmul = std::numeric_limits<cnc_float_type>::max();

            if (absolute_maximum == 0 &&
                ralgo::vecops::norm(element_maximums) == 0)
                return 0;

            if (absolute_maximum != 0)
                minmul = absolute_maximum;

            for (int i = 0; i < total_axes; i++)
                if (element_maximums[i] != 0)
                {
                    cnc_float_type lmul =
                        element_maximums[i] / fabs(direction[i]);
                    if (minmul > lmul)
                        minmul = lmul;
                }

            if (minmul == std::numeric_limits<cnc_float_type>::max())
                return 1000;
            return minmul;
        }

        static cnc_float_type evaluate_external_accfeed_2(
            const ralgo::vector<cnc_float_type> &direction,
            cnc_float_type absolute_maximum,
            const igris::static_vector<cnc_float_type, NMAX_AXES>
                &element_maximums)
        {
            if (ralgo::vecops::norm(element_maximums) == 0)
            {
                return absolute_maximum;
            }

            if (absolute_maximum == 0)
            {
                auto bounded =
                    ralgo::vecops::ray_to_box<ralgo::vector<cnc_float_type>>(
                        direction, element_maximums);
                return ralgo::vecops::norm(bounded);
            }
            else
            {
                auto vec = ralgo::vecops::mul_vs(direction, absolute_maximum);
                auto bounded =
                    ralgo::vecops::bound_to_box<ralgo::vector<cnc_float_type>>(
                        vec, element_maximums);
                return ralgo::vecops::norm(bounded);
            }
        }

        bool evaluate_interpreter_task(const control_task &task,
                                       planner_block &block,
                                       nos::ostream &)
        {
            saved_acc = task.acc;
            saved_feed = task.feed;

            cnc_float_type tasknorm = ralgo::vecops::norm(task.poses());
            if (tasknorm == 0)
                return true;

            // Это записано для инкрементального режима:
            auto intdists =
                ralgo::vecops::mul_vv<ralgo::vector<cnc_float_type>>(
                    task.poses(), gains);

            auto direction =
                ralgo::vecops::normalize<ralgo::vector<cnc_float_type>>(
                    task.poses());

            auto dirgain =
                ralgo::vecops::norm(ralgo::vecops::mul_vv(direction, gains));

            auto evalfeed = evaluate_external_accfeed_2(
                direction, task.feed, max_axes_velocities);
            auto evalacc = evaluate_external_accfeed_2(
                direction, task.acc, max_axes_accelerations);

            if (evalacc == 0)
            {
                ralgo::info("Acceleration is not defined");
                return true;
            }

            if (evalfeed == 0)
            {
                ralgo::info("Feed is not defined");
                return true;
            }

            cnc_float_type feed = evalfeed * dirgain;
            cnc_float_type acc = evalacc * dirgain;

            // scale feed and acc by revolver freqs settings.
            cnc_float_type reduced_feed = feed / revolver_frequency;
            cnc_float_type reduced_acc =
                acc / (revolver_frequency * revolver_frequency);

            if (feedback_guard)
                for (auto idx : task.active_axes)
                {
                    feedback_guard->enable_tandem_protection_for_axis(idx);
                }

            // output
            block.set_state(intdists, total_axes, reduced_feed, reduced_acc);
            return false;
        }

        std::vector<cnc_float_type> final_gained_position()
        {
            std::vector<cnc_float_type> ret(total_axes);
            for (int i = 0; i < total_axes; ++i)
                ret[i] = _final_position[i] / gains[i];
            return ret;
        }

        void set_scale(const ralgo::vector_view<cnc_float_type> &vec)
        {
            if (vec.size() != static_cast<size_t>(total_axes))
            {
                ralgo::warn("set_scale fail");
            }
            std::copy(vec.begin(), vec.end(), gains.begin());
        }

        void set_saved_acc(cnc_float_type acc)
        {
            saved_acc = acc;
        }

        void set_saved_feed(cnc_float_type feed)
        {
            saved_feed = feed;
        }

        void evaluate_task(const control_task &task, nos::ostream &os)
        {
            bool fastfinish = evaluate_interpreter_task(task, lastblock, os);
            if (fastfinish)
            {
                _external_final_shift_handle();
                ralgo::info("fastfinish");
                //    print_interpreter_state(os);
                return;
            }

            for (int i = 0; i < total_axes; ++i)
                _final_position[i] += lastblock.axdist[i];

            auto &placeblock = blocks->head_place();
            lastblock.blockno = blockno++;
            placeblock = lastblock;

            system_lock();
            blocks->move_head_one();
            system_unlock();
        }

        void command_incremental_move(const nos::argv &argv, nos::ostream &os)
        {
            if (stop_procedure_started)
                return;

            ralgo::info("command_incremental_move");

            if (check_correctness(os))
                return;
            auto poses = get_task_poses_from_argv(argv);
            cnc_float_type feed = get_task_feed_from_argv(argv);
            cnc_float_type acc = get_task_acc_from_argv(argv);
            auto task = create_task_for_relative_move(poses, feed, acc);
            if (!task.isok)
                return;
            evaluate_task(task, os);
        }

        void command_absolute_move(const nos::argv &argv, nos::ostream &os)
        {
            if (stop_procedure_started)
                return;

            ralgo::info("command_absolute_move");

            if (check_correctness(os))
                return;
            auto poses = get_task_poses_from_argv(argv);
            cnc_float_type feed = get_task_feed_from_argv(argv);
            cnc_float_type acc = get_task_acc_from_argv(argv);
            auto task = create_task_for_absolute_move(poses, feed, acc);

            if (!task.isok)
            {
                ralgo::info("command_absolute_move: task is not ok");
                return;
            }
            evaluate_task(task, os);
        }

        void command_absolute_pulses(const nos::argv &argv, nos::ostream &os)
        {
            if (stop_procedure_started)
                return;

            ralgo::info("command_absolute_move");

            if (check_correctness(os))
                return;

            auto poses = get_task_poses_from_argv(argv);
            cnc_float_type feed = get_task_feed_from_argv(argv);
            cnc_float_type acc = get_task_acc_from_argv(argv);
            (void)poses;
            (void)feed;
            (void)acc;

            // TODO: implement

            return;
        }

        void inspect_args(const nos::argv &argv, nos::ostream &os)
        {
            PRINTTO(os, argv.size());
            for (auto &arg : argv)
            {
                PRINTTO(os, arg);
            }
        }

        void command_M204(const nos::argv &argv, nos::ostream &os)
        {
            if (argv.size() == 0)
            {
                nos::print_to(os, "%f", saved_acc);
                return;
            }

            inspect_args(argv, os);

            saved_acc = igris_atof64(argv[0].data(), nullptr);
            PRINTTO(os, saved_acc);
        }

        // Включить режим остановки.
        void command_M112(const nos::argv &, nos::ostream &)
        {
            system_lock();
            // blocks->clear();
            // plan_stop_task();
            system_unlock();
        }

        void smooth_stop()
        {
            ralgo::info("SMOOTH_STOP");

            system_lock();
            if (stop_procedure_started)
            {
                ralgo::info("prevented because stop procedure is started");
                system_unlock();
                return;
            }

            auto curvels = revolver->current_velocities();
            auto cursteps = revolver->current_steps_no_lock();

            if (ralgo::vecops::norm(curvels) < 1e-5)
            {
                ralgo::info("prevented because not moving");
                system_unlock();
                return;
            }

            auto direction =
                ralgo::vecops::normalize<ralgo::vector<cnc_float_type>>(
                    curvels);
            auto external_acceleration = smooth_stop_acceleration(direction) /
                                         revolver_frequency /
                                         revolver_frequency;

            auto velocity = ralgo::vecops::norm(curvels);
            if (velocity == 0)
            {
                ralgo::info("prevented because velocity is not valid");
                system_unlock();
                return;
            }

            bool is_valid = lastblock.set_stop_pattern(
                total_axes, velocity, external_acceleration, direction);

            if (!is_valid)
            {
                nos::log::info("prevented because stop pattern is not valid");
                system_unlock();
                return;
            }

            restore_finishes();
            for (int i = 0; i < total_axes; ++i)
                _final_position[i] += lastblock.axdist[i];

            // shifts->clear();
            // revolver->clear();
            planner->clear_for_stop();
            planner->force_skip_all_blocks();
            // planner->set_current_velocity(curvels);
            auto &placeblock = blocks->head_place();
            lastblock.blockno = 0;
            placeblock = lastblock;
            blocks->move_head_one();
            stop_procedure_started = true;
            system_unlock();
        }

        void g_command(const nos::argv &argv, nos::ostream &os)
        {
            int cmd = atoi(&argv[0].data()[1]);
            switch (cmd)
            {
            case 1:
                command_incremental_move(argv.without(1), os);
                break;
            default:
                nos::println_to(os, "Unresolved G command");
            }
        }

        void m_command(const nos::argv &argv, nos::ostream &os)
        {
            int cmd = atoi(&argv[0].data()[1]);
            switch (cmd)
            {
            case 112:
                command_M204(argv.without(1), os);
                break;
            case 204:
                command_M204(argv.without(1), os);
                break;
            default:
                nos::println_to(os, "Unresolved M command");
            }
        }

        int gcode(const nos::argv &argv, nos::ostream &os)
        {
            if (argv.size() == 0)
                return 0;

            char cmdsymb = argv[0][0];
            switch (cmdsymb)
            {
            case 'G':
            {
                g_command(argv, os);
                break;
            }

            case 'M':
            {
                m_command(argv, os);
                break;
            }

            default:
                nos::fprintln_to(os, "Unresolved command: {}", argv[0]);
            }

            return 0;
        }

        std::vector<size_t> args_to_index_vector(const nos::argv &args)
        {
            std::vector<size_t> ret;
            for (unsigned int i = 0; i < args.size(); ++i)
            {
                auto index = std::stoi(args[i]);
                ret.push_back(index);
            }
            return ret;
        }

        std::vector<size_t> args_symbols_to_index_vector(const nos::argv &args)
        {
            std::vector<size_t> ret;
            for (unsigned int i = 0; i < args.size(); ++i)
            {
                auto index = symbol_to_index(args[i][0]);
                ret.push_back(index);
            }
            return ret;
        }

        int command(const nos::argv &argv, nos::ostream &os)
        {
            auto *fptr = clicommands.find(argv[0].data());
            if (fptr)
            {
                return (*fptr)(argv, os);
            }

            nos::fprintln_to(
                os, "Unresolved command: {}", std::string(argv[0]));
            return 0;
        }

        int command_help(nos::ostream &os)
        {
            for (auto &rec : clicommands)
            {
                nos::fprintln_to(os, "{} - {}", rec.key, rec.help);
            }
            return 0;
        }

        igris::static_callable_collection<int(const nos::argv &,
                                              nos::ostream &),
                                          50>
            clicommands{
                {"setprotect",
                 "setprotect",
                 [](const nos::argv &, nos::ostream &) {
                     ralgo::global_protection = false;
                     ralgo::info("Protection disabled");
                     return 0;
                 }},
                {"stop",
                 "stop",
                 [this](const nos::argv &, nos::ostream &) {
                     smooth_stop();
                     return 0;
                 }},

                {"lastblock",
                 "lastblock",
                 [this](const nos::argv &, nos::ostream &os) {
                     last_block().print_to_stream(os);
                     return 0;
                 }},

                {"relmove",
                 "relmove",
                 [this](const nos::argv &argv, nos::ostream &os) {
                     command_incremental_move(argv.without(1), os);
                     return 0;
                 }},

                {"absmove",
                 "absmove",
                 [this](const nos::argv &argv, nos::ostream &os) {
                     command_absolute_move(argv.without(1), os);
                     return 0;
                 }},

                {"abspulses",
                 "abspulses",
                 [this](const nos::argv &argv, nos::ostream &os) {
                     command_absolute_pulses(argv.without(1), os);
                     return 0;
                 }},

                {"steps",
                 "steps",
                 [this](const nos::argv &, nos::ostream &os) {
                     nos::println_to(os, current_steps());
                     return 0;
                 }},

                {"finishes",
                 "finishes",
                 [this](const nos::argv &, nos::ostream &os) {
                     nos::print_list_to(os, _final_position);
                     nos::println_to(os);
                     return 0;
                 }},

                {"gains",
                 "gains",
                 [this](const nos::argv &, nos::ostream &os) {
                     nos::print_list_to(os, gains);
                     nos::println_to(os);
                     return 0;
                 }},

                {"gears",
                 "gears",
                 [this](const nos::argv &, nos::ostream &os) {
                     auto gears = planner->get_gears();
                     nos::print_list_to(os, gears);
                     nos::println_to(os);
                     return 0;
                 }},

                {"setgear",
                 "setgear",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto axno = symbol_to_index(argv[1][0]);
                     cnc_float_type val = igris_atof64(argv[2].data(), NULL);
                     planner->set_gear(axno, val);
                     feedback_guard->set_control_to_drive_multiplier(axno, val);
                     return 0;
                 }},

                {"set_control_gear",
                 "set_control_gear",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto axno = symbol_to_index(argv[1][0]);
                     cnc_float_type val = igris_atof64(argv[2].data(), NULL);
                     planner->set_gear(axno, val);
                     feedback_guard->set_control_to_drive_multiplier(axno, val);
                     return 0;
                 }},

                {"set_feedback_gear",
                 "set_feedback_gear",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto axno = symbol_to_index(argv[1][0]);
                     cnc_float_type val = igris_atof64(argv[2].data(), NULL);
                     feedback_guard->set_feedback_to_drive_multiplier(axno,
                                                                      val);
                     return 0;
                 }},

                {"setpos",
                 "setpos",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto axno = symbol_to_index(argv[1][0]);
                     cnc_float_type val = igris_atof64(argv[2].data(), NULL);
                     system_lock();
                     _final_position[axno] =
                         val * planner->revolver->get_gear(axno);
                     revolver->get_steppers()[axno]->set_counter_value(val);
                     feedback_guard->set_feedback_position(
                         axno, _final_position[axno]);
                     system_unlock();
                     return 0;
                 }},
                {"disable_tandem_protection",
                 "disable_tandem_protection",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto idx = std::stoi(argv[1]);
                     feedback_guard->remove_tandem(idx);
                     return 0;
                 }},
                {"enable_tandem_protection",
                 "enable_tandem_protection",
                 [this](const nos::argv &argv, nos::ostream &os) {
                     feedback_guard->add_tandem_command(argv.without(1), os);
                     return 0;
                 }},
                {"tandem_info",
                 "tandem_info",
                 [this](const nos::argv &, nos::ostream &os) {
                     const auto &tandems = feedback_guard->tandems();
                     if (tandems.size() == 0)
                     {
                         nos::println_to(os, "Tandem list is empty");
                     }
                     for (auto &tandem : tandems)
                     {
                         nos::println_to(os, tandem.info());
                     }
                     return 0;
                 }},
                {"drop_pulses_allowed",
                 "drop_pulses_allowed",
                 [this](const nos::argv &argv, nos::ostream &os) {
                     size_t no = std::stoi(argv[1]);

                     if (argv.size() > 2)
                     {
                         int64_t pulses = std::stoi(argv[1]);
                         feedback_guard->set_drop_pulses_allowed(no, pulses);
                     }
                     else
                     {
                         nos::println_to(
                             os, feedback_guard->drop_pulses_allowed(no));
                     }
                     return 0;
                 }},
                {"velmaxs",
                 "Set maximum velocity for axes",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto fmap = args_to_index_value_map(argv.without(1));
                     for (auto &[key, val] : fmap)
                     {
                         max_axes_velocities[key] = val;
                     }
                     return 0;
                 }},
                {"accmaxs",
                 "Set maximum accelerations for axes",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto fmap = args_to_index_value_map(argv.without(1));
                     for (auto &[key, val] : fmap)
                         max_axes_accelerations[key] = val;
                     return 0;
                 }},
                {"help",
                 "print this help",
                 [this](const nos::argv &, nos::ostream &os) {
                     command_help(os);
                     return 0;
                 }},
                {"state",
                 "print interpreter state",
                 [this](const nos::argv &, nos::ostream &os) {
                     return print_interpreter_state(os);
                 }},

                {"guard_info",
                 "print guard info",
                 [this](const nos::argv &, nos::ostream &os) {
                     return feedback_guard->guard_info(os);
                 }},

                {"planner_pause",
                 "Stop plannering",
                 [this](const nos::argv &argv, nos::ostream &) {
                     auto en = std::stoi(argv[1]);
                     planner->set_pause_mode(en);
                     return 0;
                 }}};

        int gcode_drop_first(const nos::argv &argv, nos::ostream &os)
        {
            return gcode(argv.without(1), os);
        }

        int command_drop_first(const nos::argv &argv, nos::ostream &os)
        {
            if (argv.size() == 1)
            {
                nos::println_to(os, "No command");
                return command_help(os);
            }

            return command(argv.without(1), os);
        }

        std::vector<int64_t> current_steps()
        {
            std::vector<int64_t> vect(total_axes);
            revolver->current_steps(vect.data());
            return vect;
        }

        int print_interpreter_state(nos::ostream &os)
        {
            PRINTTO(os, revolver_frequency);
            PRINTTO(os, saved_acc);
            PRINTTO(os, saved_feed);
            // PRINTTO(os, shifts->head_index());
            // PRINTTO(os, shifts->tail_index());
            nos::print_to(os, "vel: ");
            nos::print_list_to(os, revolver->current_velocities());
            nos::println_to(os);
            nos::print_to(os, "acc:");
            nos::print_list_to(os, planner->accelerations);
            nos::println_to(os);
            nos::print_to(os, "gains:");
            nos::print_list_to(os, gains);
            nos::println_to(os);
            nos::print_to(os, "gears:");
            nos::print_list_to(os, planner->get_gears());
            nos::println_to(os);
            nos::print_to(os, "active_block: ");
            nos::println_to(os, planner->active_block != nullptr);
            nos::print_to(os, "active: ");
            nos::println_to(os, planner->active);
            nos::print_to(os, "head: ");
            nos::println_to(os, planner->blocks->head_index());
            return 0;
        }

        nos::executor executor = nos::executor({
            {"cmd",
             "commands",
             nos::make_delegate(&interpreter::command_drop_first, this)},
            {"cnc",
             "gcode commands",
             nos::make_delegate(&interpreter::gcode_drop_first, this)},
        });

        std::string newline(const char *line, size_t)
        {
            nos::string_buffer output;
            executor.execute(nos::tokens(line), output);
            return output.str();
        }

        std::string newline(const std::string &line)
        {
            return newline(line.c_str(), line.size());
        }

        void set_revolver_frequency(cnc_float_type freq)
        {
            revolver_frequency = freq;
        }

        void set_axes_count(int total)
        {
            total_axes = total;
        }

        size_t get_axes_count()
        {
            return total_axes;
        }
    };
}

#endif
