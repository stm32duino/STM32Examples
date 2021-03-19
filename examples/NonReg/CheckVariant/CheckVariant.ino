/*
   This sketch check, digital and analog pins defined in the variant
 */

#include "utils.h"

#define PORTx(pn)   (char)('A' + STM_PORT(pn))
#define PINx(pn)    STM_PIN(pn)

/*
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
