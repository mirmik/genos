#pragma once

#include <string>
#include <vector>
#include <map>

#include <igris/buffer.h>

void flt32_conv(const std::string &str, void *tgt);
void int32_conv(const std::string &str, void *tgt);

void binin_mode_prepare(std::string infmt);

std::string input_binary(const std::string& fmt);