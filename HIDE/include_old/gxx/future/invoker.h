#ifndef GXX_INVOKER_H
#define GXX_INVOKER_H

//#include <tuple>
#include <gxx/future/hexarchive.h>
#include <gxx/future/nullarchive.h>

namespace gxx {

	/*template<typename Out, typename In>
	using invoke_func_t = uint8_t (*) (Out&, In&);

	template<typename OutputArchive, typename InputArchive>
	class ArchiveInvokerBasic {
	public:
		void *func;
		ArchiveInvokerBasic(void* _f) : func(_f) {};

		virtual uint8_t invoke(OutputArchive&, InputArchive&) = 0;
	};

	template <typename Ret, typename Arg, typename OutputArchive, typename InputArchive>
	class ArchiveInvoker : public ArchiveInvokerBasic<OutputArchive, InputArchive> {
		using Parent = ArchiveInvokerBasic<OutputArchive, InputArchive>;
	public:
		ArchiveInvoker(void* _f) :ArchiveInvokerBasic<OutputArchive, InputArchive>(_f) {};
		ArchiveInvoker(const ArchiveInvoker& other) : ArchiveInvokerBasic<OutputArchive, InputArchive>(other.func) {};

		uint8_t invoke(OutputArchive& outar, InputArchive& inar) {
			Arg  arg;
			Ret  ret;
			gxx::load(arg,inar);

			auto f = (invoke_func_t<Ret,Arg>) Parent::func;
			uint8_t code = f(ret, arg);
			gxx::save(ret,outar);

			return code;
		}
	};*/
}

#endif