from licant.modules import module

scriptq.execute_recursive("arch", ".g.py")
scriptq.execute_recursive("board", ".g.py")

module ("genos.include", include_paths = "./src" )

module ("genos.irqtbl", 	sources = [ "src/hal/irqtbl.cpp" ] )
module ("genos.tasklet", 	sources = [ "src/genos/tasklet.cpp" ] )
module ("genos.timer", 		sources = [ "src/genos/timer.cpp" ] )
module ("genos.schedee",	sources = [ "src/genos/sched/schedee.cpp" ] )

module ("genos.atomic", impl = "irqs", sources = [ "src/genos/impls/atomic_section_irqs.cpp" ] )


module("genos.malloc", "lin", sources = ["src/mem/lin_malloc.c", "src/mem/lin_realloc.c"] )