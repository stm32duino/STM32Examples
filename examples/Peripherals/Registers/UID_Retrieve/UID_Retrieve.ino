/* UID Retrieve sketch
* UID (Universal Identifier) is a ID that's etched to each MCU at factory release 
* so it's uniquely identifiable. This can help traceability and addressing devices 
* without having to craft a database yourself. This sketch retrieves UID, MAC, Device
* and Revision ID of each MCU. Refer to the relevant datasheet to know where are these 
* values are stored in the registers.
* The code is provided "as is" with no liability.
*/

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;  // Wait for Serial to be ready

  Serial.printf("%s Device Identifiers:\n", BOARD_NAME);

  // Unique Device ID (96 bits / 12 bytes)
  uint32_t uid0 = HAL_GetUIDw0();
  uint32_t uid1 = HAL_GetUIDw1();
  uint32_t uid2 = HAL_GetUIDw2();

  Serial.print("UID: ");
  Serial.print(uid2, HEX);
  Serial.print("-");
  Serial.print(uid1, HEX);
  Serial.print("-");
  Serial.println(uid0, HEX);

  // MAC Address: typically stored in UID for STM32U series
  // Use the lower 6 bytes of the 96-bit UID (commonly used)
  uint8_t mac[6] = {
    (uint8_t)(uid0 >> 0),
    (uint8_t)(uid0 >> 8),
    (uint8_t)(uid0 >> 16),
    (uint8_t)(uid1 >> 0),
    (uint8_t)(uid1 >> 8),
    (uint8_t)(uid1 >> 16)
  };

  Serial.print("MAC Address: ");
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 0x10) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  Serial.printf("Device ID: 0x%x\n", HAL_GetDEVID());
  Serial.printf("Revision ID: 0x%x\n", HAL_GetREVID());
}

void loop() {
  // Nothing here
}
