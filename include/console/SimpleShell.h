#ifndef GENOS_SIMPLE_SHELL_H
#define GENOS_SIMPLE_SHELL_H

#include <genos/sigslot/delegate.h>
#include <genos/datastruct/argvc.h>
//#include <gxx/Dictionary.h>
//#include <gxx/ByteArray.h>
#include <gxx/hashtable.h>

#include <debug/dprint.h>

#include <console/Executor.h>

namespace Genos {

	class SimpleShellRecord {
	public:
		hlist_node hlnk;
		const char* name;
		delegate<int, int, char**> dlg;

		SimpleShellRecord(const char* name, delegate<int, int, char**> dlg) :
			name(name), dlg(dlg) { }
		static const char*& getkey(SimpleShellRecord& rec) { return rec.name; }
	};

	class SimpleShell : public Executor {
		gxx::static_hashtable<10, SimpleShellRecord, const char*, &SimpleShellRecord::hlnk> table;
	
	public:
		static constexpr uint8_t OK = 0;
		static constexpr uint8_t FunctionNotExist = -1;
		static constexpr uint8_t EmptyString = -2;
		static constexpr uint8_t WrongArgsTotal = -3;
		static constexpr uint8_t WrongArgsData = -4;
		static constexpr uint8_t InternalError = -5;
	
		//SimpleShell() {};
	
		void add(const char* name, delegate<int, int, char**> dlg) {
			auto node = new SimpleShellRecord(name, dlg);
			table.put(*node);
		};

		void add(const char* name, delegate<int> dlg) {
			add(name, *reinterpret_cast<delegate<int, int, char**>*>(&dlg));
		};
	
		int __execute_nosafe(char* str) {
			argvc_t argvc;
			
			char* v[10];
	
			if (strlen(str) == 0) return EmptyString;
	
			argvc.v = v;
			argvc.internal_split(str);
	
			delegate<int,int,char**> ref;
			
			if (!strcmp(argvc.v[0], "help")) {
				table.foreach([](SimpleShellRecord& rec) {
					dprln(rec.name);
				});
				return OK;
			}
	
			auto ret = table.get(argvc.v[0]);
			if (ret == nullptr) return FunctionNotExist;
			
			return ret->dlg(argvc.c, argvc.v);
		}
	
		int __execute(const char* _str) {	
			char str[128];
			strcpy(str,_str);
			return __execute_nosafe(str);
		}
	
		void execute(const char* str) {
			auto ret = __execute(str);
			if (ret && ret != EmptyString) dprln(strerr(ret));
		}
	
		static const char* strerr(int retcode) {
			switch (retcode) {
				case OK: 
					return "RetCodeOK";
				case EmptyString: 
					return "EmptyString";
				case FunctionNotExist: 
					return "FunctionNotExist";
				case WrongArgsTotal: 
					return "WrongArgsTotal";
				case WrongArgsData: 
					return "WrongArgsData";
				case InternalError: 
					return "InternalError";
				default: 
					return "UnregistredRetCode";
			};
		};
	};
}

#endif