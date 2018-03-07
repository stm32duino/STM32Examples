/*
 STM32F746G-DISCOVERY MQTT example to publish on https://io.adafruit.com broker

 See https://learn.adafruit.com/mqtt-adafruit-io-and-you/overview

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the STM32F746G-DISCOVERY board.

 It will use several components of the board and you need to install corresponding libraries :
   - Ethernet (STM32Ethernet) : https://github.com/stm32duino/STM32Ethernet
   - Temperature and humidity sensor (HTS221) : https://github.com/stm32duino/HTS221
   - Arduino Client for MQTT: https://github.com/knolleary/pubsubclient

 To get temperature and humidity you will need the X-NUCLEO-IKS01A1, a motion MEMS and
 environmental sensor expansion board for the STM32 board.

 You can find more information on the board and exapansion board here :
   http://www.st.com/en/evaluation-tools/32f746gdiscovery.html
   http://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-iks01a1.html)

 You will need to create some feeds in your Adafruit IO Dashboard.
 See https://learn.adafruit.com/mqtt-adafruit-io-and-you/getting-started-on-adafruit-io
 to know how register and create your Adafruit IO dashboard and feeds:
   - hello: Text block
   - onoff: Toggle block
   - temp: Stream block
   - hum: Gauge block
 A screenshot of the dashboard is available in the img/ directory: dashboard_adafruit.png

 It connects to the Adafruit IO's MQTT server (a.k.a broker) server then:
  - publishes announcement "Hi, I'm STM32 user!" to the topic 'AIO_USERNAME"/feeds/hello"'
  - subscribes to the topic AIO_USERNAME"/feeds/onoff"', switching LED_BUILTIN state
  - publishes temperature and humidity from the HTS221 sensor to the topic
    'AIO_USERNAME"/feeds/temp"' and 'AIO_USERNAME"/feeds/hum"' every 10 seconds

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

*/
#include <LwIP.h>
#include <STM32Ethernet.h>
#include <PubSubClient.h>
#include <HTS221Sensor.h>

// Adafruit.io Setup
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "......"
#define AIO_KEY         "........"

// Etherrnet setup
// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
EthernetClient STClient;

// i2c sensors
HTS221Sensor  *HumTemp;

PubSubClient client(STClient);
long lastMsg = 0;
char msg[8];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);

  client.setServer(AIO_SERVER, AIO_SERVERPORT);
  client.setCallback(callback);

  // Start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // Allow the hardware to sort itself out
  delay(1500);

  // Initialize I2C bus.
  Wire.begin();

  // Initlialize components.
  HumTemp = new HTS221Sensor (&Wire);
  HumTemp->Enable();
}

void callback(char* topic, byte* payload, unsigned int length) {

#if 0
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
#else
  UNUSED(topic);
#endif
  if (length > 1) {
    // Switch on/off the LED (payload messages can be 'ON' or 'OFF')
    if ((char)payload[1] == 'N') {
      digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
    } else {
      digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // Note - the default maximum packet size is 128 bytes. If the
    // combined length of clientId, username and password exceed this,
    // you will need to increase the value of MQTT_MAX_PACKET_SIZE in
    // PubSubClient.h
    if (client.connect("STM32Client", AIO_USERNAME, AIO_KEY)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(AIO_USERNAME"/feeds/hello", "Hi, I'm STM32 user!");
      // ... and resubscribe
      client.subscribe(AIO_USERNAME"/feeds/onoff");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  float temperature, humidity;
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    HumTemp->GetTemperature(&temperature);
    dtostrf(temperature, 2, 2, msg);
    Serial.print("Publish temperature ");
    Serial.println(msg);
    client.publish(AIO_USERNAME"/feeds/temp", msg);

    HumTemp->GetHumidity(&humidity);
    snprintf (msg, 8, "%u", (unsigned int)humidity);
    Serial.print("Publish humidity ");
    Serial.println(msg);
    client.publish(AIO_USERNAME"/feeds/hum", msg);
  }
}
