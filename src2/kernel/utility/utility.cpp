#include <kernel/utility/utility.h>
#include <kernel/sched/SchedeeTable.h>

namespace Genos {

	int utility::kill(int argc, char** argv) {
		Stream& out = outStream();

		if (argc < 2) {
			out.println("Who need die?");
			return -1;
		}

		int a = atoi(argv[1]);
	
		if (a != 0) {
			return final(a);
		} 
		else {
			auto ret = Glue::systemSchedeeTable().find(
			[&argv](Schedee& sch){
				return !strcmp(sch.name(), argv[1]);
			});
			if (ret) {
				ret->final();
				return 0;
			}
			return 0;
		}	
	}
	
	
	int utility::killall(int argc, char** argv) {
		Stream& out = outStream();


		if (argc < 2) {
			out.println("Who need die?");
			return -1;
		}
	
		Schedee* sch;
		Glue::systemSchedeeTable().foreach(
			[&argv](Schedee& sch)
		{
			if (!strcmp(sch.name(), argv[1])) {
				sch.final();
			}	
		});
		return 0;
	}
	
	//TODO change to stdout
	int utility::printSchedeeTable(int argc, char** argv) {
		Stream& out = outStream();

		Glue::systemSchedeeTable().foreach([&out](Schedee& sch){
			out.print(sch.pid); 
			out.print("\t");
			out.print(sch.state);
			out.print("\t"); 
			if(sch.name() != nullptr) out.print(sch.name());
			out.print("\t"); 
			auto parent = sch.parent;
			if (parent != nullptr) out.print(parent->name());
			out.println();
		});
	}
	
	int utility::addKernelUtility(SimpleShell& shell) {
		shell.addCommand("kill", utility::kill);
		shell.addCommand("killall", utility::killall);
		shell.addCommand("ps", printSchedeeTable);
	}

}