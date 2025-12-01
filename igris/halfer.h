#ifndef IGRIS_HALFER_H
#define IGRIS_HALFER_H

namespace igris
{
    class halfer
    {
        halfer *opposite = nullptr;
        ~halfer();

    public:
        bool is_alive();

        friend void connect_halfers(halfer *a, halfer *b);
    };

    void connect_halfers(halfer *a, halfer *b);
}

#endif
