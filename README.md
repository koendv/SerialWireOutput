# SerialWireOutput
Serial Wire Output library for STM32duino ARM Arduinos

With Serial Wire Output (SWO) an stm32duino can write tracing and logging to a pc at high speed. For SWO to work, you need a debugger probe to connect the program on your arduino to the debugger on your pc. SWO only runs on systems with arm processors, sorry.

# Installation
* install the [arduino IDE](https://www.arduino.cc/)
* install [STM32duino library](https://github.com/stm32duino/Arduino_Core_STM32)
* install [SerialWireOutput](https://github.com/koendv/SerialWireOutput) (this library)
* connect a debugger probe with SWO support, e.g. a [segger](https://www.segger.com/products/debug-probes/j-link/) or a [black magic probe](https://github.com/blacksphere/blackmagic/wiki). Connect the SWO output of the target with the SWO input of the probe.
* start tool to view the SWO stream.
* run example File→Examples→SerialWireOutput→HelloWorld

# Connection Diagram
![Connection Diagram](https://raw.githubusercontent.com/koendv/Connecting-Black-Magic-Probe-and-Blue-Pill/master/bmp_bp.svg  "Connecting a Blue Pill to a Black Magic Probe")

This is one way to connect a STM32F103 Blue Pill with a Black Magic Probe. This allows downloading firmware and debugging using SWD. At the same time, SWO provides high speed debugging output.

Take care to connect only one power source to a Blue Pill board; feeding power through usb and a power pin at the same time may cause short circuit.

# Arduino Use
Use SWOStream where you would use Serial:
```
#include <SWOStream.h>
SWOStream s;
void setup() {
}
void loop() {
  s.print("hello world! ");
}
```

In the Arduino IDE, choose *Tools→Upload Method→BMP (Black Magic Probe)* and *Tools→Optimize→Debug (-g)*.

When using e.g. Black Magic Probe you typically would use commands like:
```
koen@raspberrypi:~ $ gdb -q
(gdb) target extended-remote /dev/ttyBmpGdb
(gdb) monitor swdp_scan
Available Targets:
No. Att Driver
 1      STM32F1 medium density M3/M4
(gdb) attach 1
(gdb) file /tmp/arduino_build_195867/HelloWorld.ino.elf
(gdb) load
(gdb) monitor traceswo 2250000
(gdb) set mem inaccessible-by-default off
(gdb) run
```
# Capturing the SWO stream

Your typical debugger comes with tool a tool to capture the SWO stream.

The commercial Jlink comes with [SWO viewer](https://www.segger.com/products/debug-probes/j-link/tools/j-link-swo-viewer/).

The open source Black Magic Probe has the ``bmtrace`` SWO viewer:

![bmtrace](https://github.com/compuphase/Black-Magic-Probe-Book/raw/master/doc/bmtrace.png  "bmtrace SWO viewer for Black Magic Probe")

Companion utilities are the ``bmdebug``  debugger and the ``bmflash`` flashing tool. These tools are open source, like the Black Magic Probe itself. Download the [tools](https://github.com/compuphase/Black-Magic-Probe-Book/releases) and the [manual](https://github.com/compuphase/Black-Magic-Probe-Book/raw/master/BlackMagicProbe.pdf).

Instead of using an external viewer to do the decoding, Black Magic Probe can also output a decoded SWO stream. In the device manager, Black Magic Probe shows up as two COM ports. On Windows , connect [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) to the second of the two COM ports, labeled "Black Magic UART Port". On linux, type ``cat /dev/ttyBmpTarg``. In another window, at the gdb prompt, type
```
(gdb) monitor traceswo 2250000 decode
```
to decode a 2250000 baud SWO stream. The decoded SWO stream is sent to the serial port and displayed by PuTTY.

## SWOStream
The SWOStream Arduino class allows you to write tracing and logging to the PC where the debugger is running. Setup parameters are:

**baudRate** Set baud rate.

If the baud rate is zero, the SWOStream class will not set up the SWO pin. This can be used if the debugger, SWO viewer, or another SWOStream has already set up SWO.

On a STM32F103-based Black Magic Probe, baud rate has to be 4500000 divided by an integer: 4500000, 2250000,  1500000,  1125000, 900000, 750000, ... . On a bmp clone, 2250000 is a safe choice.

There is no default value. Check what baud rates your debugger supports.

**swoProtocol** Protocol is *SWO_Manchester* or *SWO_Async* (asynchronous). Default value is *SWO_Async* .

**swoChannel** a number between 0 and 31, used to distinguish different SWO streams. The default is 0, a safe choice.

**cpuClockFrequency** Set this if your board has a non-standard crystal.

# Acks
Thanks to [orbuculum](https://github.com/orbcode/orbuculum), and the [Black Magic Probe book](https://github.com/compuphase/Black-Magic-Probe-Book).

If you find errors, omissions, or have additional data, feel free to open an issue.
