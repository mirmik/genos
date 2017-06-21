#ifndef GXX_IERARCHY_H
#define GXX_IERARCHY_H

#include <gxx/ByteArray.h>
#include <gxx/DList.h>

namespace gxx {

	class IerarchyObject {
	public:
		dlist_head lnk;
		DList<IerarchyObject, &IerarchyObject::lnk> nodes;

		QString m_name;

		IerarchyObject(QString name) : m_name(name), nodes() {
			dlist_init(&lnk);
		}

		/*QString to_str() {
			QString str;
			for ( auto n : nodes ) {
				str << n.m_name << "\r\n";
			} 
			return str;
		}*/

		void add(IerarchyObject* obj) {
			nodes.push_back(*obj);
		}

		IerarchyObject* lookup(QString str) {
			QStringList entrylist = str.split('/');
			int size = entrylist.size();

			IerarchyObject* current = this;

			for(int i = 0; i < size; i++) {
				auto it = current->nodes.begin();
				for(it = current->nodes.begin(); it != current->nodes.end(); it++) {
					if (it->m_name == entrylist[i]) break;
				}
				if (it == current->nodes.end()) return nullptr;
				current = &*it; 
			}
			return current;
		}
	};

};

#endif