#include <nos/util/error.h>
#include <exception>

namespace nos
{
	class exception : std::exception 
	{
		std::string str;
	public:
		exception(const char* msg) : str(msg) {}
		const char* what() const noexcept override { return str.c_str(); }
	};
}

void nos::error(const char * msg) 
{
	throw nos::exception(msg);
}
