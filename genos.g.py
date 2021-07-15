from licant.modules import module
import traceback

licant.module("genos",
	mdepends = [
		"genos.schedee",
		"genos.include",
		"genos.resources",
		"genos.utilities"
	]
)

licant.module("genos.include", include_paths=["."])

licant.module("genos.schedee", 
	sources = ["genos/schedee.c", "genos/schedee_api.c", "genos/autom_schedee.c", "genos/wait.c"],
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

licant.module("genos.utilities",
	sources = [
		"genos/vterm.c",
		]
)

licant.module("genos.ktimer", sources = ["genos/ktimer.c"])
licant.module("genos.tasklet", sources = ["genos/tasklet.c"])