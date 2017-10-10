from licant.modules import module

scriptq.execute_recursive("arch", ".g.py")
scriptq.execute_recursive("board", ".g.py")

scriptq.execute("src/genos/irq/irq.g.py")
#scriptq.execute("src/genos/sched/scheduler.g.py")
#scriptq.execute("src/genos/sync/sync.g.py")

module ("genos.include",
	include_paths = "./src",
)