/*
   Checks Wire instances (I2C)
   Returns true in case of test passed
   Returns false in case of test failed
*/
bool checkI2CInstance(void) {
  bool testPassed = true;

#if defined(PIN_WIRE_SDA) && defined(PIN_WIRE_SCL)
  I2C_TypeDef *i2c_sda = (I2C_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_WIRE_SDA), PinMap_I2C_SDA);
  I2C_TypeDef *i2c_scl = (I2C_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_WIRE_SCL), PinMap_I2C_SCL);
  if (i2c_sda == NP) {
    Serial.printf("PIN_WIRE_SDA (%d) doesn't match a valid I2C peripheral\n", PIN_WIRE_SDA);
    testPassed = false;
  }
  if (i2c_scl == NP) {
    Serial.printf("PIN_WIRE_SCL (%d) doesn't match a valid I2C peripheral\n", PIN_WIRE_SCL);
    testPassed = false;
  }
  if (i2c_sda != i2c_scl) {
    Serial.printf("PIN_WIRE_SCL (%d) doesn't match PIN_WIRE_SDA (%d) peripheral\n", PIN_WIRE_SCL, PIN_WIRE_SDA);
    testPassed = false;
  }
#endif
  return testPassed;
}

#if defined(SERIAL_UART_INSTANCE)
/*
   Returns USART_TypeDef corresponding to SERIAL_UART_INSTANCE
*/
USART_TypeDef* getSerialInstance(void) {
#if SERIAL_UART_INSTANCE == 0 || SERIAL_UART_INSTANCE == 101
  return LPUART1;
#elif SERIAL_UART_INSTANCE == 1
  return USART1;
#elif SERIAL_UART_INSTANCE == 2
  return USART2;
#elif SERIAL_UART_INSTANCE == 3
  return USART3;
#elif SERIAL_UART_INSTANCE == 4
#if defined(UART4_BASE)
  return UART4;
#else
  return USART4;
#endif
#elif SERIAL_UART_INSTANCE == 5
#if defined(UART5_BASE)
  return UART5;
#else
  return USART5;
#endif
#elif SERIAL_UART_INSTANCE == 6
  return USART6;
#elif SERIAL_UART_INSTANCE == 7
#if defined(UART7_BASE)
  return UART7;
#else
  return USART7;
#endif
#elif SERIAL_UART_INSTANCE == 8
#if defined(UART8_BASE)
  return UART8;
#else
  return USART8;
#endif
#elif SERIAL_UART_INSTANCE == 9
  return USART9;
#elif SERIAL_UART_INSTANCE == 10
#if defined(UART10_BASE)
  return UART10;
#else
  return USART10;
#endif
#else
  return NULL;
#endif
}
#endif /* SERIAL_UART_INSTANCE */

/*
   Checks Serial instances (UART)
   Returns true in case of test passed
   Returns false in case of test failed
*/
bool checkSerialInstance(void) {
  bool testPassed = true;

#if defined(PIN_SERIAL_RX) && defined(PIN_SERIAL_TX)
  USART_TypeDef *uart_rx = (USART_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_SERIAL_RX), PinMap_UART_RX);
  USART_TypeDef *uart_tx = (USART_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_SERIAL_TX), PinMap_UART_TX);
  if (uart_rx == NP) {
    Serial.printf("PIN_SERIAL_RX (%d) doesn't match a valid UART peripheral\n", PIN_SERIAL_RX);
    testPassed = false;
  }
  if (uart_tx == NP) {
    Serial.printf("PIN_SERIAL_TX (%d) doesn't match a valid UART peripheral\n", PIN_SERIAL_TX);
    testPassed = false;
  }
  if (uart_rx != uart_tx) {
    Serial.printf("PIN_SERIAL_RX (%d) doesn't match PIN_SERIAL_TX (%d) peripheral\n", PIN_SERIAL_RX, PIN_SERIAL_TX);
    testPassed = false;
  }

#if defined(SERIAL_UART_INSTANCE)
  USART_TypeDef* usart = getSerialInstance();
  if (usart != uart_tx) {
    Serial.printf("SERIAL_UART_INSTANCE (%d) doesn't match PIN_SERIAL_TX (%d) peripheral\n", SERIAL_UART_INSTANCE, PIN_SERIAL_TX);
    testPassed = false;
  }
#endif

#endif /* PIN_SERIAL_RX && PIN_SERIAL_TX */

  return testPassed;
}

/*
   Checks SPI instances
   Returns true in case of test passed
   Returns false in case of test failed
*/
bool checkSPIInstance(void) {
  bool testPassed = true;

#if defined(PIN_SPI_MOSI) && defined(PIN_SPI_MISO)
  SPI_TypeDef *spi_mosi = (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_SPI_MOSI), PinMap_SPI_MOSI);
  SPI_TypeDef *spi_miso = (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_SPI_MISO), PinMap_SPI_MISO);
  if (spi_mosi == NP) {
    Serial.printf("PIN_SPI_MOSI (%d) doesn't match a valid SPI peripheral\n", PIN_SPI_MOSI);
    testPassed = false;
  }
  if (spi_miso == NP) {
    Serial.printf("PIN_SPI_MISO (%d) doesn't match a valid SPI peripheral\n", PIN_SPI_MISO);
    testPassed = false;
  }
  if (spi_mosi != spi_miso) {
    Serial.printf("PIN_SPI_MOSI (%d) doesn't match PIN_SPI_MISO (%d) peripheral\n", PIN_SPI_MOSI, PIN_SPI_MISO);
    testPassed = false;
  }

#if defined(PIN_SPI_SCK)
  SPI_TypeDef *spi_sck = (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(PIN_SPI_SCK), PinMap_SPI_SCLK);
  if (spi_sck == NP) {
    Serial.printf("PIN_SPI_SCK (%d) doesn't match a valid SPI peripheral\n", PIN_SPI_SCK);
    testPassed = false;
  }
  if (spi_sck != spi_mosi) {
    Serial.printf("PIN_SPI_SCK (%d) doesn't match PIN_SPI_MISO (%d) peripheral\n", PIN_SPI_SCK, PIN_SPI_MISO);
    testPassed = false;
  }
#endif

#endif /* PIN_SPI_MOSI && PIN_SPI_MISO */

#if defined(PIN_SPI_SS)
  /* PIN_SPI_SS may be used as software chip select (and not hardware) any pin is valid
     thus not possible to check this pin agianst PinMap_SPI_SSEL */
  if (!digitalPinIsValid(PIN_SPI_SS)) {
    Serial.printf("PIN_SPI_SS (%d) doesn't match PIN_SPI_MISO (%d) peripheral\n", PIN_SPI_SS, PIN_SPI_MISO);
    testPassed = false;
  }
#endif

  return testPassed;
}

/*
   Check IP instances (I2C, UART, SPI)
   Return true in case of test passed
   Return false in case of test failed
*/
bool checkIPInstance(void) {
  bool testPassed = true;

  Serial.println("#####");
  Serial.println("Checking IP instances (I2C, UART, SPI)...");

  if (!checkI2CInstance()) {
    testPassed = false;
  }
  if (!checkSerialInstance()) {
    testPassed = false;
  }
  if (!checkSPIInstance()) {
    testPassed = false;
  }
  Serial.println("End check IP instances (I2C, UART, SPI)");
  return testPassed;
}
