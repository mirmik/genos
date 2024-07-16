/** @file */

#ifndef CROW_ITER_H
#define CROW_ITER_H

#include <crow/gateway.h>
#include <crow/proto/node.h>

#include <vector>

namespace crow
{
    std::vector<crow::gateway *> gates();
    std::vector<crow::node *> nodes();
}

#endif
