#include <nos/trent/trent.h>

const char *nos::typestr(nos::trent_type m_type)
{
    switch (m_type)
    {
    case nos::trent_type::string:
        return "str";

    case nos::trent_type::list:
        return "list";

    case nos::trent_type::dict:
        return "dict";

    case nos::trent_type::numer:
        return "num";

    case nos::trent_type::boolean:
        return "bool";

    case nos::trent_type::nil:
        return "nil";
    }

    return "";
}
