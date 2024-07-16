from licant.modules import module

module("libscpi",
	srcdir = "src",
	sources = """error.c fifo.c ieee488.c 
	minimal.c parser.c units.c utils.c 
	lexer.c expression.c""".split(),
	include_paths = ["inc"]
)