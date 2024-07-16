#include <algorithm>
#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>

struct KeySource
{
    std::string key;
    std::string source; //< path to source file
};

struct KeyText
{
    std::string key;
    std::string text; // < raw file text
};

struct KeyBytes
{
    std::string key;
    std::string bytes; // < text in C format ( \xAB\x01\x02... )  
};

struct KeyBytesDivided
{
    std::string key;
    std::string bytes_divided; // < text in C format divided to many strings
};

uint8_t HIHALF(uint8_t byte)
{
    return (byte >> 4) & 0x0F;
}

uint8_t LOHALF(uint8_t byte)
{
    return byte & 0x0F;
}

char half2hex(uint8_t n)
{
    return (char)(n < 10 ? '0' + n : 'A' - 10 + n);
}

std::string uint8_to_hex(uint8_t in)
{
    char buf[2];
    char *hex = buf;
    *hex++ = half2hex(HIHALF(in));
    *hex++ = half2hex(LOHALF(in));
    return {buf, 2};
}

std::string trim(const std::string &view)
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

    return {left, ((size_t)(right - left) + 1)};
}

bool is_directory(const std::string &path)
{
    return std::filesystem::is_directory(path);
}

void for_each_directory_file_recursive(
    const std::string &path, std::function<void(const std::string &)> func)
{
    for (auto &p : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(p))
            for_each_directory_file_recursive(p.path().string(), func);
        else
            func(p.path().string());
    }
}

std::vector<KeySource> get_sources_from_file(std::string listfile)
{
    std::vector<KeySource> sources;
    std::ifstream file(listfile);
    std::string line;
    while (std::getline(file, line))
    {
        auto trimmed_line = trim(line);
        if (trimmed_line.size() == 0)
            continue;
        if (trimmed_line[0] == '#')
            continue;

        std::string key = trimmed_line.substr(0, trimmed_line.find(" "));
        std::string source = trimmed_line.substr(trimmed_line.find(" ") + 1);

        if (is_directory(source))
        {
            for_each_directory_file_recursive(
                source,
                [&sources, &source, &key](const std::string &file)
                {
                    auto filepath = std::filesystem::path(file);
                    auto dirpath = std::filesystem::path(source);
                    auto relative_path = filepath.lexically_relative(dirpath);
                    auto join_path = key + std::string(relative_path);
                    sources.push_back(KeySource{join_path, file});
                });
        }
        else
        {
            sources.push_back(KeySource{key, source});
        }
    }
    return sources;
}

size_t check_exists(const std::vector<KeySource> &sources)
{
    size_t errors = 0;
    for (auto source : sources)
    {
        if (!std::ifstream(source.source).good())
        {
            std::cout << "Fatal: Could not open file " << source.source
                      << std::endl;
            errors++;
        }
    }
    return errors;
}

void sort_sources(std::vector<KeySource> &sources)
{
    std::sort(sources.begin(),
              sources.end(),
              [](const KeySource &a, const KeySource &b)
              { return a.key < b.key; });
}

KeyText keysource_to_keytext(KeySource source)
{
    char buf[1024];
    std::ifstream file(source.source);
    std::string text;
    int readed;
    while ((readed = file.readsome(buf, sizeof(buf))) > 0)
    {
        text.append(buf, readed);
    }
    return KeyText{source.key, text};
}

std::vector<KeyText> keysources_to_keytexts(std::vector<KeySource> sources)
{
    std::vector<KeyText> keytexts;
    std::transform(sources.begin(),
                   sources.end(),
                   std::back_inserter(keytexts),
                   keysource_to_keytext);
    return keytexts;
}

KeyBytes keytext_to_keybytes(KeyText text)
{
    std::string bytes;
    for (char c : text.text)
    {
        bytes += "\\x" + uint8_to_hex(c);
    }
    return KeyBytes{text.key, bytes};
}

std::vector<KeyBytes> keytexts_to_keybytes(std::vector<KeyText> keytexts)
{
    std::vector<KeyBytes> keybytes;
    std::transform(keytexts.begin(),
                   keytexts.end(),
                   std::back_inserter(keybytes),
                   keytext_to_keybytes);
    return keybytes;
}

