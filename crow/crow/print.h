/** @file */

#ifndef CROW_PRINT_H
#define CROW_PRINT_H

#include <crow/packet.h>

namespace crow
{
    void diagnostic(const char *notation, crow::packet *pack);
}

#endif
