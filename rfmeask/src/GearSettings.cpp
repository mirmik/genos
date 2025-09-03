/**
    @file
*/

#include <GearSettings.h>
#include <MessageBus.h>
#include <nos/fprint.h>
#include <nos/io/sstream.h>
#include <nos/log.h>
#include <nos/trent/trent.h>
#include <ranges>
#include <stdexcept>

using namespace std::string_literals;
using namespace nos::argument_literal;

int GearSettings::print_to(nos::ostream &os) const
{
    int len = 0;
    len +=
        *nos::fprint_to(os,
                        "GearSettings(mul:{mul},g1:{g1},g2:{g2n},g2:{g2d},o1:{"
                        "o1},o2:{o2},b:{b},two:{two},lin:{lin})",
                        "mul"_a = mul,
                        "g1"_a = gear1inputRatio,
                        "g2n"_a = gear2num,
                        "g2d"_a = gear2den,
                        "o1"_a = outputMul,
                        "o2"_a = outputDiam,
                        "b"_a = beta,
                        "two"_a = twoLink,
                        "lin"_a = isLinear);
    return len;
}

void GearSettings::set_is_linear(bool isLinear)
{
    this->isLinear = isLinear;
}

std::string GearSettings::info() const
{
    nos::stringstream ss;
    print_to(ss);
    return ss.str();
}

std::unique_ptr<GearLink> GearSettings::parse_gear_link(const nos::trent &t)
{
    if (t["type"].as_string() == "reductor")
    {
        double mul = t["mul"].as_numer_except();
        return std::make_unique<ReductorGearLink>(mul);
    }
    else if (t["type"].as_string() == "converter_rev_degree")
    {
        return std::make_unique<RoundsToDegreeConverterLink>();
    }
    else if (t["type"].as_string() == "converter_degree_rev")
    {
        return std::make_unique<DegreeToRoundsConverterLink>();
    }
    else if (t["type"].as_string() == "converter_rad_degree")
    {
        return std::make_unique<RadiansToDegreeConverterLink>();
    }
    else if (t["type"].as_string() == "converter_degree_rad")
    {
        return std::make_unique<DegreeToRadiansConverterLink>();
    }
    else if (t["type"].as_string() == "converter_cm_mm")
    {
        return std::make_unique<SantimetersToMillimetersConverterLink>();
    }
    else if (t["type"].as_string() == "converter_mm_cm")
    {
        return std::make_unique<MillimetersToSantimetersConverterLink>();
    }
    else if (t["type"].as_string() == "reverse")
    {
        return std::make_unique<ReverseConverterLink>();
    }
    else if (t["type"].as_string() == "rational")
    {
        double num = t["num"].as_numer_except();
        double den = t["den"].as_numer_except();
        return std::make_unique<RationalGearLink>(num, den);
    }
    else if (t["type"].as_string() == "conical")
    {
        double tooth = t["tooth"].as_numer_except();
        double beta = t["beta"].as_numer_except();
        double mod = t["mod"].as_numer_except();
        return std::make_unique<ConicalGearLink>(tooth, mod, beta);
    }
    else if (t["type"].as_string() == "screw")
    {
        double screw_pitch = t["screw_pitch"].as_numer_except();
        return std::make_unique<ScrewGearLink>(screw_pitch);
    }
    else if (t["type"].as_string() == "output")
    {
        double mul;
        if (t.contains("mul"))
            mul = t["mul"].as_numer_except();
        else
        {
            double num = t["num"].as_numer_except();
            double den = t["den"].as_numer_except();
            mul = num / den;
        }
        std::string type = t["type"].as_string_except();
        return std::make_unique<OutputGearLink>(mul, type);
    }
    else if (t["type"].as_string() == "lever")
    {
        double DX = t["DX"].as_numer_except();
        double DY = t["DY"].as_numer_except();
        // double gamma = t["gamma"].as_numer_except() / 180.0 * M_PI;
        double A = t["A"].as_numer_except();
        double B = t["B"].as_numer_except();
        double l_nominal = t["l_nominal"].as_numer_except();
        return std::make_unique<LeverArmGearLink>(DX, DY, A, B, l_nominal);
    }
    else if (t["type"].as_string() == "offset")
    {
        double offset = t["offset"].as_numer_except();
        return std::make_unique<OffsetGearLink>(offset);
    }
    else
    {
        throw std::runtime_error("Unknown gear link type:"s +
                                 t["type"].as_string());
    }
}

