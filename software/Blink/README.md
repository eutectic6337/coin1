# Blink

Arduino demo, modified to loop over 8 pairs of
anti-parallel-connected LEDs,
blinking each one on for 1ms and off for 99ms
before moving on to the next.


## How to build and flash the software on the chip

You will need:

- [Arduino IDE](https://www.arduino.cc/en/software/#ide)

- WCH compiler

  within Arduino IDE, select Tools / Board / Boards Manager

  search for "wch"

  install "CH32 MCU EVT Boards"

- [WCH-Link](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html)
 utility program

- [WCH-LinkE](https://www.aliexpress.us/item/3256804695267285.html)
 USB programmer

- 3-pin 1.27mm spacing
 [pogo-pin PCB clip](https://www.aliexpress.us/item/3256806702127807.html)
