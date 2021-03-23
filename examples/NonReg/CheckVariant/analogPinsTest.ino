/*
   Check Analog pins
   Return true in case of test passed
   Return false in case of test failed
*/
bool checkAnalogPins(void) {
  bool testPassed = true;

  Serial.println("#####");
  Serial.println("Checking analog pins definition...");

  for ( uint32_t i = 0; i < (NUM_ANALOG_INPUTS); i++) {
    uint8_t res = 0;
    // i is the index in the analogInputPin array
    uint32_t pinnum_array = analogInputPin[i];
    uint32_t pinnum_aTD_i = analogInputToDigitalPin(i);
    uint32_t pinnum_aTD_Ax = analogInputToDigitalPin(A0 + i);

    PinName pn_dTpn_Ax = digitalPinToPinName(A0 + i);
    PinName pn_aTpn_Ax = analogInputToPinName(A0 + i);


    if ((pinnum_array != pinnum_aTD_i) || (pinnum_array != pinnum_aTD_Ax) || (pinnum_aTD_i == NC)) {
      res = 1;
    }
    if (!digitalpinIsAnalogInput(pinnum_array) || !digitalpinIsAnalogInput(pinnum_aTD_i) || !digitalpinIsAnalogInput(pinnum_aTD_Ax)) {
      res |= 2;
    }
    if ((pn_dTpn_Ax != pn_aTpn_Ax) || (pinnum_aTD_i == NC)) {
      res |= 4;
    }
    if (digitalPinToAnalogInput(pinnum_aTD_i) != i) {

      res |= 8;
    }

    if (res) {
      Serial.printf("A%i defined as %i with pin name: ", i, A0 + i);
      printPinName(pn_aTpn_Ax, true, true, false);
      Serial.print(" and pin number: ");
      printPinName(pn_aTpn_Ax, false, true, false);
      Serial.printf(" --> %i\n", res);
      Serial.printf(" --> digitalPinToAnalogInput(%i) = %i\n", pinnum_aTD_i, digitalPinToAnalogInput(pinnum_aTD_i));
      testPassed = false;
    }
  }
  Serial.println("End check analog pins");
  return testPassed;
}
