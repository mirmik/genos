#ifndef GENOS_EXECUTOR_H
#define GENOS_EXECUTOR_H

namespace Genos {
	class Executor {
	public:
		virtual void execute(const char* str) = 0;
	};

	class DebugExecutor : public Executor {
	public:
		void execute(const char* str) override {
			dpr("DebugExecutor::execute : ");
			dpr(str);
		}
	};
}

#endif