#include "Arduino.h"
#include <stdint.h>

/**
 * Most STM32 devices don't have an integrated EEPROM.
 * To emulate a EEPROM, the STM32 Arduino core emulated
 * the operation of an EEPROM with the help of the embedded
 * flash.
 *
 * Writing to a flash is very expensive operation, since a
 * whole flash page needs to be written, even if you only
 * want to access the flash byte-wise.
 *
 * The STM32 Arduino core provides a buffered access API
 * to the emulated EEPROM. The library has allocated the
 * buffer even if you don't use the buffered API, so
 * it's strongly suggested to use the buffered API anyhow.
 */

#define DATA_LENGTH E2END

void setup() {
	Serial.begin(115200);
}

void loop() {
	// Fill the EEPROM buffer in memory with data
	for (uint16_t i = 0; i < DATA_LENGTH; i++) {
		eeprom_buffered_write_byte(i, i % 256);
	}

	// Copy the data from the buffer to the flash
	eeprom_buffer_flush();

	// Clear the buffer for demonstration purpose
	for (uint16_t i = 0; i < DATA_LENGTH; i++) {
		eeprom_buffered_write_byte(i, 0);
	}

	// Print the 254th byte of the current buffer (should be 0)
	Serial.println(eeprom_buffered_read_byte(254));

	// Copy the data from the flash to the buffer
	eeprom_buffer_fill();

	// Print the 254th byte of the current buffer (should be 254)
	Serial.println(eeprom_buffered_read_byte(254));
}
