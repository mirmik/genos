#ifndef RALGO_HEIMER_AXSTATE_POSE3_PROCESSOR_H
#define RALGO_HEIMER_AXSTATE_POSE3_PROCESSOR_H

#include <ralgo/heimer/axstate_signal_processor.h>
#include <ralgo/heimer/dof6_signal.h>
#include <ralgo/heimer/phase_signals.h>

#include <ralgo/space/pose3.h>
#include <ralgo/space/screw.h>

#include <ralgo/linalg/matrix_view.h>
#include <ralgo/linalg/svd.h>
#include <ralgo/linalg/vector_view.h>

namespace heimer
{
    class axstate_pose3_chain_settings
    {
    public:
        ralgo::pose3<double> constant_transform = {};
        ralgo::screw3<double> local_sensivity = {};
        heimer::axis_state *controlled = nullptr;

        bool is_rotation_link()
        {
            if (local_sensivity.lin == linalg::vec<double, 3>())
                return true;

            return false;
        }
    };

    // runtime
    class axstate_pose3_chain_temporary
    {
    public:
        // Винта ошибки, возникающий из-за нелинейности вращающих звеньев.
        ralgo::screw3<double> second_pow_error_screw = {};
        ralgo::pose3<double> left_transform = {};
        ralgo::pose3<double> right_transform = {};
        ralgo::screw3<double> result_screw = {};
    };

    /**
        Порядок правых осей: x y z .
    */
    class axstate_chain3_processor : public signal_processor
    {
    public:
        ralgo::pose3<double> first_constant_transform = {};
        axstate_pose3_chain_settings *settings = nullptr;
        axstate_pose3_chain_temporary *temporary = nullptr;

        ralgo::screw3<velocity_t> feedback_velocity = {};
        ralgo::screw3<velocity_t> control_velocity = {};
        ralgo::pose3<position_t> feedback_position = {};
        ralgo::pose3<position_t> control_position = {};

        uint8_t deactivation_enabled = 0;
        bool interrupt_situation = false;

        disctime_t last_time = {};

    public:
        axstate_chain3_processor(const axstate_chain3_processor &) = delete;
        axstate_chain3_processor &
        operator=(const axstate_chain3_processor &) = delete;
        axstate_chain3_processor(const char *name, int leftdim);
        void set_resources(axstate_pose3_chain_settings *settings,
                           axstate_pose3_chain_temporary *tsettings);

        void evaluate_feedback();
        ralgo::pose3<position_t> evaluate_target_position();
        ralgo::screw3<position_t> evaluate_target_velocity();
        ralgo::screw3<position_t> evaluate_position_error();

        int feedback(disctime_t time) override;
        virtual void feedback_output(const pose3<position_t> &,
                                     const screw3<velocity_t> &) = 0;

        int command(int argc, char **argv, char *output, int outmax) override;
        void deinit() override;
        void on_activate(disctime_t) override;

        axis_state *leftax(int i);
        void set_constant(int, double, double, double, double, double, double);
        void set_sensivity(int, double, double, double, double, double, double);

        void allocate_resources();

        int on_deactivation_request(disctime_t) override
        {
            ralgo::warn("on_deactivation_request");
            if (interrupt_situation)
            {
                interrupt_situation = false;
                return 0;
            }

            deactivation_enabled = true;
            return 1;
        }

        int leftsigtype(int) override
        {
            return SIGNAL_TYPE_AXIS_STATE;
        }

        signal_head *leftsig(int i) override
        {
            return settings[i].controlled;
        }

        void set_leftsig(int i, signal_head *sig) override
        {
            settings[i].controlled = static_cast<axis_state *>(sig);
        }

        void deactivation_routine(disctime_t time)
        {
            ralgo::warn("deactivation_routine");
            for (int i = 0; i < leftdim(); ++i)
            {
                leftax(i)->ctrvel = 0;
            }

            deactivation_enabled = false;
            _deactivate(time);
        }

        bool on_interrupt(disctime_t time) override
        {
            interrupt_situation = true;
            ralgo::warn("chainproc : on_interrupt handle");
            deactivation_routine(time);
            return false;
        }
    };

    class axstate_chain3_translation_processor : public axstate_chain3_processor
    {
        heimer::phase_signal<3> *rightside = nullptr;
        double compkoeff = compkoeff_timeconst(0.0001);

        double last_w[3] = {0, 0, 0};

    public:
        static double compkoeff_timeconst(double T)
        {
            return 1. / discrete_time_frequency() / T;
        }
        void set_compkoeff(double ck)
        {
            this->compkoeff = ck;
        }
        void set_compkoeff_timeconst(double T)
        {
            this->compkoeff = compkoeff_timeconst(T);
        }

        axstate_chain3_translation_processor(const char *name, int leftdim)
            : axstate_chain3_processor(name, leftdim)
        {
        }

        axstate_chain3_translation_processor(
            const axstate_chain3_translation_processor &) = delete;
        axstate_chain3_translation_processor &
        operator=(const axstate_chain3_translation_processor &) = delete;

