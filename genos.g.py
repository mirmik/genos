from licant.modules import module

licant.execute_recursive("arch", ".g.py")
licant.execute("board/board.g.py")

licant.execute("src/drivers/drivers.g.py")

#module ("genos", include_paths = [".", "src"], sources = [ 
#	"genos/time/systime.cpp", 
#	"genos/impls/proclib.cpp" 
#])
#
module ("genos.include", include_paths = [".", "src"] )
#
##Deprecated
module ("genos.irqtbl", sources = [ "src/hal/irqtable.c" ] )
#module ("genos.irqtable", sources = [ "src/hal/irqtable.c" ] )
#
#module ("genos.tasklet", srcdir="genos/sched/src", sources = [ "tasklet.cpp" ] )
#module ("genos.timer", srcdir="genos/sched/src", sources = [ "timer.cpp" ] )
#module ("genos.schedee", srcdir="genos/sched/src", sources = [ "schedee.cpp", "action.cpp", "process.cpp" ] )
module ("genos.malloc", "lin", sources = ["src/mem/lin_malloc.cpp", "src/mem/lin_realloc.cpp"] )
#
#module ("genos.displace", "reset", srcdir="genos/impls", sources = [ "schedule_displace.cpp" ] )
#
#
#module("genos.addons.adafruit_motor_shield", sources = [
#	"genos/addons/Adafruit_MotorShield/Adafruit_MS_PWMServoDriver.cpp",
#	"genos/addons/Adafruit_MotorShield/Adafruit_MotorShield.cpp",
#])
#
#module("genos.fs", 
#	sources = [
#		"src/fs/vfs.c", 
#		"src/fs/hlpr_path.c", 
#		"src/fs/node.c",
#		"src/fs/mount_table.c",
#	]
#)

module("genos.sched", "stub",
	sources = ["src/sched/waitstub.c"],
)

module("genos.sched", "impl",
	sources = [
		"src/sched/wait.c",
		"src/sched/sched.c",
		"src/sched/schedee.c",
		"src/sched/schedee_mvfs.cpp",
		"src/sched/timer.c",
		"src/sched/api.c",
		"src/sched/schedee/autom.c",
		"src/sched/schedee/cooperative.c",

		"src/sched/posix/fcntl_mvfs.cpp",
		"src/sched/posix/unistd_mvfs.cpp",
	],
)

module("genos.mvfs", 
	sources = [
		"src/mvfs/mvfs.cpp",
		"src/mvfs/fstype.cpp",
		"src/mvfs/super.cpp",
		"src/mvfs/variant/other.cpp",
		"src/mvfs/node.cpp",
		"src/mvfs/pathops.cpp",
		"src/mvfs/vfsmount.cpp",
		"src/mvfs/lookup.cpp",
		"src/mvfs/file.cpp",
		
		"src/drivers/cdev/cdev.cpp",
		"src/drivers/cdev/virtual/null.cpp",
		"src/drivers/cdev/virtual/zero.cpp",
		"src/drivers/cdev/virtual/debug.cpp",

		#"src/drivers/bdev/bdev.cpp",
		#"src/drivers/bdev/virtual/nullb.cpp",
	],

	defines = [
		("MVFS_INCLUDED")
	]
)

module("genos.mvfs.global", 
	sources = [ "src/mvfs/variant/global_root.cpp", "src/mvfs/variant/global_pwd.cpp" ]
)

module("genos.mvfs.schedee_support", 
	sources = [ "src/mvfs/variant/global_root.cpp", "src/mvfs/variant/schedee_support.c" ]
) 

module ("genos.systime", srcdir="src", sources = [ "systime/systime.c" ] )

module ("genos.errno", srcdir="src", sources = [ "errno.c" ])

#module("genos.misc", 
#	sources = [
#		"src/mem/misc/pool.c",
#	]
#)

module ("genos.utility.getty", sources = ["src/utility/nologin_getty.cpp"])
module ("genos.utility.sh", sources = ["src/utility/sh.cpp"])