std::vector<std::unique_ptr<GearLink>>
GearSettings::parse_gear_link_array(const nos::trent &t)
{
    // std::vector<std::unique_ptr<GearLink>> ret;
    // for (auto &i : t.as_list_except())
    // {
    //     ret.push_back(parse_gear_link(i));
    // }

    auto rng = std::ranges::views::transform(
        t.as_list_except(),
        [](const nos::trent &i) { return parse_gear_link(i); });

    return std::vector<std::unique_ptr<GearLink>>(rng.begin(), rng.end());
}

void GearSettings::parse_auto_mode_trent(const nos::trent &tr)
{
    twoLink = tr["twoLink"].as_bool_except();

    if (tr.contains("beta") && tr["beta"].is_numer())
        beta = tr["beta"].as_numer();

    if (tr.contains("gear1inputRatio") && tr["gear1inputRatio"].is_numer())
        gear1inputRatio = tr["gear1inputRatio"].as_numer();

    if (tr.contains("gear2num") && tr["gear2num"].is_numer())
        gear2num = tr["gear2num"].as_numer();

    if (tr.contains("gear2den") && tr["gear2den"].is_numer())
        gear2den = tr["gear2den"].as_numer();

    if (tr.contains("outputDiam") && tr["outputDiam"].is_numer())
        outputDiam = tr["outputDiam"].as_numer();

    if (tr.contains("outputMul") && tr["outputMul"].is_numer())
        outputMul = tr["outputMul"].as_numer();

    if (tr.contains("mul") && tr["mul"].is_numer())
        mul = tr["mul"].as_numer();
}

void GearSettings::parse_trent(const nos::trent &tr)
{
    if (tr.contains("value"))
    {
        forced_fullcycle_value = tr["value"].as_numer_except();
        return;
    }

    if (tr.contains("declared_rounds_per_unit"))
    {
        declared_rounds_per_unit =
            tr["declared_rounds_per_unit"].as_numer_except();
    }

    // Автоматичекий тип использовался, пока типов было всего два:
    // линейный с рейкой и роторный.
    if (tr.contains("type") && tr["type"].is_string())
        type = tr["type"].as_string_default("auto");

    try
    {
        if (type == "auto")
            parse_auto_mode_trent(tr);
        else if (type == "array")
            links_array = std::move(parse_gear_link_array(tr["links"]));
        else
            throw std::runtime_error("Unknown gear type: " + type);
    }
    catch (const std::exception &e)
    {
        if (declared_rounds_per_unit)
        {
            nos::log::warn(
                "Ошибка при парсинге настроек механической передачи,"
                "но значение установленно непосредственно, поэтому продолжение"
                "работы возможно: {}",
                e.what());
        }
        else
        {
            throw std::runtime_error("Error parsing gear settings: " +
                                     std::string(e.what()));
        }
    }
}

double GearSettings::rounds_per_unit_rounded_onelink() const
{
    return mul * gear1inputRatio / 360.0;
}

double GearSettings::rounds_per_unit_rounded_twolink() const
{
    return mul * gear1inputRatio * gear2num / gear2den / 360.0;
}

double GearSettings::rounds_per_unit_linear_onelink() const
{
    return mul * gear1inputRatio * std::cos((M_PI / 180) * beta) * 10 /
           (M_PI * outputDiam * outputMul);
}

double GearSettings::rounds_per_unit_linear_twolink() const
{
    return mul * gear1inputRatio * gear2num / gear2den *
           std::cos((M_PI / 180) * beta) * 10 / (M_PI * outputDiam * outputMul);
}

