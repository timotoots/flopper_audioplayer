# Flopper Audio Player for ESP32 Audio Kit

## Hardware requirements

*  Ai Thinker A1S Wi-Fi+BT Audio Development Kit
https://www.seeedstudio.com/Ai-Thinker-A1S-Wi-Fi-BT-Audio-Development-Kit-p-3160.html

* NFC Reader PN532

## Set up development environment

* Install Ai Thinker's version of ESP-ADF for "Ai Thinker A1S"
https://github.com/donny681/esp-adf


* Take note of compatible IDF version in Github

* Install compatible ESP-IDF version
https://docs.espressif.com/projects/esp-idf/en/stable/get-started/

* Clone repository
git@github.com:timotoots/flopper_audioplayer.git

* Install Arduino ESP32 as ESP-IDF component
https://github.com/espressif/arduino-esp32/blob/master/docs/esp-idf_component.md
compiles with commit d06fa8ee70dd2541aada77007fb08bedc941323e
 
## Install following Arduino libraries to "components/arduino/libraries"

* for Wifi Configuration portal
https://github.com/Hieromon/AutoConnect
https://github.com/Hieromon/PageBuilder
https://github.com/bblanchon/ArduinoJson

* for missing ESP32 Analogwrite
https://github.com/ERROPiX/ESP32_AnalogWrite

* for NFC NDEF
https://github.com/don/Ndef

* for NFC PN532
https://github.com/Seeed-Studio/PN532

## Build code

* make menuconfig

	* setup USB device location

* make flash monitor
