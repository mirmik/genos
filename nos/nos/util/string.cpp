#include <nos/util/hexascii.h>
#include <nos/util/string.h>

std::string nos::trim(const std::string_view &view)
{
    if (view.size() == 0)
        return "";

    const char *left = view.data();
    const char *right = view.data() + view.size() - 1;
    const char *end = view.data() + view.size();

    while (left != end &&
           (*left == ' ' || *left == '\n' || *left == '\r' || *left == '\t'))
        ++left;

    if (left == end)
        return "";

    while (left != right && (*right == ' ' || *right == '\n' ||
                             *right == '\r' || *right == '\t'))
        --right;

    return std::string(left, (right - left) + 1);
}

std::vector<std::string> nos::split(const std::string_view &str, char delim)
{
    std::vector<std::string> outvec;

    char *strt;
    char *ptr = (char *)str.data();
    char *end = (char *)str.data() + str.size();

    while (true)
    {
        while (ptr != end && *ptr == delim)
            ptr++;

        if (ptr == end)
            break;

        strt = ptr;

        while (ptr != end && *ptr != delim)
            ptr++;

        outvec.emplace_back(strt, ptr - strt);
    }

    return outvec;
}

std::vector<std::string> nos::split(const std::string_view &str,
                                    const char *delims)
{
    std::vector<std::string> outvec;

    if (str.size() == 0)
        return outvec;

    char *strt;
    char *ptr = (char *)str.data();
    char *end = (char *)str.data() + str.size();

    while (true)
    {
        // Skip delimiters
        while (ptr != end && strchr(delims, *ptr) != NULL)
            ptr++;

        if (ptr == end)
            break;

        strt = ptr;

        while (ptr != end && strchr(delims, *ptr) == NULL)
            ptr++;

        outvec.emplace_back(strt, ptr - strt);
        if (ptr == end)
            break;
    }

    return outvec;
}

std::string nos::join(const std::vector<std::string> &vec, char delim)
{
    if (vec.size() == 0)
    {
        return "";
    }

    std::string ret;

    size_t len = 0;

    for (auto &s : vec)
    {
        len++;
        len += s.size();
    }

    ret.reserve(len);

    auto preend = vec.end();
    auto iter = vec.begin();
    preend--;

    for (; iter != preend; iter++)
    {
        ret.append(*iter);
        ret.push_back(delim);
    }

    ret.append(*iter);

    return ret;
}

std::string nos::random_string(int len, uint64_t seed)
{
    std::string tmp_s;
    static const char alphanum[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";

    std::linear_congruential_engine<uint64_t,
                                    6364136223846793005,
                                    1442695040888963407,
                                    0>
        rand(seed);
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

    return tmp_s;
}