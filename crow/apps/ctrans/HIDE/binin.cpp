#include "binin.h"
#include "bincommon.h"

#include <igris/util/string.h>
#include <igris/dprint.h>

void flt32_conv(const std::string &str, void *tgt)
{
	*(float *)tgt = (float)atof(str.c_str());
}

void int64_conv(const std::string &str, void *tgt) { *(int64_t *)tgt = atoi(str.c_str()); }
void int32_conv(const std::string &str, void *tgt) { *(int32_t *)tgt = atoi(str.c_str()); }
void int16_conv(const std::string &str, void *tgt) { *(int16_t *)tgt = atoi(str.c_str()); }
void int8_conv (const std::string &str, void *tgt) { *(int8_t *)tgt =  atoi(str.c_str()); }

size_t binin_size = 0;
std::vector<size_t> binin_sizes;
std::vector<void(*)(const std::string &, void *)> binin_conves;
std::vector<std::string> binin_format;

std::map<std::string, void(*)(const std::string &, void *)> visitor_conv =
{
	{"f32", flt32_conv}, 
	{"i64", int64_conv},
	{"i32", int32_conv},
	{"i16", int16_conv},
	{"i8",  int8_conv}
};

void binin_mode_prepare(std::string infmt) 
{
	binin_format = igris::split(infmt, ',');

	for (auto &str : binin_format)
	{
		binin_size += visitor_size[str];
		binin_sizes.push_back(visitor_size[str]);
		binin_conves.push_back(visitor_conv[str]);
	}
}

std::string input_binary(const std::string& in) 
{
	std::string ret;
	igris::strvec args = igris::split(in, ',');

	if (args.size() != binin_format.size())  
	{
		dprln("WARN:", "wrong binary input");
		return std::string();
	}

	ret.resize(binin_size);
	char * ptr = (char*)ret.data();

	for (unsigned int i = 0; i < args.size(); ++i)
	{
		binin_conves[i](args[i], ptr);
		ptr += binin_sizes[i];
	}

	return ret;
}