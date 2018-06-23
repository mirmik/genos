#ifndef GENOS_TIMER_H
#define GENOS_TIMER_H

namespace genos {
	template <typename T, typename I = decltype(T()-T())>
	struct timer {
		T start;
		I interval;
		bool check(T now) { return now - start >= interval; }
		T final() { return start + interval; }
		void swift() { start += interval; }
	};
}

#endif