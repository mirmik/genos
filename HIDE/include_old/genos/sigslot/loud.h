#ifndef GENOS_SIGSLOT_LOUD_H
#define GENOS_SIGSLOT_LOUD_H

#include <gxx/utility.h>
#include <genos/sigslot/sigslot.h>

namespace gxx {

	template <typename T>
	class loud {
	private:
		T m_data;
		signal<const T&> m_sig;
	public:
		loud(const T& ival) : m_data(ival) {}

		void update(const T& lref) {
			bool sig = m_data != lref;
			m_data = lref;
			if (sig) m_sig(m_data);
		}

		void update(T&& rref) {
			bool sig = m_data != lref;
			m_data = gxx::forward<T>(lref);
			if (sig) m_sig(m_data);
		}

		T get() {
			return m_data;
		} 
	};
};

#endif