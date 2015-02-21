# OlympicSword
Arduino WS2812B LED Strip display for Pentathlon in 2015 Sarasota FL USA

## Design
We used Arduino Mini Pro 5V (ATmega328p) to drive 3 sets of 3 meter LED lights.  For simplicity, the three strings were connected in parallel so the LED sequency would show on all three strings, logically driving one LED strip.

The "sword" is actually a 3X model of an Epee fencing sword.  The blade was made of PVC that was 270 cm long.  It had 3 LED strips along the sides driven with the same data output from the arduino.

It has 3 modes of operation:
.. 1. Idle, with no lights displayed.
.. 2. Slow, with slower animation of colors and action.
.. 3. Fast, with fast action, strobes and chase lights.

The Arduino would enter idle state until sensing 5V input from one of two pins.  Each pin was assigned to Slow or Fast mode.

## Implementation
We used a case, with both 5V and 12V power supplies.  The 5V powers the Arduino and LED strips.  The 12V was used to power a RF remote controlled relay board used to toggle other effects (smoke machine, twinkling lights, bubble machine, laser lights) along with inputs to the Arduino.

## Parts Used

* DC 12V 15-Channel 500M Wireless Remote Control Switch Module Controller Set(http://www.amazon.com/15-Channel-Wireless-Remote-Control-Controller/dp/B00EZISJCE)

* Arduino Pro Mini (http://www.ebay.com/itm/Arduino-PRO-MINI-ATMEGA328-5V-16M-MWC-avr328P-Development-Board-/271251071444)
 

## Software Used

This work is based on the excellent NeoPixel library from AdaFruit:
NeoPixel Library (https://github.com/adafruit/Adafruit_NeoPixel)


## Things learned (stuff to fix)

* The color wheel sequences run and don't check the inputs.  This means the relay could turn off but the program sequence wouldn't end right away.  A solution would be to check the input inside the loop to see if we would break out and end the program sequence.


## Next Version

* Have more modes and inputs.  Perhaps use 'chord' combinations for colors, sequences and speeds.  For example, use A0,A1 to select sequence, and A2,A3 to select colors, and A5 for start/stop control.

* Better yet, use the serial input to accept 3 byte codes from a Raspberry Pi or another Arduino connected wireless to the operator.  Pass 1 byte for sequence (C=chase, W=color wheel, S=strobe, etc.) another for color, and another for speed.






