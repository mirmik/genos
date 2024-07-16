#pragma once

#include <igris/buffer.h>
#include <string>

#include <crow/packet.h>

std::string flt32_output(void *tgt);
std::string int32_output(void *tgt);

void binout_mode_prepare(const std::string &fmt);
void output_binary(nos::buffer data, crow::packet *pack);