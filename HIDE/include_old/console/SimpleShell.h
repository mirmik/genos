#ifndef GENOS_SIMPLE_SHELL_H
#define GENOS_SIMPLE_SHELL_H

#include <genos/sigslot/delegate.h>
#include <datastruct/argvc.h>
#include <gxx/hashtable.h>
//#include <gxx/debug/dprint.h>
#include <kernel/stdstream.h>

#include <console/Executor.h>

namespace Genos {

	enum class SimpleShellRecordType {
		Command,
		AutomDelegateSchedee,
		SubstDelegateSchedee,
	};

	class SimpleShellRecord {
	public:
		SimpleShellRecordType type;

		hlist_node hlnk;
		const char* name;
		delegate<int, int, char**> dlg;

		SimpleShellRecord(const char* name, delegate<int, int, char**> dlg, SimpleShellRecordType type) :
			name(name), dlg(dlg), type(type) { }
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
	
		void addCommand(const char* name, delegate<int, int, char**> dlg) {
			auto node = new SimpleShellRecord(name, dlg, SimpleShellRecordType::Command);
			table.put(*node);
		};

		void addAutom(const char* name, delegate<int, int, char**> dlg) {
			auto node = new SimpleShellRecord(name, dlg, SimpleShellRecordType::AutomDelegateSchedee);
			table.put(*node);
		};

		/*void addCommand(const char* name, delegate<int> dlg) {
			addCommand(name, *reinterpret_cast<delegate<int, int, char**>*>(&dlg));
		};*/
	
		int __execute_nosafe(char* str) {
			str_argvc_t str_argvc(str);
			
			auto argc = str_argvc.argc();
			auto argv = str_argvc.argv();

			if (strlen(str) == 0) return EmptyString;
			delegate<int,int,char**> ref;
			
			if (!strcmp(argv[0], "help")) {
				table.foreach([](SimpleShellRecord& rec) {
					outStream().println(rec.name);
				});
				return OK;
			}
	
			auto ret = table.get(argv[0]);
			if (ret == nullptr) {
				outStream().println("FunctionNotExist");
				return -1;
			}

			if (ret->type == SimpleShellRecordType::Command)
				return ret->dlg(argc, argv);

			if (ret->type == SimpleShellRecordType::AutomDelegateSchedee) {

			}
		}
	
		int execute(const char* _str) {	
			char str[128];
			strcpy(str,_str);
			return __execute_nosafe(str);
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