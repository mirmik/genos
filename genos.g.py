from licant.modules import module

scriptq.execute_recursive("arch", ".g.py")
scriptq.execute_recursive("board", ".g.py")

module ("genos", include_paths = [".", "src"], sources = [ "genos/time/systime.cpp" ] )
module ("genos.include", include_paths = [".", "src"] )

module ("genos.irqtbl", sources = [ "genos/hal/irqtbl.cpp" ] )
module ("genos.tasklet", srcdir="genos/sched/src", sources = [ "tasklet.cpp" ] )
module ("genos.timer", srcdir="genos/sched/src", sources = [ "timer.cpp" ] )
module ("genos.schedee", srcdir="genos/sched/src", sources = [ "schedee.cpp", "action.cpp", "process.cpp" ] )

#module ("genos.atomic", impl = "irqs", sources = [ "genos/impls/atomic_section_irqs.cpp" ] )
#module ("genos.atomic", impl = "mutex", sources = [ "genos/impls/atomic_section_mutex.cpp" ] )

module("genos.malloc", "lin", sources = ["src/mem/lin_malloc.cpp", "src/mem/lin_realloc.cpp"] )

module ("genos.displace", "reset", srcdir="genos/impls", sources = [ "schedule_displace.cpp" ] )