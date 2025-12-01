#include <doctest/doctest.h>
#include <igris/series/field_annotator.h>

TEST_CASE("field_annotation")
{
    igris::series_field_annotator annotator;
    auto annot0 = annotator.add<bool>("lanspik", "Ланспик");
}
