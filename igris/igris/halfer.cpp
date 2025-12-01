#include <igris/halfer.h>

void igris::connect_halfers(halfer *a, halfer *b)
{
    a->opposite = b;
    b->opposite = a;
}

igris::halfer::~halfer()
{
    if (opposite)
        opposite->opposite = nullptr;
}

bool igris::halfer::is_alive() { return opposite != nullptr; }
