
#define PNUM(Port, Pin)   P ## Port ## Pin
#define PNAME(Port, Pin)  P ## Port ## _ ## Pin
#define DECLTESTPNUM(Port, Pin)  checkPinNumber(XSTR(PNAME(Port, Pin)), PNAME(Port, Pin), "P"#Port#Pin, PNUM(Port, Pin))

/*
   Check Pin number
   Return true in case of test passed
   Return false in case of test failed
*/
bool checkPinNumber(const char* spname, PinName pname, const char* spnum, uint32_t pnum) {
  PinName pnamex = digitalPinToPinName(pnum);
  if (pnamex != pname) {
    Serial.print("Pin number: ");
    Serial.printf("%s(%i) indexes PinName:", spnum);
    if (pnamex == NC) {
      Serial.print("NC");
    } else {
      printPinName(pnamex, true, true, false);
    }
    Serial.print(" --> ");
    uint32_t pnumx = pinNametoDigitalPin(pname);
    if (pnumx != NUM_DIGITAL_PINS) {
      Serial.printf("%s should be %i\n", spnum, pnumx);
    } else {
      Serial.printf("%s is not in digitalPin[], so %i should not be defined.\n", spname, spnum);
    }
    return false;
  }
  return true;
}

/*
   Check Digital pins
   Return true in case of test passed
   Return false in case of test failed
*/
bool checkDigitalPins(void) {
  bool testPassed = true;
  Serial.println("#####");
  Serial.println("Checking digital pins...");
  // Serial.println("\tPin number definition...");
#ifdef PA0
  if (!DECLTESTPNUM(A, 0)) testPassed = false;
#endif
#ifdef PA1
  if (!DECLTESTPNUM(A, 1)) testPassed = false;
#endif
#ifdef PA2
  if (!DECLTESTPNUM(A, 2)) testPassed = false;
#endif
#ifdef PA3
  if (!DECLTESTPNUM(A, 3)) testPassed = false;
#endif
#ifdef PA4
  if (!DECLTESTPNUM(A, 4)) testPassed = false;
#endif
#ifdef PA5
  if (!DECLTESTPNUM(A, 5)) testPassed = false;
#endif
#ifdef PA6
  if (!DECLTESTPNUM(A, 6)) testPassed = false;
#endif
#ifdef PA7
  if (!DECLTESTPNUM(A, 7)) testPassed = false;
#endif
#ifdef PA8
  if (!DECLTESTPNUM(A, 8)) testPassed = false;
#endif
#ifdef PA9
  if (!DECLTESTPNUM(A, 9)) testPassed = false;
#endif
#ifdef PA10
  if (!DECLTESTPNUM(A, 10)) testPassed = false;
#endif
#ifdef PA11
  if (!DECLTESTPNUM(A, 11)) testPassed = false;
#endif
#ifdef PA12
  if (!DECLTESTPNUM(A, 12)) testPassed = false;
#endif
#ifdef PA13
  if (!DECLTESTPNUM(A, 13)) testPassed = false;
#endif
#ifdef PA14
  if (!DECLTESTPNUM(A, 14)) testPassed = false;
#endif
#ifdef PA15
  if (!DECLTESTPNUM(A, 15)) testPassed = false;
#endif
#ifdef PB0
  if (!DECLTESTPNUM(B, 0)) testPassed = false;
#endif
#ifdef PB1
  if (!DECLTESTPNUM(B, 1)) testPassed = false;
#endif
#ifdef PB2
  if (!DECLTESTPNUM(B, 2)) testPassed = false;
#endif
#ifdef PB3
  if (!DECLTESTPNUM(B, 3)) testPassed = false;
#endif
#ifdef PB4
  if (!DECLTESTPNUM(B, 4)) testPassed = false;
#endif
#ifdef PB5
  if (!DECLTESTPNUM(B, 5)) testPassed = false;
#endif
#ifdef PB6
  if (!DECLTESTPNUM(B, 6)) testPassed = false;
#endif
#ifdef PB7
  if (!DECLTESTPNUM(B, 7)) testPassed = false;
#endif
#ifdef PB8
  if (!DECLTESTPNUM(B, 8)) testPassed = false;
#endif
#ifdef PB9
  if (!DECLTESTPNUM(B, 9)) testPassed = false;
#endif
#ifdef PB10
  if (!DECLTESTPNUM(B, 10)) testPassed = false;
#endif
#ifdef PB11
  if (!DECLTESTPNUM(B, 11)) testPassed = false;
#endif
#ifdef PB12
  if (!DECLTESTPNUM(B, 12)) testPassed = false;
#endif
#ifdef PB13
  if (!DECLTESTPNUM(B, 13)) testPassed = false;
#endif
#ifdef PB14
  if (!DECLTESTPNUM(B, 14)) testPassed = false;
#endif
#ifdef PB15
  if (!DECLTESTPNUM(B, 15)) testPassed = false;
#endif
#if defined GPIOC_BASE
#ifdef PC0
  if (!DECLTESTPNUM(C, 0)) testPassed = false;
#endif
#ifdef PC1
  if (!DECLTESTPNUM(C, 1)) testPassed = false;
#endif
#ifdef PC2
  if (!DECLTESTPNUM(C, 2)) testPassed = false;
#endif
#ifdef PC3
  if (!DECLTESTPNUM(C, 3)) testPassed = false;
#endif
#ifdef PC4
  if (!DECLTESTPNUM(C, 4)) testPassed = false;
#endif
#ifdef PC5
  if (!DECLTESTPNUM(C, 5)) testPassed = false;
#endif
#ifdef PC6
  if (!DECLTESTPNUM(C, 6)) testPassed = false;
#endif
#ifdef PC7
  if (!DECLTESTPNUM(C, 7)) testPassed = false;
#endif
#ifdef PC8
  if (!DECLTESTPNUM(C, 8)) testPassed = false;
#endif
#ifdef PC9
  if (!DECLTESTPNUM(C, 9)) testPassed = false;
#endif
#ifdef PC10
  if (!DECLTESTPNUM(C, 10)) testPassed = false;
#endif
#ifdef PC11
  if (!DECLTESTPNUM(C, 11)) testPassed = false;
#endif
#ifdef PC12
  if (!DECLTESTPNUM(C, 12)) testPassed = false;
#endif
#ifdef PC13
  if (!DECLTESTPNUM(C, 13)) testPassed = false;
#endif
#ifdef PC14
  if (!DECLTESTPNUM(C, 14)) testPassed = false;
#endif
#ifdef PC15
  if (!DECLTESTPNUM(C, 15)) testPassed = false;
#endif
#endif
#if defined GPIOD_BASE
#ifdef PD0
  if (!DECLTESTPNUM(D, 0)) testPassed = false;
#endif
#ifdef PD1
  if (!DECLTESTPNUM(D, 1)) testPassed = false;
#endif
#ifdef PD2
  if (!DECLTESTPNUM(D, 2)) testPassed = false;
#endif
#ifdef PD3
  if (!DECLTESTPNUM(D, 3)) testPassed = false;
#endif
#ifdef PD4
  if (!DECLTESTPNUM(D, 4)) testPassed = false;
#endif
#ifdef PD5
  if (!DECLTESTPNUM(D, 5)) testPassed = false;
#endif
#ifdef PD6
  if (!DECLTESTPNUM(D, 6)) testPassed = false;
#endif
#ifdef PD7
  if (!DECLTESTPNUM(D, 7)) testPassed = false;
#endif
#ifdef PD8
  if (!DECLTESTPNUM(D, 8)) testPassed = false;
#endif
#ifdef PD9
  if (!DECLTESTPNUM(D, 9)) testPassed = false;
#endif
#ifdef PD10
  if (!DECLTESTPNUM(D, 10)) testPassed = false;
#endif
#ifdef PD11
  if (!DECLTESTPNUM(D, 11)) testPassed = false;
#endif
#ifdef PD12
  if (!DECLTESTPNUM(D, 12)) testPassed = false;
#endif
#ifdef PD13
  if (!DECLTESTPNUM(D, 13)) testPassed = false;
#endif
#ifdef PD14
  if (!DECLTESTPNUM(D, 14)) testPassed = false;
#endif
#ifdef PD15
  if (!DECLTESTPNUM(D, 15)) testPassed = false;
#endif
#endif
#if defined GPIOE_BASE
#ifdef PE0
  if (!DECLTESTPNUM(E, 0)) testPassed = false;
#endif
#ifdef PE1
  if (!DECLTESTPNUM(E, 1)) testPassed = false;
#endif
#ifdef PE2
  if (!DECLTESTPNUM(E, 2)) testPassed = false;
#endif
#ifdef PE3
  if (!DECLTESTPNUM(E, 3)) testPassed = false;
#endif
#ifdef PE4
  if (!DECLTESTPNUM(E, 4)) testPassed = false;
#endif
#ifdef PE5
  if (!DECLTESTPNUM(E, 5)) testPassed = false;
#endif
#ifdef PE6
  if (!DECLTESTPNUM(E, 6)) testPassed = false;
#endif
#ifdef PE7
  if (!DECLTESTPNUM(E, 7)) testPassed = false;
#endif
#ifdef PE8
  if (!DECLTESTPNUM(E, 8)) testPassed = false;
#endif
#ifdef PE9
  if (!DECLTESTPNUM(E, 9)) testPassed = false;
#endif
#ifdef PE10
  if (!DECLTESTPNUM(E, 10)) testPassed = false;
#endif
#ifdef PE11
  if (!DECLTESTPNUM(E, 11)) testPassed = false;
#endif
#ifdef PE12
  if (!DECLTESTPNUM(E, 12)) testPassed = false;
#endif
#ifdef PE13
  if (!DECLTESTPNUM(E, 13)) testPassed = false;
#endif
#ifdef PE14
  if (!DECLTESTPNUM(E, 14)) testPassed = false;
#endif
#ifdef PE15
  if (!DECLTESTPNUM(E, 15)) testPassed = false;
#endif
#endif
#if defined GPIOF_BASE
#ifdef PF0
  if (!DECLTESTPNUM(F, 0)) testPassed = false;
#endif
#ifdef PF1
  if (!DECLTESTPNUM(F, 1)) testPassed = false;
#endif
#ifdef PF2
  if (!DECLTESTPNUM(F, 2)) testPassed = false;
#endif
#ifdef PF3
  if (!DECLTESTPNUM(F, 3)) testPassed = false;
#endif
#ifdef PF4
  if (!DECLTESTPNUM(F, 4)) testPassed = false;
#endif
#ifdef PF5
  if (!DECLTESTPNUM(F, 5)) testPassed = false;
#endif
#ifdef PF6
  if (!DECLTESTPNUM(F, 6)) testPassed = false;
#endif
#ifdef PF7
  if (!DECLTESTPNUM(F, 7)) testPassed = false;
#endif
#ifdef PF8
  if (!DECLTESTPNUM(F, 8)) testPassed = false;
#endif
#ifdef PF9
  if (!DECLTESTPNUM(F, 9)) testPassed = false;
#endif
#ifdef PF10
  if (!DECLTESTPNUM(F, 10)) testPassed = false;
#endif
#ifdef PF11
  if (!DECLTESTPNUM(F, 11)) testPassed = false;
#endif
#ifdef PF12
  if (!DECLTESTPNUM(F, 12)) testPassed = false;
#endif
#ifdef PF13
  if (!DECLTESTPNUM(F, 13)) testPassed = false;
#endif
#ifdef PF14
  if (!DECLTESTPNUM(F, 14)) testPassed = false;
#endif
#ifdef PF15
  if (!DECLTESTPNUM(F, 15)) testPassed = false;
#endif
#endif
#if defined GPIOG_BASE
#ifdef PG0
  if (!DECLTESTPNUM(G, 0)) testPassed = false;
#endif
#ifdef PG1
  if (!DECLTESTPNUM(G, 1)) testPassed = false;
#endif
#ifdef PG2
  if (!DECLTESTPNUM(G, 2)) testPassed = false;
#endif
#ifdef PG3
  if (!DECLTESTPNUM(G, 3)) testPassed = false;
#endif
#ifdef PG4
  if (!DECLTESTPNUM(G, 4)) testPassed = false;
#endif
#ifdef PG5
  if (!DECLTESTPNUM(G, 5)) testPassed = false;
#endif
#ifdef PG6
  if (!DECLTESTPNUM(G, 6)) testPassed = false;
#endif
#ifdef PG7
  if (!DECLTESTPNUM(G, 7)) testPassed = false;
#endif
#ifdef PG8
  if (!DECLTESTPNUM(G, 8)) testPassed = false;
#endif
#ifdef PG9
  if (!DECLTESTPNUM(G, 9)) testPassed = false;
#endif
#ifdef PG10
  if (!DECLTESTPNUM(G, 10)) testPassed = false;
#endif
#ifdef PG11
  if (!DECLTESTPNUM(G, 11)) testPassed = false;
#endif
#ifdef PG12
  if (!DECLTESTPNUM(G, 12)) testPassed = false;
#endif
#ifdef PG13
  if (!DECLTESTPNUM(G, 13)) testPassed = false;
#endif
#ifdef PG14
  if (!DECLTESTPNUM(G, 14)) testPassed = false;
#endif
#ifdef PG15
  if (!DECLTESTPNUM(G, 15)) testPassed = false;
#endif
#endif
#if defined GPIO_HBASE
#ifdef PH0
  if (!DECLTESTPNUM(H, 0)) testPassed = false;
#endif
#ifdef PH1
  if (!DECLTESTPNUM(H, 1)) testPassed = false;
#endif
#ifdef PH2
  if (!DECLTESTPNUM(H, 2)) testPassed = false;
#endif
#ifdef PH3
  if (!DECLTESTPNUM(H, 3)) testPassed = false;
#endif
#ifdef PH4
  if (!DECLTESTPNUM(H, 4)) testPassed = false;
#endif
#ifdef PH5
  if (!DECLTESTPNUM(H, 5)) testPassed = false;
#endif
#ifdef PH6
  if (!DECLTESTPNUM(H, 6)) testPassed = false;
#endif
#ifdef PH7
  if (!DECLTESTPNUM(H, 7)) testPassed = false;
#endif
#ifdef PH8
  if (!DECLTESTPNUM(H, 8)) testPassed = false;
#endif
#ifdef PH9
  if (!DECLTESTPNUM(H, 9)) testPassed = false;
#endif
#ifdef PH10
  if (!DECLTESTPNUM(H, 10)) testPassed = false;
#endif
#ifdef PH11
  if (!DECLTESTPNUM(H, 11)) testPassed = false;
#endif
#ifdef PH12
  if (!DECLTESTPNUM(H, 12)) testPassed = false;
#endif
#ifdef PH13
  if (!DECLTESTPNUM(H, 13)) testPassed = false;
#endif
#ifdef PH14
  if (!DECLTESTPNUM(H, 14)) testPassed = false;
#endif
#ifdef PH15
  if (!DECLTESTPNUM(H, 15)) testPassed = false;
#endif
#endif
#if defined GPIOI_BASE
#ifdef PI0
  if (!DECLTESTPNUM(I, 0)) testPassed = false;
#endif
#ifdef PI1
  if (!DECLTESTPNUM(I, 1)) testPassed = false;
#endif
#ifdef PI2
  if (!DECLTESTPNUM(I, 2)) testPassed = false;
#endif
#ifdef PI3
  if (!DECLTESTPNUM(I, 3)) testPassed = false;
#endif
#ifdef PI4
  if (!DECLTESTPNUM(I, 4)) testPassed = false;
#endif
#ifdef PI5
  if (!DECLTESTPNUM(I, 5)) testPassed = false;
#endif
#ifdef PI6
  if (!DECLTESTPNUM(I, 6)) testPassed = false;
#endif
#ifdef PI7
  if (!DECLTESTPNUM(I, 7)) testPassed = false;
#endif
#ifdef PI8
  if (!DECLTESTPNUM(I, 8)) testPassed = false;
#endif
#ifdef PI9
  if (!DECLTESTPNUM(I, 9)) testPassed = false;
#endif
#ifdef PI10
  if (!DECLTESTPNUM(I, 10)) testPassed = false;
#endif
#ifdef PI11
  if (!DECLTESTPNUM(I, 11)) testPassed = false;
#endif
#ifdef PI12
  if (!DECLTESTPNUM(I, 12)) testPassed = false;
#endif
#ifdef PI13
  if (!DECLTESTPNUM(I, 13)) testPassed = false;
#endif
#ifdef PI14
  if (!DECLTESTPNUM(I, 14)) testPassed = false;
#endif
#ifdef PI15
  if (!DECLTESTPNUM(I, 15)) testPassed = false;
#endif
#endif
#if defined GPIOJ_BASE
#ifdef PJ0
  if (!DECLTESTPNUM(J, 0)) testPassed = false;
#endif
#ifdef PJ1
  if (!DECLTESTPNUM(J, 1)) testPassed = false;
#endif
#ifdef PJ2
  if (!DECLTESTPNUM(J, 2)) testPassed = false;
#endif
#ifdef PJ3
  if (!DECLTESTPNUM(J, 3)) testPassed = false;
#endif
#ifdef PJ4
  if (!DECLTESTPNUM(J, 4)) testPassed = false;
#endif
#ifdef PJ5
  if (!DECLTESTPNUM(J, 5)) testPassed = false;
#endif
#ifdef PJ6
  if (!DECLTESTPNUM(J, 6)) testPassed = false;
#endif
#ifdef PJ7
  if (!DECLTESTPNUM(J, 7)) testPassed = false;
#endif
#ifdef PJ8
  if (!DECLTESTPNUM(J, 8)) testPassed = false;
#endif
#ifdef PJ9
  if (!DECLTESTPNUM(J, 9)) testPassed = false;
#endif
#ifdef PJ10
  if (!DECLTESTPNUM(J, 10)) testPassed = false;
#endif
#ifdef PJ11
  if (!DECLTESTPNUM(J, 11)) testPassed = false;
#endif
#ifdef PJ12
  if (!DECLTESTPNUM(J, 12)) testPassed = false;
#endif
#ifdef PJ13
  if (!DECLTESTPNUM(J, 13)) testPassed = false;
#endif
#ifdef PJ14
  if (!DECLTESTPNUM(J, 14)) testPassed = false;
#endif
#ifdef PJ15
  if (!DECLTESTPNUM(J, 15)) testPassed = false;
#endif
#endif
#if defined GPIOK_BASE
#ifdef PK0
  if (!DECLTESTPNUM(K, 0)) testPassed = false;
#endif
#ifdef PK1
  if (!DECLTESTPNUM(K, 1)) testPassed = false;
#endif
#ifdef PK2
  if (!DECLTESTPNUM(K, 2)) testPassed = false;
#endif
#ifdef PK3
  if (!DECLTESTPNUM(K, 3)) testPassed = false;
#endif
#ifdef PK4
  if (!DECLTESTPNUM(K, 4)) testPassed = false;
#endif
#ifdef PK5
  if (!DECLTESTPNUM(K, 5)) testPassed = false;
#endif
#ifdef PK6
  if (!DECLTESTPNUM(K, 6)) testPassed = false;
#endif
#ifdef PK7
  if (!DECLTESTPNUM(K, 7)) testPassed = false;
#endif
#ifdef PK8
  if (!DECLTESTPNUM(K, 8)) testPassed = false;
#endif
#ifdef PK9
  if (!DECLTESTPNUM(K, 9)) testPassed = false;
#endif
#ifdef PK10
  if (!DECLTESTPNUM(K, 10)) testPassed = false;
#endif
#ifdef PK11
  if (!DECLTESTPNUM(K, 11)) testPassed = false;
#endif
#ifdef PK12
  if (!DECLTESTPNUM(K, 12)) testPassed = false;
#endif
#ifdef PK13
  if (!DECLTESTPNUM(K, 13)) testPassed = false;
#endif
#ifdef PK14
  if (!DECLTESTPNUM(K, 14)) testPassed = false;
#endif
#ifdef PK15
  if (!DECLTESTPNUM(K, 15)) testPassed = false;
#endif
#endif

  Serial.println("End check digital pins");
  return testPassed;
}
