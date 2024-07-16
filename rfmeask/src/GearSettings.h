/**
    @file
*/

#ifndef GEAR_SETTINGS_H
#define GEAR_SETTINGS_H

#include <include_helper.h>
#include <memory>
#include <nos/trent/trent.h>
#include <variant>
#include <vector>

class GearLink
{
public:
    virtual double gear_ratio() const
    {
        throw std::runtime_error("Not implemented");
    }

    virtual double input_to_output_conversion(double input) const
    {
        return input / gear_ratio();
    }

    virtual double output_to_input_conversion(double output) const
    {
        return output * gear_ratio();
    }
};

// Обычный редуктор.
class ReductorGearLink : public GearLink
{
    double num;

public:
    ReductorGearLink(double num) : num(num) {}
    double gear_ratio() const override
    {
        return num;
    }
};

class RoundsToDegreeConverterLink : public GearLink
{
public:
    RoundsToDegreeConverterLink() {}
    double gear_ratio() const override
    {
        return 1.0 / 360.0;
    }
};

class DegreeToRoundsConverterLink : public GearLink
{
public:
    DegreeToRoundsConverterLink() {}
    double gear_ratio() const override
    {
        return 360;
    }
};

class RadiansToDegreeConverterLink : public GearLink
{
public:
    RadiansToDegreeConverterLink() {}
    double gear_ratio() const override
    {
        return M_PI / 180.0;
    }
};

class DegreeToRadiansConverterLink : public GearLink
{
public:
    DegreeToRadiansConverterLink() {}
    double gear_ratio() const override
    {
        return 180.0 / M_PI;
    }
};

class ReverseConverterLink : public GearLink
{
public:
    ReverseConverterLink() {}
    double gear_ratio() const override
    {
        return -1.0;
    }
};

class SantimetersToMillimetersConverterLink : public GearLink
{
public:
    SantimetersToMillimetersConverterLink() {}
    double gear_ratio() const override
    {
        return 1.0 / 10.0;
    }
};

class MillimetersToSantimetersConverterLink : public GearLink
{
public:
    MillimetersToSantimetersConverterLink() {}
    double gear_ratio() const override
    {
        return 10;
    }
};

// Может быть использован для планетарной передачи.
class RationalGearLink : public GearLink
{
    double num; // Числитель.
    double den; // Знаменатель.

public:
    RationalGearLink(double num, double den) : num(num), den(den) {}
    double gear_ratio() const override
    {
        return num / den;
    }
};

// Класс описывает передаточные характеристики конического
// реечного зацепления.
class ConicalGearLink : public GearLink
{
    double tooth; // делительный диаметр в миллиметрах
    double mod;  // модуль зуба в миллиметрах
    double beta; // угол наклона зубьев в градусах

public:
    ConicalGearLink(double tooth, double mod, double beta) :
        tooth(tooth), mod(mod), beta(beta)
    {
    }
    double gear_ratio() const override
    {
        return std::cos((M_PI / 180) * beta) / (M_PI * tooth * mod);
    }
};

// Класс описывает винтовую передачу.
class ScrewGearLink : public GearLink
{
    double screw_pitch; // шаг винта

public:
    ScrewGearLink(double screw_pitch) : screw_pitch(screw_pitch) {}
    double gear_ratio() const override
    {
        return 1 / screw_pitch;
    }
};

// Класс описывает передаточные выходного множителя.
// Обычно 1/360 для вращательных механизмов.
// и 10/1 для линейных механизмов.
class OutputGearLink : public GearLink
{
    double multiplier; // множитель
    std::string type;

public:
    OutputGearLink(double multiplier, std::string type) :
        multiplier(multiplier), type(type)
    {
    }
    double gear_ratio() const override
    {
        return multiplier;
    }
    bool is_linear()
    {
        return type == "linear";
    }
};

class LeverArmGearLink : public GearLink
{
    double S;
    double H;
    double A;
    double B;
    double l_nominal;

public:
    LeverArmGearLink(double S, double H, double A, double B, double l_nominal) :
        S(S), H(H), A(A), B(B), l_nominal(l_nominal)
    {
    }

