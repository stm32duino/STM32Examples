/*
 * Test some the string functions
 */
void test_string(void)
{
  int validate = true;

  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println("TEST STRING FUNCTIONS");

  String str = "hello world!";
  String strTmp = String("helln");

  if(strTmp.compareTo(str) != 0) {
    strTmp.concat(" world!");
  } else {validate = false;}

  strTmp.remove(0,6);
  if(strTmp.equals("world!") == false) {validate = false;}

  char cbuf[13];
  byte bbuf[13];
  str.getBytes(bbuf,13);
  str.toCharArray(cbuf,13);
  if(strcmp((const char *)bbuf,str.c_str()) != 0) {validate = false;}
  if(strcmp((const char *)cbuf,str.c_str()) != 0) {validate = false;}

  strTmp = "12.34";
  if(strTmp.toFloat()-12.34 >= 0.00001) {validate = false;}

  char sout[256]="";
  if(strcmp(dtostrf(1.23, 7, 2, sout), "   1.23") != 0) {validate = false;}
  if(strcmp(dtostrf(1.899, 2, 2, sout), "1.90") != 0) {validate = false;}
  if(strcmp(dtostrf(-123456.78910, 127, 6, sout),
  "                                                                                                                 -123456.789100"
           ) != 0) {validate = false;}
  if(strcmp(dtostrf(-123456.78910, 128, 6, sout),
  "-123456.789100                                                                                                                  "
           ) != 0) {validate = false;}

  printResult(validate);
}
