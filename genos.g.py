from licant.modules import module
import traceback

licant.module("genos",
	mdepends = [
		"genos.schedee",
		"genos.include"
	]
)

licant.module("genos.include", include_paths=["."])

licant.module("genos.schedee", 
	sources = ["genos/schedee.c"],
	mdepends = ["genos.ktimer"]
)

licant.module("genos.ktimer", sources = ["genos/ktimer.c"])