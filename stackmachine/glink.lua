print(text.green("GLINK START"))

local files = find.findInTree("..", ".*.gll$", ".*HIDE.*")
script:evalFile(files, _ENV)

compiler = CXXDeclarativeCompiler:new{
	buildutils = { 
		CXX = "g++", 
		CC = "gcc", 
		AR = "ar", 
		LD = "ld", 
		OBJDUMP = "objdump" 
	},
	opts = {
		--weakRecompile = "noscript",
		optimization = "-O2",
		standart = {
			cxx = "-std=gnu++11",
			cc = "-std=gnu11",
		},
		options = {
			all = "-Wl,--gc-sections -fdata-sections -ffunction-sections -Werror",
			cc = "",
			cxx = "",
			ld = "",
		}
	},
	builddir = "./build",
}

compiler:standartArgsRoutine(OPTS)

Module("main", {
	sources = {
		cc = "main.c smach.c smachL.c",
	},

	opts = {
		includePaths = ".",
	},

	modules = {
	},

	includeModules = {
		{ name = "genos.include" }
	},
})

compiler:updateBuildDirectory()

local ret = compiler:assembleModule("main", {
})

if not ret then print(text.yellow("Nothing to do")) end