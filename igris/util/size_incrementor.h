#ifndef IGRIS_SIZE_INCREMENTOR_H
#define IGRIS_SIZE_INCREMENTOR_H

namespace igris
{
    class size_incrementor
    {
        int value = 0;

    public:
        size_incrementor() {}
        size_incrementor(int size) : value(size) {}

        template <class T> int increment()
        {
            return increment(sizeof(T));
        }

        int increment(int size)
        {
            int ret = value;
            value += size;
            return ret;
        }

        int increment(size_t size)
        {
            int ret = value;
            value += static_cast<int>(size);
            return ret;
        }
    };
}

#endif
