Module("genos.include.libc", {
	modules : [
	],
	opts : {
		includePaths : ["include"]
	},
	depends : ["genos.libc"],
})

var libc = []
var stdlib = script.findInTree(path.resolve(script.currentDir, "stdlib"), /.*\.c/, /.*HIDE.*/);
var string = script.findInTree(path.resolve(script.currentDir, "string"), /.*\.c/, /.*HIDE.*/);
var inttypes = script.findInTree(path.resolve(script.currentDir, "inttypes"), /.*\.c/, /.*HIDE.*/);

Module("genos.libc", {
	sources : {
		cc : libc.concat(stdlib, string, inttypes),	
	},
	opts : {
		options : {
			cc : ["-Wno-implicit-function-declaration"]
		}
	}
})