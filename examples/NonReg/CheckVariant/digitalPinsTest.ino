
#define PNUM(Port, Pin)   P ## Port ## Pin
#define PNAME(Port, Pin)  P ## Port ## _ ## Pin
#define DECLTESTPNUM(Port, Pin)  checkPinNumber(XSTR(PNAME(Port, Pin)), PNAME(Port, Pin), "P"#Port#Pin, PNUM(Port, Pin))

void checkPinNumber(const char* spname, PinName pname, const char* spnum, uint32_t pnum) {
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
  }
}

void checkDigitalPins(void) {
  Serial.println("Checking digital pins...");
  Serial.println("\tPin number definition...");
#ifdef PA0
  DECLTESTPNUM(A, 0);
#endif
#ifdef PA1
  DECLTESTPNUM(A, 1);
#endif
#ifdef PA2
  DECLTESTPNUM(A, 2);
#endif
#ifdef PA3
  DECLTESTPNUM(A, 3);
#endif
#ifdef PA4
  DECLTESTPNUM(A, 4);
#endif
#ifdef PA5
  DECLTESTPNUM(A, 5);
#endif
#ifdef PA6
  DECLTESTPNUM(A, 6);
#endif
#ifdef PA7
  DECLTESTPNUM(A, 7);
#endif
#ifdef PA8
  DECLTESTPNUM(A, 8);
#endif
#ifdef PA9
  DECLTESTPNUM(A, 9);
#endif
#ifdef PA10
  DECLTESTPNUM(A, 10);
#endif
#ifdef PA11
  DECLTESTPNUM(A, 11);
#endif
#ifdef PA12
  DECLTESTPNUM(A, 12);
#endif
#ifdef PA13
  DECLTESTPNUM(A, 13);
#endif
#ifdef PA14
  DECLTESTPNUM(A, 14);
#endif
#ifdef PA15
  DECLTESTPNUM(A, 15);
#endif
#ifdef PB0
  DECLTESTPNUM(B, 0);
#endif
#ifdef PB1
  DECLTESTPNUM(B, 1);
#endif
#ifdef PB2
  DECLTESTPNUM(B, 2);
#endif
#ifdef PB3
  DECLTESTPNUM(B, 3);
#endif
#ifdef PB4
  DECLTESTPNUM(B, 4);
#endif
#ifdef PB5
  DECLTESTPNUM(B, 5);
#endif
#ifdef PB6
  DECLTESTPNUM(B, 6);
#endif
#ifdef PB7
  DECLTESTPNUM(B, 7);
#endif
#ifdef PB8
  DECLTESTPNUM(B, 8);
#endif
#ifdef PB9
  DECLTESTPNUM(B, 9);
#endif
#ifdef PB10
  DECLTESTPNUM(B, 10);
#endif
#ifdef PB11
  DECLTESTPNUM(B, 11);
#endif
#ifdef PB12
  DECLTESTPNUM(B, 12);
#endif
#ifdef PB13
  DECLTESTPNUM(B, 13);
#endif
#ifdef PB14
  DECLTESTPNUM(B, 14);
#endif
#ifdef PB15
  DECLTESTPNUM(B, 15);
#endif
#if defined GPIOC_BASE
#ifdef PC0
  DECLTESTPNUM(C, 0);
#endif
#ifdef PC1
  DECLTESTPNUM(C, 1);
#endif
#ifdef PC2
  DECLTESTPNUM(C, 2);
#endif
#ifdef PC3
  DECLTESTPNUM(C, 3);
#endif
#ifdef PC4
  DECLTESTPNUM(C, 4);
#endif
#ifdef PC5
  DECLTESTPNUM(C, 5);
#endif
#ifdef PC6
  DECLTESTPNUM(C, 6);
#endif
#ifdef PC7
  DECLTESTPNUM(C, 7);
#endif
#ifdef PC8
  DECLTESTPNUM(C, 8);
#endif
#ifdef PC9
  DECLTESTPNUM(C, 9);
#endif
#ifdef PC10
  DECLTESTPNUM(C, 10);
#endif
#ifdef PC11
  DECLTESTPNUM(C, 11);
#endif
#ifdef PC12
  DECLTESTPNUM(C, 12);
#endif
#ifdef PC13
  DECLTESTPNUM(C, 13);
#endif
#ifdef PC14
  DECLTESTPNUM(C, 14);
#endif
#ifdef PC15
  DECLTESTPNUM(C, 15);
#endif
#endif
#if defined GPIOD_BASE
#ifdef PD0
  DECLTESTPNUM(D, 0);
#endif
#ifdef PD1
  DECLTESTPNUM(D, 1);
#endif
#ifdef PD2
  DECLTESTPNUM(D, 2);
#endif
#ifdef PD3
  DECLTESTPNUM(D, 3);
#endif
#ifdef PD4
  DECLTESTPNUM(D, 4);
#endif
#ifdef PD5
  DECLTESTPNUM(D, 5);
#endif
#ifdef PD6
  DECLTESTPNUM(D, 6);
#endif
#ifdef PD7
  DECLTESTPNUM(D, 7);
#endif
#ifdef PD8
  DECLTESTPNUM(D, 8);
#endif
#ifdef PD9
  DECLTESTPNUM(D, 9);
#endif
#ifdef PD10
  DECLTESTPNUM(D, 10);
#endif
#ifdef PD11
  DECLTESTPNUM(D, 11);
#endif
#ifdef PD12
  DECLTESTPNUM(D, 12);
#endif
#ifdef PD13
  DECLTESTPNUM(D, 13);
#endif
#ifdef PD14
  DECLTESTPNUM(D, 14);
#endif
#ifdef PD15
  DECLTESTPNUM(D, 15);
#endif
#endif
#if defined GPIOE_BASE
#ifdef PE0
  DECLTESTPNUM(E, 0);
#endif
#ifdef PE1
  DECLTESTPNUM(E, 1);
#endif
#ifdef PE2
  DECLTESTPNUM(E, 2);
#endif
#ifdef PE3
  DECLTESTPNUM(E, 3);
#endif
#ifdef PE4
  DECLTESTPNUM(E, 4);
#endif
#ifdef PE5
  DECLTESTPNUM(E, 5);
#endif
#ifdef PE6
  DECLTESTPNUM(E, 6);
#endif
#ifdef PE7
  DECLTESTPNUM(E, 7);
#endif
#ifdef PE8
  DECLTESTPNUM(E, 8);
#endif
#ifdef PE9
  DECLTESTPNUM(E, 9);
#endif
#ifdef PE10
  DECLTESTPNUM(E, 10);
#endif
#ifdef PE11
  DECLTESTPNUM(E, 11);
#endif
#ifdef PE12
  DECLTESTPNUM(E, 12);
#endif
#ifdef PE13
  DECLTESTPNUM(E, 13);
#endif
#ifdef PE14
  DECLTESTPNUM(E, 14);
#endif
#ifdef PE15
  DECLTESTPNUM(E, 15);
#endif
#endif
#if defined GPIOF_BASE
#ifdef PF0
  DECLTESTPNUM(F, 0);
#endif
#ifdef PF1
  DECLTESTPNUM(F, 1);
#endif
#ifdef PF2
  DECLTESTPNUM(F, 2);
#endif
#ifdef PF3
  DECLTESTPNUM(F, 3);
#endif
#ifdef PF4
  DECLTESTPNUM(F, 4);
#endif
#ifdef PF5
  DECLTESTPNUM(F, 5);
#endif
#ifdef PF6
  DECLTESTPNUM(F, 6);
#endif
#ifdef PF7
  DECLTESTPNUM(F, 7);
#endif
#ifdef PF8
  DECLTESTPNUM(F, 8);
#endif
#ifdef PF9
  DECLTESTPNUM(F, 9);
#endif
#ifdef PF10
  DECLTESTPNUM(F, 10);
#endif
#ifdef PF11
  DECLTESTPNUM(F, 11);
#endif
#ifdef PF12
  DECLTESTPNUM(F, 12);
#endif
#ifdef PF13
  DECLTESTPNUM(F, 13);
#endif
#ifdef PF14
  DECLTESTPNUM(F, 14);
#endif
#ifdef PF15
  DECLTESTPNUM(F, 15);
#endif
#endif
#if defined GPIOG_BASE
#ifdef PG0
  DECLTESTPNUM(G, 0);
#endif
#ifdef PG1
  DECLTESTPNUM(G, 1);
#endif
#ifdef PG2
  DECLTESTPNUM(G, 2);
#endif
#ifdef PG3
  DECLTESTPNUM(G, 3);
#endif
#ifdef PG4
  DECLTESTPNUM(G, 4);
#endif
#ifdef PG5
  DECLTESTPNUM(G, 5);
#endif
#ifdef PG6
  DECLTESTPNUM(G, 6);
#endif
#ifdef PG7
  DECLTESTPNUM(G, 7);
#endif
#ifdef PG8
  DECLTESTPNUM(G, 8);
#endif
#ifdef PG9
  DECLTESTPNUM(G, 9);
#endif
#ifdef PG10
  DECLTESTPNUM(G, 10);
#endif
#ifdef PG11
  DECLTESTPNUM(G, 11);
#endif
#ifdef PG12
  DECLTESTPNUM(G, 12);
#endif
#ifdef PG13
  DECLTESTPNUM(G, 13);
#endif
#ifdef PG14
  DECLTESTPNUM(G, 14);
#endif
#ifdef PG15
  DECLTESTPNUM(G, 15);
#endif
#endif
#if defined GPIO_HBASE
#ifdef PH0
  DECLTESTPNUM(H, 0);
#endif
#ifdef PH1
  DECLTESTPNUM(H, 1);
#endif
#ifdef PH2
  DECLTESTPNUM(H, 2);
#endif
#ifdef PH3
  DECLTESTPNUM(H, 3);
#endif
#ifdef PH4
  DECLTESTPNUM(H, 4);
#endif
#ifdef PH5
  DECLTESTPNUM(H, 5);
#endif
#ifdef PH6
  DECLTESTPNUM(H, 6);
#endif
#ifdef PH7
  DECLTESTPNUM(H, 7);
#endif
#ifdef PH8
  DECLTESTPNUM(H, 8);
#endif
#ifdef PH9
  DECLTESTPNUM(H, 9);
#endif
#ifdef PH10
  DECLTESTPNUM(H, 10);
#endif
#ifdef PH11
  DECLTESTPNUM(H, 11);
#endif
#ifdef PH12
  DECLTESTPNUM(H, 12);
#endif
#ifdef PH13
  DECLTESTPNUM(H, 13);
#endif
#ifdef PH14
  DECLTESTPNUM(H, 14);
#endif
#ifdef PH15
  DECLTESTPNUM(H, 15);
#endif
#endif
#if defined GPIOI_BASE
#ifdef PI0
  DECLTESTPNUM(I, 0);
#endif
#ifdef PI1
  DECLTESTPNUM(I, 1);
#endif
#ifdef PI2
  DECLTESTPNUM(I, 2);
#endif
#ifdef PI3
  DECLTESTPNUM(I, 3);
#endif
#ifdef PI4
  DECLTESTPNUM(I, 4);
#endif
#ifdef PI5
  DECLTESTPNUM(I, 5);
#endif
#ifdef PI6
  DECLTESTPNUM(I, 6);
#endif
#ifdef PI7
  DECLTESTPNUM(I, 7);
#endif
#ifdef PI8
  DECLTESTPNUM(I, 8);
#endif
#ifdef PI9
  DECLTESTPNUM(I, 9);
#endif
#ifdef PI10
  DECLTESTPNUM(I, 10);
#endif
#ifdef PI11
  DECLTESTPNUM(I, 11);
#endif
#ifdef PI12
  DECLTESTPNUM(I, 12);
#endif
#ifdef PI13
  DECLTESTPNUM(I, 13);
#endif
#ifdef PI14
  DECLTESTPNUM(I, 14);
#endif
#ifdef PI15
  DECLTESTPNUM(I, 15);
#endif
#endif
#if defined GPIOJ_BASE
#ifdef PJ0
  DECLTESTPNUM(J, 0);
#endif
#ifdef PJ1
  DECLTESTPNUM(J, 1);
#endif
#ifdef PJ2
  DECLTESTPNUM(J, 2);
#endif
#ifdef PJ3
  DECLTESTPNUM(J, 3);
#endif
#ifdef PJ4
  DECLTESTPNUM(J, 4);
#endif
#ifdef PJ5
  DECLTESTPNUM(J, 5);
#endif
#ifdef PJ6
  DECLTESTPNUM(J, 6);
#endif
#ifdef PJ7
  DECLTESTPNUM(J, 7);
#endif
#ifdef PJ8
  DECLTESTPNUM(J, 8);
#endif
#ifdef PJ9
  DECLTESTPNUM(J, 9);
#endif
#ifdef PJ10
  DECLTESTPNUM(J, 10);
#endif
#ifdef PJ11
  DECLTESTPNUM(J, 11);
#endif
#ifdef PJ12
  DECLTESTPNUM(J, 12);
#endif
#ifdef PJ13
  DECLTESTPNUM(J, 13);
#endif
#ifdef PJ14
  DECLTESTPNUM(J, 14);
#endif
#ifdef PJ15
  DECLTESTPNUM(J, 15);
#endif
#endif
#if defined GPIOK_BASE
#ifdef PK0
  DECLTESTPNUM(K, 0);
#endif
#ifdef PK1
  DECLTESTPNUM(K, 1);
#endif
#ifdef PK2
  DECLTESTPNUM(K, 2);
#endif
#ifdef PK3
  DECLTESTPNUM(K, 3);
#endif
#ifdef PK4
  DECLTESTPNUM(K, 4);
#endif
#ifdef PK5
  DECLTESTPNUM(K, 5);
#endif
#ifdef PK6
  DECLTESTPNUM(K, 6);
#endif
#ifdef PK7
  DECLTESTPNUM(K, 7);
#endif
#ifdef PK8
  DECLTESTPNUM(K, 8);
#endif
#ifdef PK9
  DECLTESTPNUM(K, 9);
#endif
#ifdef PK10
  DECLTESTPNUM(K, 10);
#endif
#ifdef PK11
  DECLTESTPNUM(K, 11);
#endif
#ifdef PK12
  DECLTESTPNUM(K, 12);
#endif
#ifdef PK13
  DECLTESTPNUM(K, 13);
#endif
#ifdef PK14
  DECLTESTPNUM(K, 14);
#endif
#ifdef PK15
  DECLTESTPNUM(K, 15);
#endif
#endif
  Serial.println("\tEnd check Pin number definition");
  Serial.println("\tCheck pin name available in digitalPin[]");
  Serial.println("\tEnd check pin name available in digitalPin[]");
  Serial.println("End check digital pins");
  Serial.println("#####");
}
