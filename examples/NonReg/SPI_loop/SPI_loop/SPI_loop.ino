/*
  SPI_loopback

  Test the loopback transfer on SPI serial link.
  A test string is sent 9 times and compared to the Rx frame.
  when the MISO pin is physically connected to the MOSI pin of the SPI bus
  Default test the SPI_A link.
  Led remains ON when test ends successfully (off in case of error).
  To test the transfer on the SPI_B , just define SPI_B pins and connect,
  as follows:
    int MOSI_PIN = 22; // to be connected to the B_MISO
    int SCK_PIN = 23;
    int CS_PIN = 24;
    int MISO_PIN = 25; // to be connected to the B_MOSI
  and configure as follows:
    SPI.setMISO(MISO_PIN);
    SPI.setMOSI(MOSI_PIN);
    SPI.setSCLK(SCK_PIN);
    SPI.setSSEL(CS_PIN);

  This example code is in the public domain.

*/
#include "SPI.h"

#define MOSI_PIN  PIN_SPI_MOSI
#define SCK_PIN   PIN_SPI_SCK
#define CS_PIN    PIN_SPI_SS
#define MISO_PIN  PIN_SPI_MOSI

#define BUF_SIZE 36
uint8_t buffer_tx[] = "Thequickbrownfoxjumpsoverthelazydog\0";
uint8_t buffer_rx[BUF_SIZE] = {};

/* SPI transfer loop nb */
#define TEST_LOOP_NB 9

void setup() {
  uint8_t tested_ok = 0; // test result

  /* the SPI pin configuration is done by the SPI.begin */
   SPI.begin(CS_PIN); // waking up SPI bus
  // SPI.setClockDivider(CS_PIN, 256);     // setting a slower clock

  pinMode(LED_BUILTIN, OUTPUT);   // prepare led for status
  digitalWrite(LED_BUILTIN, LOW); // start with led off

  for (uint8_t received = 0; received < TEST_LOOP_NB; received++) {
    SPI.transfer(CS_PIN, buffer_tx, buffer_rx, BUF_SIZE, SPI_LAST);
    delay(100);
    /* compare what is received to what was sent */
    if (!memcmp(buffer_tx, buffer_rx, BUF_SIZE)) {
      /* this transfer passed */
      tested_ok++;
    }
    memset(buffer_rx, 0, BUF_SIZE);
  }
  /* display test result */
  if (tested_ok == TEST_LOOP_NB) {
      /* success */
      digitalWrite(LED_BUILTIN, HIGH);
  } else {
      /* error */
      digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
  /* nothing special */
}

