/*
  SPI_loopback

  Test the loopback transfer on SPI.
  A single SPI instance is used, the default one.
  MISO pin must be externally connected to MOSI pin.
  To test the transfer on different SPI instance, just redefine SPI pins
  as follows:
  #define MOSI_PIN  XXX
  #define SCK_PIN     XXX
  #define MISO_PIN  XXX
   #define CS_PIN      XXX

  Test behavior:
  A test string is sent 9 times and compared to the Rx frame.
  The LED remains ON when test ends successfully
  The LED remains OFF in case of error.
  This example code is in the public domain.
*/
#include "SPI.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN PNUM_NOT_DEFINED
#warning "LED_BUILTIN is not defined."
#endif

#define MOSI_PIN  PIN_SPI_MOSI
#define MISO_PIN  PIN_SPI_MISO
#define SCK_PIN   PIN_SPI_SCK
#define CS_PIN    PIN_SPI_SS

uint8_t buffer_tx[] = "Thequickbrownfoxjumpsoverthelazydog\0";
#define BUF_SIZE  sizeof(buffer_tx)
uint8_t buffer_rx[BUF_SIZE] = {};

/* SPI transfer loop nb */
#define TEST_LOOP_NB 9

void setup() {
  uint8_t tested_ok = 0; // test result

  SPI.setMOSI(MOSI_PIN);
  SPI.setMISO(MISO_PIN);
  SPI.setSCLK(SCK_PIN);
  // Don't set CS_PIN to have Software ChipSelect management
  // Instead, CS_PIN is passed as argument to SPI.transfer(CS_PIN)
  // SPI.setSSEL(CS_PIN);

  /* the SPI pin configuration is done by the SPI.begin */
   SPI.begin(CS_PIN);

  for (uint8_t received = 0; received < TEST_LOOP_NB; received++) {
    SPI.transfer(CS_PIN, buffer_tx, buffer_rx, BUF_SIZE, SPI_LAST);

    /* compare what is received to what was sent */
    if (!memcmp(buffer_tx, buffer_rx, BUF_SIZE)) {
      /* this transfer passed */
      tested_ok++;
    }
    memset(buffer_rx, 0, BUF_SIZE);
  }
  /* display test result */
  pinMode(LED_BUILTIN, OUTPUT);   // Configure LED pin, for test result
  digitalWrite(LED_BUILTIN, LOW); // start with led off
  if (tested_ok == TEST_LOOP_NB) {
      /* success */
      digitalWrite(LED_BUILTIN, HIGH);
  } else {
      /* error. Please verify MISO is externally connected to MOSI */
      digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
  /* nothing to do */
}
