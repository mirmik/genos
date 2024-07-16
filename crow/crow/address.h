/** @file */

#ifndef CROW_ADDRESS_H
#define CROW_ADDRESS_H

#include <crow/hexer.h>
#include <crow/hostaddr.h>
#include <igris/compiler.h>
#include <nos/buffer.h>
#include <stdint.h>
#include <stdlib.h>

namespace crow
{
    hostaddr crowker_address();
    hostaddr address(const nos::buffer &in);
    hostaddr address_warned(const nos::buffer &in);
}

#endif
