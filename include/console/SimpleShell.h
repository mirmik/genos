#ifndef GENOS_SIMPLE_SHELL_H
#define GENOS_SIMPLE_SHELL_H

#include <genos/sigslot/delegate.h>
#include <genos/datastruct/argvc.h>
//#include <gxx/Dictionary.h>
//#include <gxx/ByteArray.h>
#include <gxx/hashtable.h>

#include <debug/dprint.h>

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

	class SimpleShell {
		gxx::static_hashtable<10, SimpleShellRecord, const char*, &SimpleShellRecord::hlnk> table;
	
	public:
		static constexpr uint8_t RetCodeOK = 0;
		static constexpr uint8_t FunctionNotExist = -1;
		static constexpr uint8_t EmptyString = -2;
		static constexpr uint8_t WrongArgsTotal = -3;
		static constexpr uint8_t WrongArgsData = -4;
	
		//SimpleShell() {};
	
		void add(const char* name, delegate<int, int, char**> dlg) {
			auto node = new SimpleShellRecord(name, dlg);
			table.put(node);
		};
	
		int execute(char* str) {
			argvc_t argvc;
			
			char* v[10];
	
			if (strlen(str) == 0) return EmptyString;
	
			argvc.v = v;
			argvc.internal_split(str);
	
			delegate<int,int,char**> ref;
	
			if (!strcmp(argvc.v[0], "help")) {
				dprln("HELP: TODO");
				//for (auto rec : dict) {
	
				//}
	
				return RetCodeOK;
			}
	
			auto ret = table.get(argvc.v[0]);
			if (ret == nullptr) return FunctionNotExist;
			
			return ret->dlg(argvc.c, argvc.v);
		}
	
		int execute_safe(const char* _str) {	
			char str[128];
			strcpy(str,_str);
			return execute(str);
		}
	
		void execute_info(char* str) {
			auto ret = execute(str);
			if (ret) dprln(strerr(ret).c_str());
		}
	
		static gxx::string strerr(int retcode) {
			switch (retcode) {
				case RetCodeOK: 
					return gxx::string("RetCodeOK");
				case EmptyString: 
					return gxx::string("EmptyString");
				case FunctionNotExist: 
					return gxx::string("FunctionNotExist");
				case WrongArgsTotal: 
					return gxx::string("WrongArgsTotal");
				case WrongArgsData: 
					return gxx::string("WrongArgsData");
				default: 
					return gxx::string("UnregistredRetCode ") += retcode;
			};
		};
	};
}

#endif