/*
 * Test some advanced I/O functions
 */
#ifdef TIMER_TONE
/*
 * Variable for advanced I/O test
 */
int modeWR = 0;
volatile int clknb = 0;
volatile int data = 0;
int clkOut = 2;
int dataOut = 3;
int clkIn = 4;
int dataIn = 5;

void test_IO(void)
{
  bool validate = true;
  int tmp = 0;
  int freq_test = 1000;

  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println("TEST ADVANCED I/O");
  Serial.println("Connect D2 with D4 and D3 with D5. Then press ENTER.");
  while (message((char*)"\n") == false) {yield();}

  pinMode(clkOut, OUTPUT);
  pinMode(dataOut, OUTPUT);
  pinMode(clkIn, INPUT);
  pinMode(dataIn, INPUT);

  tone(clkOut,freq_test);
  while(digitalRead(clkIn) == HIGH){yield();}
  if(digitalRead(clkIn) == LOW) {
    tmp = pulseIn(clkIn, HIGH);
  }
  noTone(clkOut);
  pinMode(clkOut, OUTPUT);

  float freq_period_us = 1.0/(float)freq_test;
  tmp -= freq_period_us*1000000/2;
  tmp = abs(tmp);

  //error = +/-1%
  if(tmp > 10) {validate = false;}

  digitalWrite(clkOut, LOW);
  digitalWrite(dataOut, LOW);

  attachInterrupt(clkIn, test_IO_IT, RISING);

  clknb = 0;
  data = 0;
  modeWR = 0;
  shiftOut(dataOut, clkOut, LSBFIRST, 0xE2);
  if(data != 0xE2) {validate = false;}
  digitalWrite(clkOut, LOW);
  digitalWrite(dataOut, LOW);
  pinMode(dataIn, OUTPUT);
  pinMode(dataOut, INPUT);
  data = 0xD7;
  clknb = 0;

  modeWR = 1;
  data = shiftIn(dataOut, clkOut, LSBFIRST);
  if(data != 0xD7) {validate = false;}

  detachInterrupt(clkIn);

  printResult(validate);
}

void test_IO_IT(void)
{
  if(modeWR == 0) {
    data |= digitalRead(dataIn) << clknb;
    clknb++;
  } else {
    int tmp = bitRead(data, clknb);
    digitalWrite(dataIn, tmp);
    clknb++;
  }
}
#endif
