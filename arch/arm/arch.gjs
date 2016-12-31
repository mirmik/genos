
Module("genos.arch.stm32", {
	sources : {
		cc : [
			"stm32/src/stm32_start.c", "stm32/src/stm32_gpio.c", "stm32/src/stm32_diag.c", "stm32/src/stm32_usart.c", "stm32/src/stm32_rcc.c", "stm32/src/stm32_arch.c", "stm32/src/stm32_vectors.S", "stm32/src/stm32_irq.c",  
		]
	},
	opts : {
		options : [""]
	}
})

Module("genos.include.arch.stm32f407", {
	opts : {
		includePaths : ["include", "stm32/include", "stm32/stm32f407/include"],
		ldscripts : ["stm32/stm32f407/ldscript.ld"],
	},
	modules : [
		{name : "genos.arch.stm32"},
	],
})