/** @file */

#ifndef CROW_HEXER_H
#define CROW_HEXER_H

#include <igris/compiler.h>
#include <stdint.h>
#include <stdlib.h>

#define CROW_HEXER_UNDEFINED_SYMBOL -1
#define CROW_HEXER_MORE3_DOT -4
#define CROW_HEXER_ODD_GRID -5

/// Утилита для удобной записи хекс адреса.
/// Пример: .12.127.0.0.1#2714  -> 0C7F0000012714
/// Пример: .12.127.0.0.1:10004 -> 0C7F0000012714
/// Пример: .12.127:.1:10004 -> 0C7F0000012714
/// Пример: .12#7F000001:10004 -> 0C7F0000012714
/// Пример: #0C#7F000001#2714 -> 0C7F0000012714
/// Пример: #0C7F0000012714 -> 0C7F0000012714
int hexer(uint8_t *dst, size_t maxsz, const char *src, size_t srcsz);
int hexer_s(uint8_t *dst, size_t maxsz, const char *src);

#endif
