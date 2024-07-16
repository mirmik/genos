/**
    @file
*/

#ifndef CROW_SUPPORT_H
#define CROW_SUPPORT_H

#include <crow/address.h>
#include <stdint.h>
#include <vector>

extern crow::hostaddr crowaddr;

int crow_configure(int crowport);

#endif