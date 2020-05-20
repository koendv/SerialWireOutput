#include <SWOStream.h>

/* 
 * Arduino class to write to Serial Wire Output (SWO)
 * 2020 Koen De Vleeschauwer
 * No copyright asserted.
 */

/* See orbunculum, https://github.com/orbcode/orbuculum,
 * and bmtrace, https://github.com/compuphase/Black-Magic-Probe-Book
 */

SWOStream::SWOStream(uint32_t swoSpeedBaud, swoProtocolType swoProtocol, uint32_t swoChannel, uint32_t cpuClockFreqHz) {

  channel = 0;

  /* Don't initialize SWO if baudrate == 0 */
  if (swoSpeedBaud == 0) return;

  if ((swoChannel < 0) || (swoChannel > 31)) swoChannel = 0;
  channel = swoChannel;

  /* when using Async code, clock equals baudrate. 
   * When using Manchester code, clock equals double the baudrate.
   */

  if (swoProtocol == SWO_Manchester) swoSpeedBaud = 2 * swoSpeedBaud;
  uint32_t swoPrescaler = (cpuClockFreqHz / swoSpeedBaud) - 1;

  /* SWO initialisation */

#if defined(STM32F0xx) && !defined(STM32F042x6) && !defined(STM32F048xx)
  /* for STM32F03x, STM32F05x, STM32F07x, STM32F09x; not for STM32F04x */
  DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
#elif defined(STM32F1xx)
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; /* enable AFIO access */
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1; /* disable JTAG to release TRACESWO */
  DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN; /* enable IO trace pins */
#elif defined(STM32F2xx) || defined(STM32F3xx)
  DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
#elif defined(STM32F4xx) || defined(STM32F7xx)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; /* enable GPIOB clock */
  GPIOB->MODER = (GPIOB->MODER & ~0x000000c0) | 0x00000080; /* alternate func for PB3 */
  GPIOB->AFR[0] &= ~0x0000f000; /* set AF0 (==TRACESWO) on PB3 */
  GPIOB->OSPEEDR |= 0x000000c0; /* set max speed on PB3 */
  GPIOB->PUPDR &= ~0x000000c0; /* no pull-up or pull-down on PB3 */
  DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN; /* enable IO trace pins */
#elif defined(STM32G0xx)
#elif defined(STM32G4xx)
#elif defined(STM32H7xx)
#elif defined(STM32L0xx)
#elif defined(STM32L1xx)
#elif defined(STM32L4xx)
#elif defined(STM32MP1xx)
#elif defined(STM32WBxx)
#endif

  CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk; /* trace enable */
  TPI->CSPSR = 1; /* protocol width = 1 bit */
  TPI->SPPR = swoProtocol; /* 1 = Manchester, 2 = Asynchronous */
  TPI->ACPR = swoPrescaler; /* frequency */
  TPI->FFCR = 0; /* turn off formatter, discard ETM output */
  ITM->LAR = 0xC5ACCE55; /* unlock access to ITM registers */
  ITM->TCR = ITM_TCR_SWOENA_Msk | ITM_TCR_ITMENA_Msk; /* trace control register */
  ITM->TPR = 0; /* all ports accessible unprivileged */
  ITM->TER = 0xFFFFFFFF; /* enable all stimulus channels */
  
};

SWOStream::~SWOStream() {
}

static uint32_t swo_bytes_written = 0;

void SWOStream::setChannel(uint32_t swoChannel) {
  if ((swoChannel < 0) || (swoChannel > 31)) swoChannel = 0;
  channel = swoChannel;
}

/* print on traceswo console */

size_t SWOStream::write(uint8_t ch) {
  if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & 1UL<<channel) != 0UL)) { /* enabled */
    while (ITM->PORT[channel].u32 == 0UL);
    ITM->PORT[channel].u8 = ch;
    swo_bytes_written++;
  }
  return 1;
}

size_t SWOStream::write(const uint8_t *buffer, size_t size) {
  if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & 1UL<<channel) != 0UL)) { /* enabled */
    size_t n = size;
    while (n > 3) {
      uint32_t buf32;
      memcpy(&buf32, buffer, sizeof(buf32));
      while (ITM->PORT[channel].u32 == 0UL);
      ITM->PORT[channel].u32 = buf32;
      buffer += 4;
      n -= 4;
    }
    if (n > 1) {
      uint16_t buf16;
      memcpy(&buf16, buffer, sizeof(buf16));
      while (ITM->PORT[channel].u32 == 0UL);
      ITM->PORT[channel].u16 = buf16;
      buffer += 2;
      n -= 2;
    }
    while (n > 0) {
      while (ITM->PORT[channel].u32 == 0UL);
      ITM->PORT[channel].u8 = *buffer;
      buffer++;
      --n;
    }
    swo_bytes_written+=size;
  }
  return (size);
}

void SWOStream::flush() {
  /*
   * write null bytes to flush input buffers. typical buffer size is 64 bytes.
   * bmp feature: a write to channel 31 flushes output.
	 */

  uint32_t curr_channel = channel;
  channel = 31;
  while(swo_bytes_written & 0x3f)
    SWOStream::write('\0');
  channel = curr_channel;
};

/* not truncated */
