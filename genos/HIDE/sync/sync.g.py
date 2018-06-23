implementation("genos.atomic", "irqs",
	sources = ["atomic_section_irqs.c"]
)

implementation("genos.atomic", "std::mutex",
	sources = ["atomic_section_mutex.cpp"]
)