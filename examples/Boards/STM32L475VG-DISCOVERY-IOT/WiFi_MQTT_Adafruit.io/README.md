# WiFi_MQTT_Adafruit.io

 This example demonstrates the capabilities of the [PubSubClient](https://github.com/knolleary/pubsubclient) library in combination
 with the [B-L475E-IOT01A](http://www.st.com/en/evaluation-tools/b-l475e-iot01a.html) board to publish on https://io.adafruit.com broker

 See https://learn.adafruit.com/mqtt-adafruit-io-and-you/overview

# Dependencies

Install the following libraries using the Arduino IDE: **_Sketch -> Include Library -> Manage Libraries_** and search:

  * [PubSubClient](https://github.com/knolleary/pubsubclient): Arduino Client for MQTT.
  * [STM32duino ISM43362-M3G-L44](https://github.com/stm32duino/WiFi-ISM43362-M3G-L44): WiFi library for the [B-L475E-IOT01A](http://www.st.com/en/evaluation-tools/b-l475e-iot01a.html) board.
  * [STM32duino HTS221](https://github.com/stm32duino/HTS221): to support the HTS221 capacitive digital sensor for relative humidity and temperature
  
# Adafruit IO Dashboard.

 You will need to create some feeds in your Adafruit IO Dashboard.

 See https://learn.adafruit.com/mqtt-adafruit-io-and-you/getting-started-on-adafruit-io

 to know how register and create your Adafruit IO dashboard and feeds:
   * hello: Text block
   * onoff: Toggle block
   * temp: Stream block
   * hum: Gauge block

 A screenshot of the dashboard: 
 [Dashboard Adafruit](/img/dashboard_adafruit.png)

  It connects to the Adafruit IO's MQTT server (a.k.a broker) server then:
  * publishes announcement "_Hi, I'm STM32 user!_" to the topic `AIO_USERNAME"/feeds/hello"`
  * subscribes to the topic `AIO_USERNAME"/feeds/onoff"`, switching `LED_BUILTIN` state
  * publishes temperature and humidity from the HTS221 sensor to the topic
    `AIO_USERNAME"/feeds/temp"` and `AIO_USERNAME"/feeds/hum"` every 10 seconds
