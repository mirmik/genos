/**
    @file
*/

#include <MitsuAdjustment.h>
#include <nos/fprint.h>

using namespace nos::argument_literal;

MitsuAdjustment::MitsuAdjustment() : type(1), table_number(16), inertia_rate(7)
{
}
MitsuAdjustment::MitsuAdjustment(const MitsuAdjustment &oth) :
    type(oth.type),
    table_number(oth.table_number),
    inertia_rate(oth.inertia_rate)
{
}

MitsuAdjustment &MitsuAdjustment::operator=(const MitsuAdjustment &oth)
{
    type = oth.type;
    table_number = oth.table_number;
    inertia_rate = oth.inertia_rate;
    return *this;
}

MitsuAdjustment::MitsuAdjustment(const nos::trent &tr)
{
    type = tr["type"].as_numer();

    switch (type)
    {
    case 1:
        table_number = tr["table_number"].as_integer_except();
        break;
    case 2:
        table_number = tr["table_number"].as_integer_except();
        inertia_rate = tr["inertia_rate"].as_numer_except();
        break;
    default:
        throw std::runtime_error("Unknown servo type");
        break;
    }
}

nos::expected<size_t, nos::output_error>
MitsuAdjustment::print_to(nos::ostream &o) const
{
    return nos::fprint_to(o,
                          "type: {type}, inertia_rate: {inertiarate}, "
                          "table_number: {tablenumber}",
                          "type"_a = type,
                          "inertiarate"_a = inertia_rate,
                          "tablenumber"_a = table_number);
}
