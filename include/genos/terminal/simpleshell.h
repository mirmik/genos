#ifndef GENOS_SIMPLE_SHELL_H
#define GENOS_SIMPLE_SHELL_H

#include <genos/sigslot/delegate.h>
#include <genos/datastruct/argvc.h>
#include <gxx/Dictionary.h>
#include <gxx/ByteArray.h>

#include <debug/dprint.h>

class SShell {
	gxx::Dictionary<const char*, delegate<int, int, char**>> dict;

public:
	static constexpr uint8_t RetCodeOK = 0;
	static constexpr uint8_t FunctionNotExist = -1;
	static constexpr uint8_t EmptyString = -2;
	static constexpr uint8_t WrongArgsTotal = -3;
	static constexpr uint8_t WrongArgsData = -4;

	SShell() : dict() {};

	void add(const char* name, delegate<int, int, char**> dlg) {
		dict.insert(name, dlg);
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
			return RetCodeOK;
		}

		auto retcode = dict.get(argvc.v[0],ref);
		if (retcode == false) return FunctionNotExist;
		
		return ref(argvc.c, argvc.v);
	}

	int execute_s(char* _str) {	
		char str[128];
		strcpy(str,_str);
		return execute(str);
	}

	void executor(char* str) {
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
				return gxx::string::format("UnregistredRetCode %d", retcode);
		};
	};
};

#endif