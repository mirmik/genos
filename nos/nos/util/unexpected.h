#ifndef NOS_UTIL_UNEXPECTED_H
#define NOS_UTIL_UNEXPECTED_H

namespace nos 
{
	template <class Err>
	class unexpected 
	{
		Err _error;

	public:
		Err error() const { return _error; }
		unexpected(const Err& err) : _error(err) {}
	};
}

#endif