#ifndef ARCH_ARMV7EM_H
#define ARCH_ARMV7EM_H



#include <periph/regs/nvic.h>
#include <periph/regs/systick.h>



/* Memory mapping of Cortex-M4 Hardware */


//#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address  */
//#define ITM_BASE            (0xE0000000UL)                            /*!< ITM Base Address                   */
//#define DWT_BASE            (0xE0001000UL)                            /*!< DWT Base Address                   */
//#define TPI_BASE            (0xE0040000UL)                            /*!< TPI Base Address                   */
//#define CoreDebug_BASE      (0xE000EDF0UL)                            /*!< Core Debug Base Address            */
//#define SYSTICK_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address               */
//#define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address                  */
//#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address  */
//
//#ifndef STM32G4XX
//#define SysTick_BASE        SYSTICK_BASE                   /*!< SysTick Base Address               */
//
//#define SCnSCB              ((SCnSCB_Type    *)     SCS_BASE      )   /*!< System control Register not in SCB */
//#define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct           */
//#define SYSTICK ((struct systick_regs*) SYSTICK_BASE)
//#define SysTick ((SysTick_Type   *)     SysTick_BASE  )   /*!< SysTick configuration struct       */
//#define NVIC    ((struct nvic_regs   *) NVIC_BASE     )   /*!< NVIC configuration struct          */
//#define ITM       ((ITM_Type       *)     ITM_BASE      )   /*!< ITM configuration struct           */
//#define DWT       ((struct dwt_regs *)     DWT_BASE      )   /*!< DWT configuration struct           */
//#define TPI       ((TPI_Type       *)     TPI_BASE      )   /*!< TPI configuration struct           */
//#define CoreDebug ((struct coredebug_regs *)     CoreDebug_BASE)   /*!< Core Debug configuration struct    */
//#define SCB       ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct           */
//#endif

///* Memory mapping of Cortex-M4 Hardware */
//#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address  */
//#define ITM_BASE            (0xE0000000UL)                            /*!< ITM Base Address                   */
//#define DWT_BASE            (0xE0001000UL)                            /*!< DWT Base Address                   */
//#define TPI_BASE            (0xE0040000UL)                            /*!< TPI Base Address                   */
//#define CoreDebug_BASE      (0xE000EDF0UL)                            /*!< Core Debug Base Address            */
//#define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address               */
//#define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address                  */
//#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address  */
//
//#define SCnSCB              ((SCnSCB_Type    *)     SCS_BASE      )   /*!< System control Register not in SCB */
//#define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct           */
//#define SysTick             ((SysTick_Type   *)     SysTick_BASE  )   /*!< SysTick configuration struct       */
//#define NVIC                ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct          */
//#define ITM                 ((ITM_Type       *)     ITM_BASE      )   /*!< ITM configuration struct           */
//#define DWT                 ((DWT_Type       *)     DWT_BASE      )   /*!< DWT configuration struct           */
//#define TPI                 ((TPI_Type       *)     TPI_BASE      )   /*!< TPI configuration struct           */
//#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)   /*!< Core Debug configuration struct    */
//
//#if (__MPU_PRESENT == 1)
//  #define MPU_BASE          (SCS_BASE +  0x0D90UL)                    /*!< Memory Protection Unit             */
//  #define MPU               ((MPU_Type       *)     MPU_BASE      )   /*!< Memory Protection Unit             */
//#endif
//
//#if (__FPU_PRESENT == 1)
//  #define FPU_BASE          (SCS_BASE +  0x0F30UL)                    /*!< Floating Point Unit                */
//  #define FPU               ((FPU_Type       *)     FPU_BASE      )   /*!< Floating Point Unit                */
//#endif

/*@} */


#endif