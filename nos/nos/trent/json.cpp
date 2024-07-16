#include <fstream>
#include <nos/trent/json.h>

nos::trent nos::json::parse(const char *str)
{
    parser_cstr parser(str);
    return parser.parse();
}

nos::trent nos::json::parse(const std::string &str)
{
    parser_str parser(str);
    return parser.parse();
}

nos::trent nos::json::parse_file(const std::string &str)
{
    std::ifstream t(str);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    return parse(buffer);
}
