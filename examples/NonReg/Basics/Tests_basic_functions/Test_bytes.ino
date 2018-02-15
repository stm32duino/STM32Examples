/*
 * Test some bytes functions
 */
void test_bytes(void)
{
   bool validate = true;

  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println("TEST BIT & BYTES FUNCTIONS");

  int val = 0x2563;

  if(lowByte(val) != 0x63) {validate = false;}
  if(highByte(val) != 0x25) {validate = false;}
  if(bitRead(val, 5) != 1) {validate = false;}
  bitWrite(val, 3, 1);
  if(val != 0x256B) {validate = false;}
  bitSet(val, 12);
  if(val != 0x356B) {validate = false;}
  bitClear(val, 8);
  if(val != 0x346B) {validate = false;}
  val = bit(2) + bit(4);
  if(val != 0x14) {validate = false;}

  printResult(validate);
}
