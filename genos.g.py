from licant.modules import module
import traceback

licant.module("genos",
	mdepends = [
		"genos.schedee",
		"genos.include",
		#"genos.resources"
	]
)

licant.module("genos.include", include_paths=["."])

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
	mdepends = ["genos.ktimer", "genos.tasklet"]
)

licant.module("genos.resources",
	sources = [
		"genos/resource/namespace.c",
		"genos/resource/file_head.c",
		"genos/drivers/devns.c",
		"genos/drivers/device_head.c",
		"genos/drivers/virtual/devnull.c",
		"genos/drivers/stream_adapter.c",
		"genos/resource/mvfs.c",
		"genos/resource/openres.c",
		]
)

licant.module("genos.host_stub", 
	include_paths = ["genos/host-stub"]
)

licant.module("genos.ktimer", sources = ["genos/ktimer.cpp"])
licant.module("genos.tasklet", sources = ["genos/tasklet.cpp"])