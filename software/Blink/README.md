# Blink

Arduino demo, modified to loop over 8 pairs of
anti-parallel-connected LEDs,
blinking each one on for 1ms and off for 99ms
before moving on to the next.


## How to build and flash the software on the chip

### What to get

- [Arduino IDE](https://www.arduino.cc/en/software/#ide)

- WCH compiler

  First, add the unofficial WCH repo: In Arduino IDE > File > Preferences > Additional Board Manager URLs > Click the 'Edit' icon on the far right side of the IDE (It looks like a copy button)
  Always double check the JSON by clicking 'Click for a list of unofficial board support URLs', but the .json link is listed below:
  https://raw.githubusercontent.com/openwch/board_manager_files/main/package_ch32v_index.json

  You must install v1.0.0 first or you will run into errors. After that, install v1.0.4. (If you run into issues, run the IDE with elevated privs)

  within Arduino IDE, select Tools / Board / Boards Manager

  search for "wch"

  install "CH32 MCU EVT Boards"

- [WCH-LinkUtility](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html)
 programming tool

   (works on Linux under WINE)

- [WCH-LinkE](https://www.aliexpress.us/item/3256804695267285.html)
 USB programmer

- 3-pin 1.27mm spacing
 [pogo-pin PCB clip](https://www.aliexpress.us/item/3256806702127807.html)

- some Dupont-connector jumper wires

### What to do

- open this modified Blink sketch in Arduino environment, and compile it by clicking leftmost button "Verify"

  find the generated bin file (e.g. /home/user/.cache/arduino/sketches/40EA08CCBD0DF8AB898F31F63E3E5F1E/Blink.ino.bin)
                              (e.g. C:\Users\USER\AppData\Local\arduino\sketches\40C10A2A9890BEF7BEAD8C7F314D8C2A/sketch_[DATE].ino.bin)

- connect 3V, GND, and SWDIO pins on WCH-LinkE device to 3 pins on PCB clip,
 from right to left respectively, so the pogo pins can contact the pads on the
 back face of the coin (the back of the PCB with components mounted)

- plug WCH-LinkE in to USB port on PC

- run WCH-LinkUtility

  load the bin file (see above) using first button "Choose"
  connect to WCH-LinkE using second button "Connect" (should soon see status message similar to "Connected RISC-V mode WCH-Link Cnt:0")

  select Core "RISC-V"

  select Series "CH32V003"

- in "Operation" section

  check "Erase All", "Program", "Verify", "Reset and Run"

- in "Operation Configuration" section

  select "Disable MCU Code Read-Protect"

  select from dropdown CLK Speed "High"

- in "MCU Configuration" section

  check "Disable Standby-Mode RST" (Note: Need to clarify whether we're unchecking 'Starting From the Boot Area')

  select from dropdown "Disable mul-func,PD7 is used for IO function"

### then for each coin

- connect PCB clip to coin

  verify connection using third button "Query Chip Info"
 (should soon see status messages "Begin to set chip type...", "Begin to get chip version...", "Begin to get chip UID & flash size...")
")

  program chip using fourth button "Execute"
 (should soon see status messages "Begin to set chip type...", "Begin to Erase...", "Begin to Program and Verify...", "Begin to Reset...", "Operation is Successful")

  sit back und watchen das blinkenlights
