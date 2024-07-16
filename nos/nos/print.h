/** @file */

#ifndef NOS_PRINT_H
#define NOS_PRINT_H

#include <nos/io/ostream.h>
#include <nos/print/print.h>
#include <nos/print/stdtype.h>

#define PRINT(arg) nos::println(#arg ":", arg)
#define PRINTTO(os, arg) nos::println_to(os, #arg ":", arg)

#endif