KeyBytesDivided keybytes_to_keybytesdivided(KeyBytes keybytes, int tabs)
{
    size_t size = keybytes.bytes.size();
    size_t writed = 0;
    std::string compiled;

    if (size == 0)
    {
        for (int i = 0; i < tabs; i++)
            compiled += "\t";
        compiled += "\"\"\n";
        return KeyBytesDivided{keybytes.key, compiled};
    }

    while (size - writed != 0)
    {
        size_t writable = size - writed > 18 * 4 ? 18 * 4 : size - writed;
        for (int i = 0; i < tabs; i++)
            compiled += "\t";
        compiled += "\"";
        compiled += keybytes.bytes.substr(writed, writable) + "\"";
        writed += writable;
        if (size - writed != 0)
            compiled += "\n";
    }
    return KeyBytesDivided{keybytes.key, compiled};
}

std::string compile_headers(bool cpp_enabled)
{
    std::string headers;
    if (cpp_enabled)
    {
        headers += "#include <string>\n";
        headers += "#include <vector>\n";
        headers += "#include <utility>\n";
    }
    headers += "#include <string.h>\n";
    headers += "#include <stdlib.h>\n";
    return headers;
}

std::string compile_ircc_resources_consts(std::vector<KeyBytes> keybytes)
{
    std::string compiled;
    for (int i = 0; i < keybytes.size(); i++)
    {
        compiled +=
            "const char* const IRCC_RESOURCES_" + std::to_string(i) + " = \n";
        compiled += keybytes_to_keybytesdivided(keybytes[i], 2).bytes_divided;
        compiled += ";\n\n";
    }
    return compiled;
}

std::string compile_ircc_resources_map_cstyle(std::vector<KeyBytes> keybytes)
{

    std::string compiled = "";
    compiled += "struct key_value_size IRCC_RESOURCES_[] = {\n";
    for (size_t i = 0; i < keybytes.size(); ++i)
    {
        compiled += "\t{\"" + keybytes[i].key + "\", ";
        compiled += "IRCC_RESOURCES_" + std::to_string(i);
        compiled += ", ";
        compiled += std::to_string(keybytes[i].bytes.size() / 4);
        compiled += "},\n";
    }
    compiled += "\t{NULL, NULL, 0}};\n";
    return compiled;
}

std::string text_struct_key_value_size()
{
    return R"(struct key_value_size
{
    const char *key;
    const char *value;
    size_t size;
};
)";
}

std::string text_binary_search_function()
{
    return R"(struct key_value_size *ircc_binary_search(const char *key)
{
    int low = 0;
    int high = sizeof(IRCC_RESOURCES_) / sizeof(IRCC_RESOURCES_[0]) - 1;
    int mid;
    while (low <= high)
    {
        mid = (low + high) / 2;
        int cmp = strcmp(key, IRCC_RESOURCES_[mid].key);
        if (cmp < 0)
            high = mid - 1;
        else if (cmp > 0)
            low = mid + 1;
        else
            return &IRCC_RESOURCES_[mid];
    }
    return NULL;
}
)";
}

std::string text_c_functions()
{
    return R"(#ifdef __cplusplus
extern "C" const char *ircc_c_string(const char *key, size_t *sizeptr);
#endif
const char *ircc_c_string(const char *key, size_t *sizeptr)
{
    struct key_value_size *kvs = ircc_binary_search(key);
    if (kvs == NULL)
        return NULL;
    if (sizeptr != NULL)
        *sizeptr = kvs->size;
    return kvs->value;
}

#ifdef __cplusplus
extern "C" const char *ircc_name_by_no(size_t no);
#endif
const char *ircc_name_by_no(size_t no)
{
    return IRCC_RESOURCES_[no].key;
}
)";
}

std::string text_cxx_functions()
{
    return R"(std::string ircc_string(const std::string& key)
{
    struct key_value_size *kvs = ircc_binary_search(key.c_str());
    if (kvs == NULL)
        return {};
    return std::string(kvs->value, kvs->size);
}

std::vector<uint8_t> ircc_vector(const std::string& key)
{
    struct key_value_size *kvs = ircc_binary_search(key.c_str());
    if (kvs == NULL)
        return {};
    return std::vector<uint8_t>((const uint8_t*)kvs->value, 
                (const uint8_t*)(kvs->value + kvs->size));
}

std::pair<const char*, size_t> ircc_pair(const std::string& key)
{
    struct key_value_size *kvs = ircc_binary_search(key.c_str());
    if (kvs == NULL)
        return {};
    return std::pair<const char*, size_t>(kvs->value, kvs->size);
}

std::vector<std::string> ircc_keys()
{
    std::vector<std::string> list;
    for (int i = 0; IRCC_RESOURCES_[i].key != NULL; i++)
    {
        list.push_back(IRCC_RESOURCES_[i].key);
    }
    return list;
}
)";
}

