#ifndef CROW_ADDONS_NOSPUBLISHER_H
#define CROW_ADDONS_NOSPUBLISHER_H

#include <nos/io/ostream.h>

namespace crow
{
    class nospublisher : public nos::ostream
    {
        uint8_t *addr;
        uint8_t alen;
        const char *theme;

    public:
        void init(uint8_t *addr, uint8_t alen, const char *theme)
        {
            this->addr = addr;
            this->alen = alen;
            this->theme = theme;
        }

    public:
        ssize_t write(const void *ptr, size_t sz) override
        {
            crow::publish(addr, alen, theme, ptr, sz, 0, 200);
        }
    };

    class nospublisher_buffered : public nos::ostream
    {
        uint8_t *addr;
        uint8_t alen;
        const char *theme;

        char buffer[64];
        int cursor = 0;

    public:
        void init(uint8_t *addr, uint8_t alen, const char *theme)
        {
            this->addr = addr;
            this->alen = alen;
            this->theme = theme;
        }

    public:
        ssize_t write(const void *ptr, size_t sz) override
        {
            const char *cptr = (const char *)ptr;
            while (sz--)
            {
                char c = *cptr++;

                buffer[cursor++] = c;

                if (c == '\n')
                {
                    crow::publish(addr, alen, theme, buffer, cursor, 0, 200);
                    cursor = 0;
                    memset(buffer, 0, 64);
                }
            }
        }
    };

} // namespace crow

#endif