    double gear_ratio() const override
    {
        double gear = input_to_output_conversion(l_nominal + 1) -
                      input_to_output_conversion(l_nominal);
        return 1 / gear;
    }

    double input_to_output_conversion(double input) const override;
    double output_to_input_conversion(double output) const override;
};

class OffsetGearLink : public GearLink
{
    double offset;

public:
    OffsetGearLink(double offset) : offset(offset) {}

    double gear_ratio() const override
    {
        return 1;
    }

    double input_to_output_conversion(double input) const override
    {
        return input + offset;
    }
    double output_to_input_conversion(double output) const override
    {
        return output - offset;
    }
};

// using GearLinkVariant = std::variant<ReductorGearLink,
//                                      RationalGearLink,
//                                      ConicalGearLink,
//                                      ScrewGearLink,
//                                      OutputGearLink>;

/**
    Класс расчитывает механические параметры передачи, преводя к единицам
   round per unit.

    В замкнутом контуре эта характеристика не учитывается в unitRatio, но
   фигурирует в расчёте скорости и в load gear. Для сервоусилителя round
   легко пересчитывается в drive путём умножения на impulses_per_round.
*/
class GearSettings
{
private:
    std::string type = "auto"; ///< Тип передачи: "auto", "array".

    // Параметры для типа "auto".
    double mul = 1;
    double gear1inputRatio = 1;
    double gear2num = 1;
    double gear2den = 1;
    double outputDiam = 1; // делительный диаметр.
    double outputMul = 1;  // модуль зуба или наоборот.
    double beta = 19.5283;
    bool twoLink = false;
    bool isLinear = false;

    // Альтернативный способ описания механической передачи.
    // Используется в типе "array".
    std::vector<std::unique_ptr<GearLink>> links_array;

    /**
        следующие два поля используются для установки значения передачи в
        обход расчёта. forced_fullcycle_value приоритетнее и покрывает
        механическую передачу и электронную передачу сервоусилителя.
        declared_rounds_per_unit - только механическую.
        Если declared_rounds_per_unit установлен, расчёт имеет
        справочное значение.
        Использование forced_fullcycle_value не рекомендуется и
        оставлено для обратной совместимости.
    */
    double forced_fullcycle_value = 0;
    double declared_rounds_per_unit = 0;

public:
    GearSettings() = default;
    GearSettings(const GearSettings &) = delete;
    GearSettings(GearSettings &&) = default;
    GearSettings &operator=(const GearSettings &) = delete;
    GearSettings &operator=(GearSettings &&) = default;

    int print_to(nos::ostream &os) const;

    // Вынесено в отдельную функцию, так как заполняется из типа оси.
    // TODO: выглядит как баг интерфейса.
    void set_is_linear(bool isLinear);

    // Десериализует себя из объекта trent.
    void parse_trent(const nos::trent &tr);

    // Возвращает расчитанное значение.
    double rounds_per_unit() const;

    // Возвращает значение, заданное в конфиге.
    // Обратная совместимость со старым стилем
    // Ранее предполагалось, что unitRatio учитывает
    // полное отношение unit к управляющим пульсам.
    // Позже unitRatio был переопределён через gearSettings
    // и стал учитывать только механическую передачу.
    double get_forced_fullcycle_value() const
    {
        return forced_fullcycle_value;
    }

    void set_forced_fullcycle_value(double value)
    {
        forced_fullcycle_value = value;
    }

    bool has_setted_value() const
    {
        return forced_fullcycle_value != 0;
    }

    static std::vector<std::unique_ptr<GearLink>>
    parse_gear_link_array(const nos::trent &t);
    static std::unique_ptr<GearLink> parse_gear_link(const nos::trent &t);

    std::string info() const;

public:
    double rounds_per_unit_rounded_onelink() const;
    double rounds_per_unit_rounded_twolink() const;
    double rounds_per_unit_linear_onelink() const;
    double rounds_per_unit_linear_twolink() const;

    double units_to_rounds_by_array(double units) const;
    double units_to_rounds(double units) const;

    double rounds_to_units_by_array(double rounds) const;
    double rounds_to_units(double rounds) const;

private:
    double evaluate_rounds_per_unit() const;
    void parse_auto_mode_trent(const nos::trent &tr);
};

#endif