#include <igris/datastruct/argvc.h>

TEST_CASE("ARGVC_test")
{
	char* argv[10];
	memset(argv, 0, sizeof(argv));
	char* str = strdup("one two three 314");
	int argc = argvc_internal_split(str, argv, 10);
	(void) argc;

	CHECK_EQ(std::string(argv[0]), std::string("one"));
	CHECK_EQ(std::string(argv[1]), std::string("two"));
	CHECK_EQ(std::string(argv[2]), std::string("three"));
	CHECK_EQ(std::string(argv[3]), std::string("314"));
	free(str);
}

TEST_CASE("spaces_test")
{
	char* argv[10];
	char* str = strdup("    one two three 314    ");
	int argc = argvc_internal_split(str, argv, 10);
	(void) argc;

	CHECK_EQ(std::string(argv[0]), std::string("one"));
	CHECK_EQ(std::string(argv[1]), std::string("two"));
	CHECK_EQ(std::string(argv[2]), std::string("three"));
	CHECK_EQ(std::string(argv[3]), std::string("314"));
	free(str);
}

TEST_CASE("spaces_test 2")
{
	char* argv[10];
	char* str = strdup("");
	int argc = argvc_internal_split(str, argv, 10);
	free(str);

	CHECK_EQ(argc, 0);
}

TEST_CASE("spaces_test 3")
{
	char* argv[10];
	char* str = strdup(" ");
	int argc = argvc_internal_split(str, argv, 10);
	free(str);

	CHECK_EQ(argc, 0);
}

TEST_CASE("spaces_test 4")
{
	char* argv[10];
	char* str = strdup(" \r \n \t ");
	int argc = argvc_internal_split(str, argv, 10);
	free(str);

	CHECK_EQ(argc, 0);
}
