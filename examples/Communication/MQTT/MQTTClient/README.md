# Paho MQTT C client

Required to install prebuilt Arduino port of MQTTClient.

See http://www.eclipse.org/paho/clients/c/embedded/

Download the prebuilt Arduino port of MQTTClient and in the Arduino IDE use
**_Sketch -> Include Library -> Add .ZIP Library..._** with the downloaded client zip file.

# Examples

## Hello_stm32 (original _name Hello_)
This is the basic example provided with the MQTTClient library.
It has been modified to use the [STM32 Ethernet](https://github.com/stm32duino/STM32Ethernet) library.

Only includes have changed:
```
#include <SPI.h>
#include <Ethernet.h>
```
Replaced by:
```
#include <LwIP.h>
#include <STM32Ethernet.h>
```
