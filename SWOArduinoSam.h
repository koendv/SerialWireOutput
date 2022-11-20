#ifndef SWOARDUINOSAM_H
#define SWOARDUINOSAM_H

#include <chip.h>

/* For some reason, these definitions are missing from the Arduino SAM core header files. */
/* The SAM3X8 is a Cortex-M3 core. */

/*
 * sources:
 * https://developer.arm.com/documentation/ddi0337/h/trace-port-interface-unit/tpiu-programmers-model?lang=en
 * ARMv7-M Architecture Reference Manual
 */
/** \brief Structure type to access the Trace Port Interface Unit (TPIU). */
typedef struct {
  __I  uint32_t SSPSR; /*!< Offset: 0x000 (R/ )  Supported Parallel Port Sizes Register */
  __IO uint32_t CSPSR; /*!< Offset: 0x004 (R/W)  Current Parallel Port Size Register */
       uint32_t RESERVED0[2]; /* Offsets: 0x008, 0x00C */
  __IO uint32_t ACPR; /*!< Offset: 0x010 (R/W)  Asynchronous Clock Prescaler Register */
       uint32_t RESERVED1[55]; /* Offsets: 0x014 .. 0x0EC */
  __IO uint32_t SPPR; /*!< Offset: 0x0F0 (R/W)  Selected Pin Protocol Register */
       uint32_t RESERVED2[131]; /* Offsets: 0x0F4 .. 0x2FC */
  __I  uint32_t FFSR; /*!< Offset: 0x300 (R/ )  Formatter and Flush Status Register x */
  __IO uint32_t FFCR; /*!< Offset: 0x304 (R/W)  Formatter and Flush Control Register */
  __I  uint32_t FSCR; /*!< Offset: 0x308 (R/ )  Formatter Synchronization Counter Register */
       uint32_t RESERVED3[759]; /* Offsets: 0x30C .. 0xEE4 */
  __I  uint32_t TRIGGER; /*!< Offset: 0xEE8 (R/ )  TRIGGER */
  __I  uint32_t FIFODATA0; /*!< Offset: 0xEEC (R/ )  Integration FIFO 0 Data */
  __I  uint32_t ITATBCTR2; /*!< Offset: 0xEF0 (R/ )  ITATBCTR2 */
       uint32_t RESERVED4[1]; /* Offset: 0xEF4 */
  __I  uint32_t ITATBCTR0; /*!< Offset: 0xEF8 (R/ )  ITATBCTR0 */
  __I  uint32_t FIFODATA1; /*!< Offset: 0xEFC (R/ )  Integration FIFO 1 Data */
  __IO uint32_t ITCTRL; /*!< Offset: 0xF00 (R/W)  Integration Mode Control */
       uint32_t RESERVED5[39]; /* Offsets: 0xF04 .. 0xF9C */
  __IO uint32_t CLAIMSET; /*!< Offset: 0xFA0 (R/W)  Claim Tag Set */
  __IO uint32_t CLAIMCLR; /*!< Offset: 0xFA4 (R/W)  Claim Tag Clear */
       uint32_t RESERVED6[8]; /* Offsets: 0xFA8 .. 0xFC4 */
  __I  uint32_t DEVID; /*!< Offset: 0xFC8 (R/ )  Device ID */
       uint32_t RESERVED7[1]; /* Offset: 0xFCC */
  __I  uint32_t PID4; /*!< Offset: 0xFD0 (R/ )  Peripheral identification register 4 */
  __I  uint32_t PID5; /*!< Offset: 0xFD4 (R/ )  Peripheral identification register 5 */
  __I  uint32_t PID6; /*!< Offset: 0xFD8 (R/ )  Peripheral identification register 6 */
  __I  uint32_t PID7; /*!< Offset: 0xFDC (R/ )  Peripheral identification register 7 */
  __I  uint32_t PID0; /*!< Offset: 0xFE0 (R/ )  Peripheral identification register 0 */
  __I  uint32_t PID1; /*!< Offset: 0xFE4 (R/ )  Peripheral identification register 1 */
  __I  uint32_t PID2; /*!< Offset: 0xFE8 (R/ )  Peripheral identification register 2 */
  __I  uint32_t PID3; /*!< Offset: 0xFEC (R/ )  Peripheral identification register 3 */
  __I  uint32_t CID0; /*!< Offset: 0xFF0 (R/ )  Component identification register 0 */
  __I  uint32_t CID1; /*!< Offset: 0xFF4 (R/ )  Component identification register 1 */
  __I  uint32_t CID2; /*!< Offset: 0xFF8 (R/ )  Component identification register 2 */
  __I  uint32_t CID3; /*!< Offset: 0xFFC (R/ )  Component identification register 3 */
} TPI_Type;


/** \brief Structure type to access some registers conspicuously missing from the ITM (Internal Trace Macrocell) definition. */
typedef struct {
       uint32_t RESERVED0[1004]; /* OFfsets: 0x000 .. 0xFAC */
  __O  uint32_t LAR; /*!< Offset: 0xFB0 ( /W)  Lock Access Register */
  __I  uint32_t LSR; /*!< Offset: 0xFB4 (R/ )  Lock Status Register */
} ITM_Ext_Type;

#define TPI_BASE 0xE0040000         /*!< TPIU base address */

#define TPI     ((TPI_Type *)     TPI_BASE) /*!< TPIU configuration struct */
#define ITM_Ext ((ITM_Ext_Type *) ITM_BASE) /*!< ITM extension configuration struct */

#endif
