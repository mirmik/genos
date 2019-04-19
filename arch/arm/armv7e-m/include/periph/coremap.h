#ifndef ARCH_ARMV7EM_H
#define ARCH_ARMV7EM_H

#include <periph/regs/nvic.h>
#include <periph/regs/systick.h>

/* Memory mapping of Cortex-M4 Hardware */
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address  */
#define ITM_BASE            (0xE0000000UL)                            /*!< ITM Base Address                   */
#define DWT_BASE            (0xE0001000UL)                            /*!< DWT Base Address                   */
#define TPI_BASE            (0xE0040000UL)                            /*!< TPI Base Address                   */
#define CoreDebug_BASE      (0xE000EDF0UL)                            /*!< Core Debug Base Address            */
#define SYSTICK_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address               */
#define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address                  */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address  */

//#define SCnSCB              ((SCnSCB_Type    *)     SCS_BASE      )   /*!< System control Register not in SCB */
//#define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct           */
#define SYSTICK ((struct systick_regs*) SYSTICK_BASE)
#define NVIC    ((struct nvic_regs   *) NVIC_BASE     )   /*!< NVIC configuration struct          */
//#define ITM                 ((ITM_Type       *)     ITM_BASE      )   /*!< ITM configuration struct           */
//#define DWT                 ((DWT_Type       *)     DWT_BASE      )   /*!< DWT configuration struct           */
//#define TPI                 ((TPI_Type       *)     TPI_BASE      )   /*!< TPI configuration struct           */
//#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)   /*!< Core Debug configuration struct    */


#endif