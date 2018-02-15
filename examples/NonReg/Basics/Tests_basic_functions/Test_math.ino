/*
 * Test some math and trigo functions
 */
void test_math(void)
{
  bool validate = true;

  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println("TEST MATH & TRIGONOMETRY FUNCTIONS");

  if(min(1512,25) != 25) {validate = false;}
  if(max(1512,25) != 1512) {validate = false;}
  if((abs(-12) != 12) && (abs(52) != 52)) {validate = false;}
  if((round(13.99) != 13) && (round(13.01) != 13)) {validate = false;}
  if(pow(2,3) != 8) {validate = false;}
  if(sqrt(4) != 2) {validate = false;}
  if((sin(PI/2) - 1.0) > 0.00001) {validate = false;}
  if((cos(0) - 1.0) > 0.00001) {validate = false;}
  if((tan(PI/4) - 1.0) > 0.00001) {validate = false;}

  printResult(validate);
}
