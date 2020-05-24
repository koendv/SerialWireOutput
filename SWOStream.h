#ifndef SWOSTREAM_H
#define SWOSTREAM_H

/*
 * Arduino class to write to Serial Wire Output (SWO)
 * 2020 Koen De Vleeschauwer
 * No copyright asserted.
 */

#include <Print.h>
#include <stm32_def.h>

typedef enum {
  SWO_Manchester = 0x1,
  SWO_Async = 0x2,
} swoProtocolType;

class SWOStream : public Print {
private:
  uint32_t channel;

public:
  SWOStream(uint32_t swoSpeedBaud, swoProtocolType swoProtocol = SWO_Async,
            uint32_t swoChannel = 0, bool swoEnable = true,
            uint32_t cpuClockFreqHz = SystemCoreClock);
  ~SWOStream();

  void setChannel(uint32_t swoChannel);
  void enable(bool swoEnable);
  virtual size_t write(uint8_t ch);
  virtual size_t write(const uint8_t *buffer, size_t size);
  virtual void flush();
};
#endif

/* not truncated */
