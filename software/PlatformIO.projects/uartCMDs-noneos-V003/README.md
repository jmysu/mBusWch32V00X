How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/Community-PIO-CH32V/platform-ch32v/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Change directory to example
$ cd platform-ch32v/examples/blinky-none-os

# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Upload firmware for the specific environment
$ pio run -e ch32v307_evt --target upload

# Clean build files
$ pio run --target clean
```
uartCMDs i2c OLED<br>
<img src="https://github.com/jmysu/mBusWch32V00X/blob/main/pic/PXL_20241105_OLED.jpg" width="25%"> <br>
<br>
<br>
uartCMDs Commander V1.1 w/ i2c scan... <br>
<img src="https://github.com/jmysu/mBusWch32V00X/blob/main/pic/uartCMDSV1.1.jpg" width="50%"> <br>

uartCMDs Commander V1.1 w/ enabled modules <br>
<img src="https://github.com/jmysu/mBusWch32V00X/blob/main/pic/uartCMDsV1.1_modules.jpg" width="65%"> <br>
<br>

uartCMDs left I2CSlave @0x09, right master scan <br>
<img src="https://github.com/jmysu/mBusWch32V00X/blob/main/pic/uartCMDsScanSlave.jpg"> <br>
