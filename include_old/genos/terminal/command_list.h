#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

	#include "util/stub.h"	
	#include <datastruct/dlist_head.h>
	#include "genos/sigslot/delegate.h"
	#include <string.h>
	#include <util/split.h>
	
	//#include <genos/io/stream.h>

	typedef void(*executed_t)(int,char**);

	class command_t{
		public:
		delegate<void, int, char**> _d;
		const char* _name;
		dlist_head lst;
		public:
		command_t(const char* name, delegate<void, int, char**> d) : _d(d), _name(name) {};
	};
	
	class command_list{
		
		dlist_head list;

		public:
		command_list() : list() {};
		
		void add(const char* str, delegate<void, int, char**> d) 
		{
			command_t* cmd = new command_t(str, d); 
			dlist_add_next(&cmd->lst, &list);
		};	

		void add(const char* str, void(*f)(int,char**)) 
		{
			delegate<void, int, char**>d(f);
			command_t* cmd = new command_t(str, d); 
			dlist_add_next(&cmd->lst, &list);
		};	


		void add(const char* str, void(*f)()) 
		{
			delegate<void, int, char**>d((void(*)(int,char**)) f);
			command_t* cmd = new command_t(str, d); 
			dlist_add_next(&cmd->lst, &list);
		};

		template<typename T>
		void add(const char* str, T* a, void (T::*ptr)(int, char**))
		{
			delegate<void, int, char**>d = delegate<void, int, char**>(a, ptr);
			command_t* cmd = new command_t(str, d); 
			dlist_add_next(&cmd->lst, &list);
		};

		template<typename T>
		void add(const char* str, T* a, void (T::*ptr)())
		{
			delegate<void, int, char**>d = delegate<void, int, char**>(a, (void (T::*)(int, char**))ptr);
			command_t* cmd = new command_t(str, d); 
			dlist_add_next(&cmd->lst, &list);
		};

		int find(const char* str,  delegate<void, int, char**> *outd) 
		{
			*outd = (executed_t) do_nothing;
			command_t* elem;
			dlist_for_each_entry(elem, &list, lst)
			{
				if (!strcmp(elem->_name, str)) 
				{
					*outd = elem->_d;
					return 0;
				};
			};
			return -1;
		};	

	};
	

#endif