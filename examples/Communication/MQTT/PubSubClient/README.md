# PubSubClient

This library provides a client for doing simple publish/subscribe messaging with a server that supports MQTT.

https://github.com/knolleary/pubsubclient

# Dependencies

Install the following libraries using the Arduino IDE: **_Sketch -> Include Library -> Manage Libraries_** and search:

 * [PubSubClient](https://github.com/knolleary/pubsubclient): Arduino Client for MQTT.
 * [STM32duino ISM43362-M3G-L44](https://github.com/stm32duino/WiFi-ISM43362-M3G-L44): WiFi library for the B-L475E-IOT01A board.
 * [STM32 Ethernet](https://github.com/stm32duino/STM32Ethernet): Ethernet library for STM32 based board with on-board Ethernet connector.

# Examples

## mqtt_B-L475E-IOT01A
This example is based on the mqtt_esp8266 provided with PubSubClient library.
It has been modified to use the [STM32duino ISM43362-M3G-L44](https://github.com/stm32duino/WiFi-ISM43362-M3G-L44) WiFi library with [B-L475E-IOT01A](http://www.st.com/en/evaluation-tools/b-l475e-iot01a.html) board

## mqtt_STM32Ethernet
This example is based on the mqtt_basic provided with PubSubClient library.
It has been modified to use the [STM32 Ethernet](https://github.com/stm32duino/STM32Ethernet) Ethernet library with a STM32 based board
with on-board Ethernet connector.
