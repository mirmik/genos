#include <nos/io/stdfile.h>

namespace nos {
	nos::ostream * current_ostream = &nos::cout;
	nos::istream * current_istream = &nos::cin;
}
