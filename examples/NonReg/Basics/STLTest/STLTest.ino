/*
  STLTest

  Test abs/min/max/round called from C or C++

  Creation 10 Oct 2017
  by Frederic Pillon <frederic.pillon@st.com>

  This example code is in the public domain.
*/

#include "test_stl.h"
static int32_t count_pressed = 0;
static int32_t random_val = 0;

// Check if a default user button is defined
// If not then define it to pin 2, user can set an other pin.
// Button have to be wired on this pin
#ifndef USER_BTN
#define USER_BTN 2
#endif
int pushButton = USER_BTN;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  random_val = random(300);
  if (buttonState == LOW)
  {
    count_pressed++;
  }

  Serial.print("count pressed: ");
  Serial.print(count_pressed);
  Serial.print("\trandom: ");
  Serial.println(random_val);

  Serial.print("C++\tmin: ");
  Serial.print(min(random_val,count_pressed));
  Serial.print("\tmax: ");
  Serial.print(max(random_val,count_pressed));
  Serial.print("\tabs: ");
  Serial.print(abs(count_pressed));
  Serial.print("\tround (count_pressed/random_val): ");
  Serial.println(round(count_pressed/random_val));

  Serial.print("C\tmin: ");
  Serial.print(test_c(random_val,count_pressed, MIN));
  Serial.print("\tmax: ");
  Serial.print(test_c(random_val,count_pressed, MAX));
  Serial.print("\tabs: ");
  Serial.print(test_c(count_pressed, 0, ABS));
  Serial.print("\tround (count_pressed/random_val) is ");
  Serial.println(test_c(count_pressed, random_val, ROUND));

  Serial.println();
  delay(100);        // delay in between reads for stability
}
