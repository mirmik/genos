from licant.modules import module

licant.execute_recursive("src", ".g.py")
licant.execute_recursive("arch", ".g.py")
licant.execute_recursive("board", ".g.py")
#licant.execute("src/drivers/drivers.g.py")
#licant.execute("src/sched/sched.g.py")

#module ("genos", include_paths = [".", "src"], sources = [ 
#	"genos/time/systime.cpp", 
#	"genos/impls/proclib.cpp" 
#])
#
module ("genos.include", 
	include_paths = [".", "src"],
	mdepends = ["igris.include"] )
#
##Deprecated
module ("genos.irqtable", sources = [ "src/hal/irqtable.cpp" ] )
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
module("genos.addons.adafruit_motor_shield", sources = [
	"src/addons/Adafruit_MotorShield/Adafruit_MS_PWMServoDriver.cpp",
	"src/addons/Adafruit_MotorShield/Adafruit_MotorShield.cpp",
])
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
		"src/sched/sched.cpp",
		"src/sched/displace.cpp",
		"src/sched/schedee.c",
		"src/sched/schedee_mvfs.cpp",
		"src/sched/timer.c",
		"src/sched/api.c",
		"src/sched/apitime.c",
		"src/sched/schedee/autom.c",
		"src/sched/schedee/cooperative.c",

		"src/sched/posix/fcntl_mvfs.cpp",
		"src/sched/posix/unistd_mvfs.cpp",
	],
	default=True,
)

module("genos.mvfs", 
	sources = [
		"src/mvfs/mvfs.c",
		"src/mvfs/fstype.c",
		"src/mvfs/super.c",
		"src/mvfs/node.c",
		"src/mvfs/pathops.c",
		"src/mvfs/lookup.c",
		"src/mvfs/file.c",
		
		"src/drivers/cdev/cdev.c",
		"src/drivers/cdev/virtual/null.c",
		"src/drivers/cdev/virtual/zero.c",
		"src/drivers/cdev/virtual/debug.c",

		#"src/drivers/bdev/bdev.cpp",
		#"src/drivers/bdev/virtual/nullb.cpp",

		"src/mvfs/fs/joke/joke.c"
	],

	defines = [
		("MVFS_INCLUDED")
	]
)

module("genos.mvfs.global", 
	sources = [ "src/mvfs/variant/global_root.c", "src/mvfs/variant/global_pwd.c" ]
)

module("genos.mvfs.schedee_support", 
	sources = [ "src/mvfs/variant/global_root.c", "src/mvfs/variant/schedee_support.c" ]
) 

module ("genos.systime", srcdir="src", sources = [ "systime/systime.c" ] )

module ("genos.errno", srcdir="src", sources = [ "errno.c" ])
module ("genos.cpudelay", srcdir="src", sources = [ "hal/cpudelay.c" ])

module ("genos.utility.numcmd", sources = ["src/utility/numcmd.c"], mdepends=["igris.protocols.numcmd"])
module ("genos.utility.contty", sources = ["src/utility/contty.c"])
module ("genos.utility.contty2", sources = ["src/utility/contty2.c"])
module ("genos.utility.mshell", sources = ["src/utility/mshell.c"])

module("genos.diag", "stub", sources = ["src/diag/diag_stub.c"])
module("genos.diag", "impl", sources = ["src/diag/diag_impl.c"], default=True)

licant.module("igris.dprint", "diag", 
	srcdir="src/diag",
	sources = ["dprint_diag.c"],
	mdepends = [("igris.dprint.common","impl"), "genos.diag"],
)