#ifndef __ARCH_ARM_INCLUDE_ARMV6_M_IRQ_H
#define __ARCH_ARM_INCLUDE_ARMV6_M_IRQ_H

#define REG_R13             (0)  /* R13 = SP at time of interrupt */
#define REG_PRIMASK         (1)  /* PRIMASK */
#define REG_R4              (2)  /* R4 */
#define REG_R5              (3)  /* R5 */
#define REG_R6              (4)  /* R6 */
#define REG_R7              (5)  /* R7 */
#define REG_R8              (6)  /* R8 */
#define REG_R9              (7)  /* R9 */
#define REG_R10             (8)  /* R10 */
#define REG_R11             (9)  /* R11 */

static inline uint8_t getprimask(void) {
  uint32_t primask;
  __asm__ __volatile__
    (
     "\tmrs  %0, primask\n"
     : "=r" (primask)
     :
     : "memory");

  return (uint8_t)primask;
}

static inline void setprimask(uint32_t primask) {
  __asm__ __volatile__
    (
      "\tmsr primask, %0\n"
      :
      : "r" (primask)
      : "memory");
}

static inline void up_irq_disable(void) {
  __asm__ __volatile__ ("\tcpsid  i\n");
}

static inline irqstate_t up_irq_save(void) {
  unsigned short primask;

  __asm__ __volatile__
    (
     "\tmrs    %0, primask\n"
     "\tcpsid  i\n"
     : "=r" (primask)
     :
     : "memory");

  return primask;
}

/* Enable IRQs */

static inline void up_irq_enable(void) {
  __asm__ __volatile__ ("\tcpsie  i\n");
}

static inline void up_irq_restore(irqstate_t flags) {
  __asm__ __volatile__
    (
      "\tmsr primask, %0\n"
      :
      : "r" (flags)
      : "memory");
}

static inline uint32_t getipsr(void) {
  uint32_t ipsr;
  __asm__ __volatile__
    (
     "\tmrs  %0, ipsr\n"
     : "=r" (ipsr)
     :
     : "memory");

  return ipsr;
}

static inline uint32_t getcontrol(void) {
  uint32_t control;
  __asm__ __volatile__
    (
     "\tmrs  %0, control\n"
     : "=r" (control)
     :
     : "memory");

  return control;
}

static inline void setcontrol(uint32_t control) {
  __asm__ __volatile__
    (
      "\tmsr control, %0\n"
      :
      : "r" (control)
      : "memory");
}

#endif