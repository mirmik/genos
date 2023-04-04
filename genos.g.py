from licant.modules import module
import traceback

licant.module("genos",
	mdepends = [
		"genos.schedee",
		"genos.include",
		"genos.resources",
		"genos.drivers",
		"genos.utilities"
	]
)

licant.module("genos.include", include_paths=["."])

licant.module("genos.utilities", sources=["genos/utility/*.cpp"])

licant.module("genos.schedee", 
	sources = [
		"genos/schedee.cpp", 
		"genos/schedee_api.cpp", 
		"genos/autom_schedee.cpp", 
		"genos/coop_schedee.cpp", 
		"genos/displace.cpp", 
		"genos/wait.cpp",
		"genos/chardev.cpp",
		"genos/fork.cpp",
		"genos/invoke.cpp",
	],
	mdepends = ["genos.ktimer", "genos.tasklet", "genos.drivers"]
)

licant.module("genos.drivers", 
	sources = [
		"genos/drivers/*.cpp",
	],
)

#licant.module("genos.tty", sources = ["genos/drivers/tty.cpp"])

licant.module("genos.posix",
	sources=["genos/posix/*.cpp","genos/posix/sys/*.cpp"],
	include_paths = ["genos/posix/"]
)

licant.module("genos.resources",
	sources = [
		"genos/resource/*.cpp",
		]
)

licant.module("genos.shellutil",
	sources=["genos/shellutil.cpp"]
)

licant.module("genos.host_stub", 
	include_paths = ["genos/host-stub"]
)

licant.module("genos.ktimer", sources = ["genos/ktimer.cpp"])
licant.module("genos.tasklet", sources = ["genos/tasklet.cpp"])