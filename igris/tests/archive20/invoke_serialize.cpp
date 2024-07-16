#include <doctest/doctest.h>
#include <igris/protocols/invoke_serializer.h>
#include <igris/serialize/serialize.h>
#include <igris/serialize/archive.h>
#include <igris/serialize/serialize_storage.h>

TEST_CASE("invoke_serialize") 
{
    std::string str;
    igris::archive::binary_string_writer wr(str);
    //serialize_invoke(wr, );
}