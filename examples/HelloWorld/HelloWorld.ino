#include <SWOStream.h>

SWOStream s(2250000); // Check what baudrate your debugger supports
int n=0;

void setup() {
}

void loop() {
  s.print(n++);
  s.print(" hello, world! ");
}

/* not truncated */
