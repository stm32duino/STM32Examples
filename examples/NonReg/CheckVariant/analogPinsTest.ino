
void checkAnalogPins(void) {
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
    /*
       pname: Pin of type PinName (PY_n)
       asPN: true display as a PinName, false as a pin number (PYn)
       val: display value or not
       ln: carriage return or not
    */
    if (res) {
      Serial.printf("A%i defined as %i with pin name: ", i, A0 + i);
      printPinName(pn_aTpn_Ax, true, true, false);
      Serial.print(" and pin number: ");
      printPinName(pn_aTpn_Ax, false, true, false);
      Serial.printf(" --> %i\n", res);
      Serial.printf(" --> digitalPinToAnalogInput(%i) = %i\n", pinnum_aTD_i, digitalPinToAnalogInput(pinnum_aTD_i));
      

      //      Serial.print("\tPin name ");
      //      printPinName(pn_Ax, true, true, false);
      //      Serial.print(" have to be indexed higher than NUM_ANALOG_INPUTS (");
      //      Serial.print(NUM_ANALOG_INPUTS);
      //      Serial.println(") to be able to use:");
      //      PinName pn_Ax_analog = analogInputToPinName(pinNametoDigitalPin(pn_Ax));
      //      Serial.print("\tAnalogRead(");
      //      printPinName(pn_Ax, false, false, false);
      //      Serial.print(") -->  which currently use pin name: ");
      //      printPinName(pn_Ax_analog, true, true, false);
      //      Serial.print(" and pin number: ");
      //      printPinName(pn_Ax_analog, false, true, true);
    }
  }
  Serial.println("End check analog pins");
  Serial.println("#####");
}