        void feedback_output(const pose3<position_t> &,
                             const screw3<velocity_t> &) override
        {
            rightside->feedpos = feedback_position.lin;
            rightside->feedvel = feedback_velocity.lin;
        }

        signal_head *rightsig(int) override
        {
            return rightside;
        }
        int rightsigtype(int) override
        {
            return SIGNAL_TYPE_PHASE3;
        }
        void set_rightsig(int, signal_head *sig) override
        {
            rightside = static_cast<phase_signal<3> *>(sig);
        }

        const ralgo::pose3<double> &left_transform_before(int i)
        {
            return i == 0 ? first_constant_transform
                          : temporary[i - 1].left_transform;
        }

        int serve(disctime_t time) override
        {
            // Строки : 3
            // Столбцов : leftdim

            auto delta = time - last_time;
            last_time = time;

            auto poserr = rightside->ctrpos - feedback_position.lin;
            auto target = rightside->ctrvel + poserr * compkoeff * delta;

            nos::println(feedback_position.lin);
            nos::println(length(poserr));

            TEMPORARY_STORAGE(double, leftdim(), Rdata);
            TEMPORARY_STORAGE(double, leftdim(), Rdata_correction);
            TEMPORARY_STORAGE(double, 3 * leftdim(), Adata);
            TEMPORARY_STORAGE(double, leftdim(), Wdata);
            TEMPORARY_STORAGE(double, 3 * leftdim(), Udata);
            TEMPORARY_STORAGE(double, leftdim() * leftdim(), Vdata);

            ralgo::matrix_view<double> A(std::data(Adata),
                                         3,
                                         leftdim()); // Матрица чуствительности
            ralgo::matrix_view<double> U(std::data(Udata),
                                         3,
                                         leftdim()); // Левая матрица разложения
            ralgo::matrix_view<double> V(
                std::data(Vdata),
                leftdim(),
                leftdim()); // Правая матрица разложения
            ralgo::vector_view<double> W(std::data(Wdata),
                                         leftdim()); // Центральная матрица.
            ralgo::vector_view<double> R(
                std::data(Rdata),
                leftdim()); // Вектор результата (скорости звеньев).
            ralgo::vector_view<double> R_correction(
                std::data(Rdata_correction), leftdim()); // Вектор коррекции.
            ralgo::vector_view<double> T(&target[0],
                                         3); // Вектор целевой скорости

            for (int i = 0; i < leftdim(); ++i)
                for (int j = 0; j < 3; ++j)
                    A.at(j, i) = temporary[i].result_screw.lin[j];

            auto svd = ralgo::make_SVD(A, U, V, W);

            // Получаем решение обратной задачи.
            svd.solve(T, R);

            // Проверяем решение обратной задачи по вычисленным скоростям.
            /*linalg::vec<double, 3> check = {};
            for (int i = 0; i < leftdim(); ++i)
            {
                check += R[i] * temporary[i].result_screw.lin;
            }*/

            // Проверка сингулярности???
            for (int i = 0; i < 3; ++i)
            {
                if (last_w[i] > W[i] && W[i] < 1)
                {
                    ralgo::warn("chainproc : singular deactivation");
                    interrupt(time, false);
                    return 0;
                }

                last_w[i] = W[i];
            }

            if (delta != 0)
            {
                // Блок коррекции.
                linalg::vec<double, 3> correction_errvec_accumulator;
                for (int i = 0; i < leftdim(); ++i)
                {
                    if (settings[i].is_rotation_link())
                    {
                        auto angle = R[i] * delta / discrete_time_frequency();
                        auto angsin = 1 - cos(angle);
                        auto angsin2 = angle - sin(angle);

                        // Ищем направление радиуса.
                        // Это трансляционная часть правой матрицы.
                        // Спроецированная На плоскость перпендикулярную оси
                        // чуствительности.
                        auto rlin = temporary[i].right_transform.lin;
                        auto normsens =
                            normalize(settings[i].local_sensivity.ang);

                        auto rad = rlin - dot(rlin, normsens) * normsens;
                        auto tangvec =
                            cross(settings[i].local_sensivity.ang, rad);

                        auto local_errvec = angsin * rad;
                        auto local_errvec_tang = tangvec * angsin2;
                        auto errvec = left_transform_before(i).rotate_vector(
                            local_errvec + local_errvec_tang);

                        correction_errvec_accumulator +=
                            errvec / delta * discrete_time_frequency();
                    }
                }

                ralgo::vector_view<double> T_correction(
                    &correction_errvec_accumulator[0], 3);
                svd.solve(T_correction, R_correction);

                for (int i = 0; i < leftdim(); ++i)
                {
                    R[i] += R_correction[i];
                }
            }

            // Логика блока доводки.
            bool zerovel = true;
            for (int i = 0; i < leftdim(); ++i)
            {
                leftax(i)->ctrvel = R[i];
                if (fabs(R[i]) > 1e-12)
                {
                    zerovel = false;
                }
                leftax(i)->ctrpos = leftax(i)->feedpos;
            }

            if (deactivation_enabled && zerovel)
            {
                deactivation_routine(time);
            }

            return 0;
        }
    };
}

#endif
#include <ralgo/linalg/matrix_view.h>
