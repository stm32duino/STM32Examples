/*
   This sketch check:
    - digital and analog pins defined in the variant
    - peripheral instances associated  to wire (I2C), serial (UART) and SPI
*/

#include "utils.h"

#if !defined(STM32_CORE_VERSION) || (STM32_CORE_VERSION  <= 0x01090000)
#error "This sketch is not compatible with core version prior to 2.0.0"
#endif

#define PORTx(pn)   (char)('A' + STM_PORT(pn))
#define PINx(pn)    STM_PIN(pn)

#ifndef LED_BUILTIN
#define LED_BUILTIN PNUM_NOT_DEFINED
#endif

/*
   Initial check of Serial to be sure we can further print on serial
   Returns true in case of test passed
   Returns false in case of test failed
*/
bool checkSerial(void) {
  bool testPassed = true;
#if defined(PinMap_UART_RX) && defined(PinMap_UART_TX)
  USART_TypeDef *uart_rx = (USART_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_SERIAL_RX), PinMap_UART_RX);
  USART_TypeDef *uart_tx = (USART_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_SERIAL_TX), PinMap_UART_TX);
  if (uart_rx == NP) {
    /* PIN_SERIAL_RX (%d) doesn't match a valid UART peripheral */
    testPassed = false;
  }
  if (uart_tx == NP) {
    /* PIN_SERIAL_TX doesn't match a valid UART peripheral */
    testPassed = false;
  }
  if (uart_rx != uart_tx) {
    /* PIN_SERIAL_RX (%d) doesn't match PIN_SERIAL_TX peripheral */
    testPassed = false;
  }
#endif
  return testPassed;
}

/*
   Prints Pin name
   pname: Pin of type PinName (PY_n)
   asPN: true display as a PinName, false as a pin number (PYn)
    val: display value or not
    ln: carriage return or not
*/
void printPinName(PinName pname, bool asPN, bool val, bool ln) {
  Serial.print("P");
  Serial.print(PORTx(pname));
  if (asPN) {
    Serial.print("_");
  }
  Serial.print(PINx(pname));
  if (val) {
    Serial.print(" (");
    Serial.print(asPN ? (uint32_t)pname : pinNametoDigitalPin(pname));
    Serial.print(")");
  }
  if (ln) {
    Serial.println();
  }
}

void setup() {
  /* Check first whether Serial is valid and we can further print on Serial */
  if (!checkSerial()) {
    uint32_t blinkDelay = 200;
    while (1) {
      /* blink led quickly and forever in case of error */
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
      delay(blinkDelay);
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
      delay(blinkDelay);
    }
  }

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  bool testPassed = true;
  String testStatus;
  uint32_t blinkDelay;

  Serial.println("#####");
  Serial.printf("NUM_DIGITAL_PINS = %i\n", NUM_DIGITAL_PINS);
  Serial.printf("NUM_ANALOG_INPUTS = %i\n", NUM_ANALOG_INPUTS);

  /* Run the different tests */
  if (!checkDigitalPins()) {
    testPassed = false;
  }
  if (!checkAnalogPins()) {
    testPassed = false;
  }
  if (!checkIPInstance()) {
    testPassed = false;
  }

  /* Display test result */
  if (testPassed) {
    testStatus = "PASSED";
    blinkDelay = 1000;
  } else {
    testStatus = "FAILED";
    blinkDelay = 200;
  }
  Serial.println("");
  Serial.println("########################################");
  Serial.printf("#### Test %s\n", testStatus.c_str());
  Serial.println("########################################");

  /* Blink Led forever, slowly for test passed, and quickly for test failed */
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(blinkDelay);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(blinkDelay);
  }
}
