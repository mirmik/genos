from licant.modules import module
import traceback

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
module ("genos.malloc", "lin", 
	sources=["src/mem/lin_malloc.cpp", "src/mem/lin_realloc.cpp"], 
	default=True
)
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
		"src/genos/sched.cpp",
		"src/genos/schedee.cpp",
		"src/genos/schedee_mvfs.cpp",
		"src/genos/apitime.cpp",
		"src/genos/schedee/autom.cpp",
		"src/genos/schedee/coop.cpp",

		"src/sched/posix/fcntl_mvfs.cpp",
		"src/sched/posix/unistd_mvfs.cpp",

		"src/genos/wait.cpp",
		"src/genos/ktimer.cpp",

		"src/genos/fops.cpp",
		
		"src/genos/resource.cpp",
		"src/genos/resmngr.cpp",
		"src/genos/nav.cpp",
		"src/genos/dev.cpp",
		
		"src/genos/errno.cpp"
	],
	default=True,
)

#module("genos.wait", "static", sources=["src/sched/wait_static.c"], default=True)
#module("genos.wait", "malloc", sources=["src/sched/wait_malloc.c"])

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
module ("genos.utility.contty", sources = ["src/utility/contty.cpp"])
module ("genos.utility.contty2", sources = ["src/utility/contty2.c"])
module ("genos.utility.contty3", sources = ["src/utility/contty3.c"])
module ("genos.utility.mshell", sources = ["src/utility/mshell.c"])

module("genos.diag", "stub", sources = ["src/diag/diag_stub.c"])
module("genos.diag", "impl", sources = ["src/diag/diag_impl.c"], default=True)

licant.module("igris.dprint", "diag", 
	srcdir="src/diag",
	sources = ["dprint_diag.c"],
	mdepends = [("igris.dprint.common","impl"), "genos.diag"],
)

module("genos",
	mdepends = 
	[
		"igris.include",
		"genos.include",
		
		"igris.stdlibs",
		"genos.irqtable",

		"igris.util",
		"igris.bug",
		("igris.dprint", "diag"),
		("igris.syslock", "genos.atomic"),

		"genos.sched",
		"igris.cxx_support"
	]
)





def genos_firmware(sources=[], mdepends=[], **kwargs):
	import shutil
	import licant.modules

	if os.path.exists("__configure__.py"):
		try:
			import __configure__
		except:
			print("Error in configuration file.")		

	dir_path = licant.modules.mlibrary.get("genos").opts["__dir__"]

	@licant.routine
	def configure(board):
		print("load configuration script for {}".format(board))

		confpath = os.path.join(dir_path, "conf", "conf-" + board + ".py")


		shutil.copy(
			confpath,
			"__configure__.py"
		)

	if not os.path.exists("__configure__.py"):
		@licant.routine
		def firmware(*args, **kwargs):
			print("Enougth of configuration file")
	else:
		try:
			licant.include("igris")

			all_modules = ["genos"]
			all_modules.extend(__configure__.mdepends)
			all_modules.extend(mdepends)

			licant.cxx_application("firmware",
				binutils=__configure__.binutils,
				sources=sources,
				mdepends=all_modules,
				**kwargs
			)

			@licant.routine(deps=["firmware"])
			def install(*args):
				__configure__.install("firmware", *args)

			@licant.routine
			def terminal(*args):
				__configure__.terminal(*args)

		except Exception as e:
			print(e)
			traceback.print_exc()
			print("Error in firmware module. Configuration problem?")

	licant.ex("firmware")

licant.global_function(genos_firmware)



