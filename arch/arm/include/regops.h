#ifndef ARM_REGOPS_H
#define ARM_REGOPS_H

# define getreg8(a)           (*(volatile uint8_t *)(a))
# define putreg8(v,a)         (*(volatile uint8_t *)(a) = (v))
# define getreg32(a)          (*(volatile uint32_t *)(a))
# define putreg32(v,a)        (*(volatile uint32_t *)(a) = (v))

/* Some compiler options will convert short loads and stores into byte loads
 * and stores.  We don't want this to happen for IO reads and writes!
 */

/* # define getreg16(a)       (*(volatile uint16_t *)(a)) */
static inline uint16_t getreg16(unsigned int addr) {
	uint16_t retval;
	__asm__ __volatile__("\tldrh %0, [%1]\n\t" : "=r"(retval) : "r"(addr));
	return retval;
}

/* define putreg16(v,a)       (*(volatile uint16_t *)(a) = (v)) */
static inline void putreg16(uint16_t val, unsigned int addr) {
	__asm__ __volatile__("\tstrh %0, [%1]\n\t": : "r"(val), "r"(addr));
}

static inline void* get_stack_pointer() {
	void* retval;
	__asm__ __volatile__("mov %0, sp\n\t" : "=r"(retval) : );	
	return retval;
};

static inline void* get_program_counter() {
	void* retval;
	__asm__ __volatile__("mov %0, pc\n\t" : "=r"(retval) : );
	return retval;
};


__BEGIN_DECLS

/* Atomic modification of registers */
void modifyreg8(unsigned int addr, uint8_t clearbits, uint8_t setbits);
void modifyreg16(unsigned int addr, uint16_t clearbits, uint16_t setbits);
void modifyreg32(unsigned int addr, uint32_t clearbits, uint32_t setbits);

__END_DECLS

#endif
