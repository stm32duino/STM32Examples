# STM32Examples
Provides several examples for the Arduino core for STM32 MCUs.

For the complete description of each example, please refer to the comments at the beginning of each .ino file
or the README.md file in the sketch folder.

## Boards Examples
* [STM32L475VG-DISCOVERY](http://www.st.com/en/evaluation-tools/b-l475e-iot01a.html):
  * BTLE_sensors_tof_demo:
    * get environmental data and send it via  BlueTooth to your smartphone.
    * use Time of Flight sensor to detect gestures
  * WiFi_MQTT_Adafruit.io:
    * publish environmental data and send via WiFi using MQTT on https://io.adafruit.com 
    * use HTS221 sensor to get temperature and humidity
* [STM32F746G-DISCOVERY](http://www.st.com/en/evaluation-tools/32f746gdiscovery.html):
  * Ethernet_MQTT_Adafruit.io:
    * publish environmental data and send via Ethernet using MQTT on https://io.adafruit.com 
    * use HTS221 sensor of [X-NUCLEO-IKS01A1](http://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-iks01a1.html) expansion board to get temperature and humidity

Feel free to share your examples...
