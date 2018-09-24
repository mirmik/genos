from licant.modules import module

licant.execute_recursive("arch", ".g.py")
licant.execute("board/board.g.py")

licant.execute("src/drivers/drivers.g.py")

module ("genos", include_paths = [".", "src"], sources = [ "genos/time/systime.cpp", "genos/impls/proclib.cpp" ] )
module ("genos.include", include_paths = [".", "src"] )

#Deprecated
module ("genos.irqtbl", sources = [ "src/hal/irqtable.c" ] )
module ("genos.irqtable", sources = [ "src/hal/irqtable.c" ] )

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

module("genos.mvfs", 
	sources = [
		"src/mvfs/mvfs.c",
		"src/mvfs/fstype.c",
		"src/mvfs/super.c",
		#"src/mvfs/dentry.c",
		"src/mvfs/pathops.c",
		"src/mvfs/vfsmount.c",
		"src/mvfs/lookup.c",
		"src/mvfs/file.c",
		
		#"src/drivers/cdev/cdev.c",
		#"src/drivers/cdev/virtual/null.c",
		#"src/drivers/cdev/virtual/zero.c",
		#"src/drivers/cdev/virtual/debug.c",

		#"src/drivers/bdev/bdev.c",
		#"src/drivers/bdev/virtual/nullb.c",
	]
)

module("genos.mvfs.global_root", 
	sources = [ "src/mvfs/variant/global_root.c" ]
)

#module("genos.misc", 
#	sources = [
#		"src/mem/misc/pool.c",
#	]
#)