bool is_rebuild_needed(std::vector<KeySource> keysources, std::string outfile)
{
    if (!std::filesystem::exists(outfile))
        return true;
    if (keysources.size() == 0)
        return false;
    /// outfile modification time
    std::filesystem::file_time_type outfile_mtime =
        std::filesystem::last_write_time(outfile);

    for (auto keysource : keysources)
    {
        if (std::filesystem::last_write_time(keysource.source) > outfile_mtime)
            return true;
    }
    return false;
}

void print_help()
{
    std::cout << "Usage: ircc [options] [file]\n";
    std::cout << "Options:\n";
    std::cout << "\t-h, --help\tShow this help\n";
    std::cout << "\t-c, --c_only\tMake C file instead C++\n";
    std::cout << "For build systems compatible:\n";
    std::cout << "\t-s, --sources\tprint list of resourse pathes\n";
    std::cout << "\t-S, --sources-cmake\tprint list of resourse pathes in "
                 "cmake compatible format\n";
    std::cout << "\t-k, --keys\tprint list of keys\n";
    std::cout << "\t-n, --is-rebuild-needed\tprint yes if rebuild needed. "
                 "Otherwise print no.\n";
}

int main(int argc, char **argv)
{
    bool CPP_ENABLED = true;
    bool PRINT_KEYS_MODE = false;
    bool PRINT_SOURCES_MODE = false;
    bool PRINT_SOURCES_CMAKE_MODE = false;
    bool IS_REBUILD_NEEDED_MODE = false;
    std::string OUTFILE = {};

    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"c_only", no_argument, NULL, 'c'},
        {"output", required_argument, NULL, 'o'},
        {"sources", no_argument, NULL, 's'},
        {"sources-cmake", no_argument, NULL, 'S'},
        {"is-rebuild-needed", no_argument, NULL, 'n'},
        {"keys", no_argument, NULL, 'k'},
    };

    int long_index = 0;
    int opt = 0;

    while ((opt = getopt_long(
                argc, argv, "hco:ksS", long_options, &long_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);

        case 'c':
            CPP_ENABLED = false;
            break;

        case 'o':
            OUTFILE = optarg;
            break;

        case 'k':
            PRINT_KEYS_MODE = true;
            break;

        case 's':
            PRINT_SOURCES_MODE = true;
            break;

        case 'S':
            PRINT_SOURCES_CMAKE_MODE = true;
            break;

        case 'n':
            IS_REBUILD_NEEDED_MODE = true;
            break;

        case '?':
            exit(-1);
            break;

        case 0:
            std::cout << "getopt error\n";
            exit(-1);
            break;
        }
    };

    if (optind >= argc)
    {
        std::cout << "No input file\n";
        print_help();
        exit(-1);
    }

    if (OUTFILE.empty())
    {
        std::cout << "Output file is needed. Use -o option.\n";
        print_help();
        exit(-1);
    }

    std::string listfile = argv[optind];
    auto sources = get_sources_from_file(listfile);

    int errors = check_exists(sources);
    if (errors > 0)
    {
        std::cout << "Fatal: " << errors << " errors occurred." << std::endl;
        exit(1);
    }

    sort_sources(sources);

    if (IS_REBUILD_NEEDED_MODE)
    {
        if (is_rebuild_needed(sources, OUTFILE))
            std::cout << "yes" << std::endl;
        else
            std::cout << "no" << std::endl;
        exit(0);
    }

    if (PRINT_SOURCES_MODE)
    {
        for (auto &source : sources)
        {
            std::cout << source.source << "\n";
        }
        exit(0);
    }

    if (PRINT_SOURCES_CMAKE_MODE)
    {
        for (auto &source : sources)
        {
            std::cout << source.source << ";";
        }
        exit(0);
    }

    if (PRINT_KEYS_MODE)
    {
        for (auto &source : sources)
        {
            std::cout << source.key << "\n";
        }
        exit(0);
    }

    auto texts = keysources_to_keytexts(sources);
    auto keybytes = keytexts_to_keybytes(texts);
    std::ofstream out(OUTFILE);
    out << compile_headers(CPP_ENABLED);
    out << "\n";
    out << compile_ircc_resources_consts(keybytes);
    out << text_struct_key_value_size();
    out << "\n";
    out << compile_ircc_resources_map_cstyle(keybytes);
    out << "\n";
    out << text_binary_search_function();
    out << "\n";
    out << text_c_functions();

    if (CPP_ENABLED)
    {
        out << "\n";
        out << text_cxx_functions();
    }
    out.close();
    return 0;
}
