#ifndef RALGO_RXPID_H
#define RALGO_RXPID_H

#include <initializer_list>
#include <rxcpp/rx.hpp>
#include <rxcpp/subjects/rx-subject.hpp>

namespace ralgo
{
    template <class T>
    auto rxintegral(const rxcpp::observable<T> &source, double delta)
    {
        return source.scan(
            T{}, [delta](T acc, T value) { return acc + value * delta; });
    }

    template <class T>
    auto rxderivative(const rxcpp::observable<T> &source, double delta)
    {
        double last = 0;
        double inverse_delta = 1.0 / delta;
        return source | rxcpp::operators::transform(
                            [inverse_delta, last](T value) mutable {
                                auto diff = value - last;
                                last = value;
                                return diff * inverse_delta;
                            });
    }

    template <class T> auto rxlist(std::vector<T> values)
    {
        return rxcpp::observable<>::create<T>([values](auto subscriber) {
            for (auto value : values)
                subscriber.on_next(value);
            subscriber.on_completed();
        });
    }

    template <class T>
    auto rxpid(double kp,
               double ki,
               double kd,
               double delta,
               const rxcpp::observable<T> &error)
    {
        auto integral = rxintegral<T>(error, delta);
        auto derivative = rxderivative<T>(error, delta);

        (void)kp;
        (void)ki;
        (void)kd;
        (void)delta;
        (void)integral;
        (void)derivative;

        auto zip = error | rxcpp::operators::zip(integral, derivative);

        return zip |
               rxcpp::operators::transform([kp, ki, kd](const auto &args) {
                   auto [error, integral, derivative] = args;
                   return kp * error + ki * integral + kd * derivative;
               });
    }
}

#endif