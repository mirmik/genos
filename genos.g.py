from licant.modules import module

scriptq.execute_recursive("arch", ".g.py")
scriptq.execute("board/board.g.py")

module ("genos", include_paths = [".", "src"], sources = [ "genos/time/systime.cpp", "genos/impls/proclib.cpp" ] )
module ("genos.include", include_paths = [".", "src"] )

module ("genos.irqtbl", sources = [ "genos/hal/irqtbl.cpp" ] )
module ("genos.tasklet", srcdir="genos/sched/src", sources = [ "tasklet.cpp" ] )
module ("genos.timer", srcdir="genos/sched/src", sources = [ "timer.cpp" ] )
module ("genos.schedee", srcdir="genos/sched/src", sources = [ "schedee.cpp", "action.cpp", "process.cpp" ] )
module ("genos.malloc", "lin", sources = ["src/mem/lin_malloc.cpp", "src/mem/lin_realloc.cpp"] )

module ("genos.displace", "reset", srcdir="genos/impls", sources = [ "schedule_displace.cpp" ] )


module("genos.addons.adafruit_motor_shield", sources = [
	"genos/addons/Adafruit_MotorShield/Adafruit_MS_PWMServoDriver.cpp",
	"genos/addons/Adafruit_MotorShield/Adafruit_MotorShield.cpp",
])

module("genos.fs", 
	sources = [
		"src/fs/vfs.c", 
		"src/fs/hlpr_path.c", 
		"src/fs/node.c",
		"src/fs/mount_table.c",
	]
)

module("genos.misc", 
	sources = [
		"src/mem/misc/pool.c",
	]
)