double GearSettings::evaluate_rounds_per_unit() const
{
    if (type == "auto")
    {
        if (isLinear)
            if (twoLink)
                return rounds_per_unit_linear_twolink();
            else
                return rounds_per_unit_linear_onelink();
        else if (twoLink)
            return rounds_per_unit_rounded_twolink();
        else
            return rounds_per_unit_rounded_onelink();
    }
    else if (type == "array")
    {
        double ret = 1;
        for (auto &i : links_array)
        {
            // std::visit([&](auto &&v) { ret *= v.gear_ratio(); }, i);
            ret *= i->gear_ratio();
        }
        return ret;
    }
    else
    {
        throw std::runtime_error("Unknown gear type: " + type);
    }
}

double GearSettings::rounds_per_unit() const
{
    if (forced_fullcycle_value != 0)
        return forced_fullcycle_value;

    if (declared_rounds_per_unit != 0)
    {
        return declared_rounds_per_unit;
    }

    auto evaluated = evaluate_rounds_per_unit();
    return evaluated;
}

double GearSettings::units_to_rounds_by_array(double units) const
{
    double ret = units;

    auto rit = links_array.rbegin();
    for (; rit != links_array.rend(); ++rit)
    {
        ret = (*rit)->output_to_input_conversion(ret);
    }
    return ret;
}

double GearSettings::units_to_rounds(double units) const
{
    if (type == "auto")
    {
        return units * rounds_per_unit();
    }
    else if (type == "array")
    {
        return units_to_rounds_by_array(units);
    }
    else
    {
        throw std::runtime_error("Unknown gear type: " + type);
    }
}

double GearSettings::rounds_to_units_by_array(double rounds) const
{
    double ret = rounds;
    assert(!std::isinf(ret));
    assert(!std::isnan(ret));
    // assert(ret != 0);
    for (auto &i : links_array)
    {
        ret = i->input_to_output_conversion(ret);
        assert(!std::isinf(ret));
        assert(!std::isnan(ret));
        // assert(ret != 0);
    }
    return ret;
}

double GearSettings::rounds_to_units(double rounds) const
{
    assert(!std::isinf(rounds));
    assert(!std::isnan(rounds));

    // assert(rounds != 0);

    if (type == "auto")
    {
        auto r = rounds / rounds_per_unit();
        assert(!std::isinf(r));
        assert(!std::isnan(r));
        // assert(r != 0);
        return r;
    }
    else if (type == "array")
    {
        auto r = rounds_to_units_by_array(rounds);
        assert(!std::isinf(r));
        assert(!std::isnan(r));
        // assert(r != 0);
        return r;
    }
    else
    {
        throw std::runtime_error("Unknown gear type: " + type);
    }
}

double LeverArmGearLink::input_to_output_conversion(double input) const
{
    double gamma = atan(B / A);
    double R = sqrt(A * A + B * B);
    auto z = 2 * R * sqrt(S * S + H * H);
    auto b = asin(H / sqrt(S * S + H * H));
    double V = S * S + H * H + R * R;

    auto l = input;
    auto a = (V - l * l) / z;
    auto ret = acos(a) - b + gamma;

    if (std::isnan(ret))
    {
        nos::println("LeverArmGearLink input_to_output_conversion is nan");
        nos::println("l = ", l);
        nos::println("a = ", a);
        nos::println("b = ", b);
        nos::println("ret = ", ret);

        nos::println("S = ", S);
        nos::println("H = ", H);
        nos::println("A = ", A);
        nos::println("B = ", B);
        nos::println("R = ", R);
        nos::println("gamma = ", gamma);

        message_bus_notify(
            "LeverArmGearLink input_to_output_conversion is nan");
        return input;
    }

    return ret;
}

double LeverArmGearLink::output_to_input_conversion(double output) const
{
    double gamma = atan(B / A);
    double R = sqrt(A * A + B * B);
    auto z = 2 * R * sqrt(S * S + H * H);
    auto b = asin(H / sqrt(S * S + H * H));
    double V = S * S + H * H + R * R;

    auto alpha = output;
    return sqrt(V - z * cos(alpha + b - gamma));
}