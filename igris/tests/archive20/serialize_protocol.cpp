#include <doctest/doctest.h>
#include <igris/serialize/serialize_protocol.h>
#include <igris/serialize/serialize_storage.h>
#include <igris/serialize/serialize_archive.h>

TEST_CASE("serialize_protocol") 
{
	uint64_t data;
	std::string dump;

	dump = igris::serialize((uint8_t) 32);
	CHECK_EQ(dump.size(), 1);
	data = igris::deserialize<uint8_t>(dump);
	CHECK_EQ(data, 32);

	dump = igris::serialize((uint16_t) 33);
	CHECK_EQ(dump.size(), 2);
	data = igris::deserialize<uint16_t>(dump);
	CHECK_EQ(data, 33);

	dump = igris::serialize((uint32_t) 34);
	CHECK_EQ(dump.size(), 4);
	data = igris::deserialize<uint32_t>(dump);
	CHECK_EQ(data, 34);

	dump = igris::serialize((uint64_t) 35);
	CHECK_EQ(dump.size(), 8);
	data = igris::deserialize<uint64_t>(dump);
	CHECK_EQ(data, 35);
}

TEST_CASE("serialize_protocol.list") 
{
	std::vector<uint32_t> data = {31, 32, 33, 34}, rdata;
	std::string dump;

	dump = igris::serialize(data);
	CHECK_EQ(dump.size(), 18);		

	rdata = igris::deserialize<std::vector<uint32_t>>(dump);

	CHECK_EQ(rdata.size(), 4);
	CHECK_EQ(rdata[0], 31);
	CHECK_EQ(rdata[1], 32);
	CHECK_EQ(rdata[2], 33);
	CHECK_EQ(rdata[3], 34);
}