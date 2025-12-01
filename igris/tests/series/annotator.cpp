#include <doctest/doctest.h>
#include <igris/series/field_annotator.h>

TEST_CASE("field_annotation")
{
    igris::series_field_annotator annotator;
    auto annot0 = annotator.add<bool>("lanspik", "Ланспик");
    auto annot1 = annotator.add<int>("lanspik2", "Ланспик2");

    CHECK_EQ(annotator.annotations()->size(),
             annotator.annotations_dict()->size());
}