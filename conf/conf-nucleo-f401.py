import licant

binutils = licant.cxx_make.make_gcc_binutils("arm-none-eabi")
modules = [
	("genos.board", "nucleo-f401re"),
	"genos.drivers.stm32"
]