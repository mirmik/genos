#include <igris/dprint.h>
#include <stdint.h>

void hardfault_handler(void) __attribute__((naked));
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
    __attribute__((used));

/* The fault handler implementation calls a function called
prvGetRegistersFromStack(). */
void hardfault_handler(void)
{
    __asm volatile(
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n");
}

struct mfsr_reg // Memory Management Fault Status
{
    unsigned char IACCVIOL : 1; // Instruction access violation
    unsigned char DACCVIOL : 1; // Data access violation
    unsigned char UnusedBits : 1;
    unsigned char MUNSTKERR : 1; // Unstacking error
    unsigned char MSTKERR : 1;   // Stacking error
    unsigned char UnusedBits2 : 2;
    unsigned char MMARVALID : 1; // Indicates the MMAR is valid
};

struct bfsr_reg // Bus Fault Status Register (0xE000ED29)
{
    unsigned int IBUSERR : 1;    // Instruction access violation
    unsigned int PRECISERR : 1;  // Precise data access violation
    unsigned int IMPREISERR : 1; // Imprecise data access violation
    unsigned int UNSTKERR : 1;   // Unstacking error
    unsigned int STKERR : 1;     // Stacking error
    unsigned int UnusedBits : 2;
    unsigned int BFARVALID : 1; // Indicates BFAR is valid
};

struct ufsr_reg // Usage Fault Status Register (0xE000ED2A)
{
    unsigned short UNDEFINSTR : 1; // Attempts to execute an undefined
    // instruction
    unsigned short INVSTATE : 1; // Attempts to switch to an invalid state
    // (e.g., ARM)
    unsigned short INVPC : 1; // Attempts to do an exception with a bad
    // value in the EXC_RETURN number
    unsigned short NOCP : 1; // Attempts to execute a coprocessor
    // instruction
    unsigned short UnusedBits : 4;
    unsigned short UNALIGNED : 1; // Indicates that an unaligned access fault
    // has taken place
    unsigned short DIVBYZERO : 1; // Indicates a divide by zero has taken
    // place (can be set only if DIV_0_TRP
    // is set)
};

void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
    /* These are volatile to try and prevent the compiler/linker optimising them
    away as the variables never actually get used.  If the debugger won't show
    the values of the variables, make them global my moving their declaration
    outside of this function. */
    volatile uint32_t r0;
    volatile uint32_t r1;
    volatile uint32_t r2;
    volatile uint32_t r3;
    volatile uint32_t r12;
    volatile uint32_t lr;  /* Link register. */
    volatile uint32_t pc;  /* Program counter. */
    volatile uint32_t psr; /* Program status register. */

    r0 = pulFaultStackAddress[0];
    r1 = pulFaultStackAddress[1];
    r2 = pulFaultStackAddress[2];
    r3 = pulFaultStackAddress[3];

    r12 = pulFaultStackAddress[4];
    lr = pulFaultStackAddress[5];
    pc = pulFaultStackAddress[6];
    psr = pulFaultStackAddress[7];

    debug_print_newline();
    dprln("HARDFAULT:");

    debug_print("HFSR: ");
    debug_printhex_uint32(*(uint32_t *)0xE000ED2C);
    debug_print_newline();

    debug_print("CFSR: ");
    debug_printhex_uint32(*(uint32_t *)0xE000ED28);
    debug_print_newline();

    debug_print("UFSR: ");
    debug_printhex_uint16(*(uint16_t *)0xE000ED2A);
    debug_print_newline();

    debug_print("BFSR: ");
    debug_printhex_uint8(*(uint8_t *)0xE000ED29);
    debug_print_newline();

    debug_print("MMFSR: ");
    debug_printhex_uint8(*(uint8_t *)0xE000ED28);
    debug_print_newline();

    debug_print("ABFSR: ");
    debug_printhex_uint32(*(uint32_t *)0xE000EFA8);
    debug_print_newline();

    uint8_t bfsr = *(uint8_t *)0xE000ED29;
    uint8_t mmfsr = *(uint8_t *)0xE000ED28;
    uint16_t ufsr = *(uint16_t *)0xE000ED2A;

    struct bfsr_reg *bfsr_reg = (struct bfsr_reg *)&bfsr;
    struct mfsr_reg *mfsr_reg = (struct mfsr_reg *)&mmfsr;
    struct ufsr_reg *ufsr_reg = (struct ufsr_reg *)&ufsr;

    if (bfsr_reg->IBUSERR)
        dprln("IBUSERR");
    if (bfsr_reg->PRECISERR)
        dprln("PRECISERR");
    if (bfsr_reg->IMPREISERR)
        dprln("IMPREISERR");
    if (bfsr_reg->UNSTKERR)
        dprln("UNSTKERR");
    if (bfsr_reg->STKERR)
        dprln("STKERR");
    if (bfsr_reg->BFARVALID)
        dprln("BFARVALID");

    // TODO: Add more fault types
    (void)mfsr_reg;
    (void)ufsr_reg;

    DPRINTHEX(r0);
    DPRINTHEX(r1);
    DPRINTHEX(r2);
    DPRINTHEX(r3);
    DPRINTHEX(r12);
    DPRINTHEX(lr);
    DPRINTHEX(pc);
    DPRINTHEX(psr);

    /* When the following line is hit, the variables contain the register
     * values. */
    for (;;)
        ;
}