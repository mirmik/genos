from licant import module

module("genos.sched", "stub",
	sources = ["waitstub.c"],
)


module("genos.sched.cooperative", "impl", sources=[
	"schedee/cooperative.cpp",
	"displace.cpp"
], default=True)

module("genos.sched", "impl",
	sources = [
		"wait.c",
		"sched.cpp",
		"schedee.c",
		"schedee_mvfs.cpp",
		"schedee/autom.cpp",

		"api.c",

		"posix/fcntl_mvfs.cpp",
		"posix/unistd_mvfs.cpp",
	],
	mdepends=[
		"genos.sched.cooperative"
	]
)

module("genos.sched.timer_manager", "impl",
	sources = [
		"timer.c",
		"apitime.c",
	],
	mdepends=[
		"genos.sched.cooperative"
	],
	default = True
)