#include <doctest/doctest.h>
#include <genos/vterm.h>

#include <string>

static char buffer[48];
static char history[48 * 5];

std::string output;

static int a = 0;

static void execute_callback(void * priv, const char * data, unsigned int size)
{
	a++;
	CHECK_EQ(std::string(data, size), "hello");
}

static void write_callback(void * priv, const char * data, unsigned int size)
{
	output += std::string(data, size);
}

TEST_CASE("vterm")
{
	struct vterm_automate vterm;
	vterm_automate_init(&vterm, buffer, 48, history, 5);

	vterm_set_write_callback(&vterm, write_callback, NULL);
	vterm_set_execute_callback(&vterm, execute_callback, NULL);

	output.clear();
	vterm_automate_init_step(&vterm);

	CHECK_EQ(output, "$ ");
	output.clear();

	for (char c : std::string("hello"))
	{
		vterm_automate_newdata(&vterm, c);
	}

	CHECK_EQ(output.size(), 5);
	CHECK_EQ(output, "hello");

	vterm_automate_newdata(&vterm, '\r');
	vterm_automate_newdata(&vterm, '\n');

	CHECK_EQ(a, 1);

	for (char c : std::string("hello\r\n"))
	{
		vterm_automate_newdata(&vterm, c);
	}

	CHECK_EQ(a, 2);
}