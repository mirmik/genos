#ifndef SAMD21_PORT_H
#define SAMD21_PORT_H

#include <genos/util/regs.h>
#include <inttypes.h>

struct port_group_regs {
  __IO uint32_t DIR;         // 0x00 (R/W 32) Data Direction
  __IO uint32_t DIRCLR;      // 0x04 (R/W 32) Data Direction Clear
  __IO uint32_t DIRSET;      // 0x08 (R/W 32) Data Direction Set
  __IO uint32_t DIRTGL;      // 0x0C (R/W 32) Data Direction Toggle
  __IO uint32_t OUT;         // 0x10 (R/W 32) Data Output Value
  __IO uint32_t OUTCLR;      // 0x14 (R/W 32) Data Output Value Clear
  __IO uint32_t OUTSET;      // 0x18 (R/W 32) Data Output Value Set
  __IO uint32_t OUTTGL;      // 0x1C (R/W 32) Data Output Value Toggle
  __I  uint32_t IN;          // 0x20 (R/  32) Data Input Value
  __IO uint32_t CTRL;        // 0x24 (R/W 32) Control
  __O  uint32_t WRCONFIG;    // 0x28 ( /W 32) Write Configuration
       uint8_t Reserved1[0x4];
  __IO uint8_t PMUX[16];    /**< \brief Offset: 0x30 (R/W  8) Peripheral Multiplexing n */
  __IO uint8_t PINCFG[32];  /**< \brief Offset: 0x40 (R/W  8) Pin Configuration n */
       uint8_t Reserved2[0x20];
};

#endif