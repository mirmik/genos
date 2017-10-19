from licant.modules import module

scriptq.execute_recursive("arch", ".g.py")
scriptq.execute_recursive("board", ".g.py")

module ("genos.include", include_paths = "./src" )

module ("genos.irqtbl", 	sources = [ "src/hal/irqtbl.cpp" ] )
module ("genos.tasklet", 	sources = [ "src/genos/tasklet.cpp" ] )
module ("genos.timer", 		sources = [ "src/genos/timer.cpp" ] )