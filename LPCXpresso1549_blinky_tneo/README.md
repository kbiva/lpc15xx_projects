Introduction
=
LPCXpresso1549 board and eight LEDs.

One LED is blinking every second.

The other LEDs are blinking with following periods:

50ms,100ms,200ms,400ms,800ms,1600ms and 3200ms.

Using real-time kernel TNeo. Each LED is controlled by separate task (total 8 tasks).

Hardware
=
* LPCXpresso1549 board. (MCU clock is 72Mhz from external oscillator).
* Eight LEDs.

Connections
=
* P0_22 -> resistor -> LED -> 3.3V
* P0_23 -> resistor -> LED -> 3.3V
* P0_16 -> resistor -> LED -> 3.3V
* P0_12 -> resistor -> LED -> 3.3V
* P0_28 -> resistor -> LED -> 3.3V
* P0_27 -> resistor -> LED -> 3.3V
* P0_24 -> resistor -> LED -> 3.3V
* P0_0 -> resistor -> LED -> 3.3V

Software
=
Used peripherals: 

* GPIO.

Project uses external libraries:

* LPCOpen 2.20 library for LPC15xx
* TNeo library
