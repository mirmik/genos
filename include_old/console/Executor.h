#ifndef GENOS_EXECUTOR_H
#define GENOS_EXECUTOR_H

namespace Genos {
	class Executor {
	public:
		virtual int execute(const char* str) = 0;
	};

	class DebugExecutor : public Executor {
	public:
		int execute(const char* str) override {
			dpr("DebugExecutor::execute : ");
			dpr(str);
			return 0;
		}
	};
}

#endif