/*#include <nos/trace.h>
#include <doctest/doctest.h>
#include <nos/io/string_writer.h>

static void bar()
{
	TRACE();
}

static void foo()
{
	TRACE();
	bar();
}

static void bar_a(int a)
{
	TRACE_ARGS(a);
}

static void foo_a(int a)
{
	TRACE_ARGS(a);
	bar_a(a);
}


TEST_CASE("fprint")
{
	std::string output;
	nos::string_writer writer{output};
	nos::current_ostream = &writer;

	SUBCASE("TRACE_test")
	{
		foo();
		CHECK_EQ(output,
		         "TRACE: 1: -> void foo()\r\n"
		         "TRACE: 2: -> void bar()\r\n"
		         "TRACE: 2: <- void bar()\r\n"
		         "TRACE: 1: <- void foo()\r\n");
	}

	SUBCASE("TRACE_ARGS_test")
	{
		foo_a(42);
		CHECK_EQ(output,
		         "TRACE: 1: -> void foo_a(int) args: a:42 \r\n"
		         "TRACE: 2: -> void bar_a(int) args: a:42 \r\n"
		         "TRACE: 2: <- void bar_a(int)\r\n"
		         "TRACE: 1: <- void foo_a(int)\r\n");
	}
}
*/