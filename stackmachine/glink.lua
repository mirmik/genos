print(text.green("GLINK START"))

local files = find.findInTree("../genos", ".*.gll$", ".*HIDE.*")
script:evalFile(files, _ENV)

compiler = CXXModuleCompiler:new{
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
--compiler.debugInfo = true;
--compiler.parallel = true;

if (OPTS[1]) then
	if OPTS[1] == "clean" then
		compiler:cleanBuildDirectory()
		os.exit(0)
	elseif OPTS[1] == "rebuild" then
		compiler.rebuild = true 
	elseif OPTS[1] == "install" then
		os.execute("bash ./install.sh")
		os.exit(1) 
	else
		error(text.red("Unresolved Parametr"))
	end
end

if (OPTS.parallel) then
	compiler.parallel = true
end

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