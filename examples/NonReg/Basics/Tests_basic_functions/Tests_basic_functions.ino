/*
 *  This sketch test several of the Arduino standard functions which
 *  are not tested in the example sketches.
 *
 *  !!! The human action is sometimes required!
 *
 *  Created by Wi6Labs www.wi6labs.com
 *  Modified by Frederic Pillon <frederic.pillon@st.com>
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  list_test();
}

void list_test(void)
{
  Serial.println();
  Serial.println("**Enter a number to select a test**");
  Serial.println("1-String");
  Serial.println("2-Math");
  Serial.println("3-Bytes");
#ifdef TIMER_TONE
  Serial.println("4-Advanced I/O");
#endif
}

void loop() {
  int test = 0;

  while (Serial.available() > 0) {
    test = Serial.parseInt();
    Serial.flush();

    switch(test)
    {
    case 1: test_string(); break;
    case 2: test_math(); break;
    case 3: test_bytes(); break;
#ifdef TIMER_TONE
    case 4: test_IO(); break;
#endif
    default: break;
    }

    list_test();
  }

  test = 0;
}

bool message(char *str)
{
  if (Serial.available() > 0) {
    if (Serial.find(str) == true)
      return true;
  }

  return false;
}

void printResult(int validate)
{
  if(validate == true) {
    Serial.println("> OK <");
  } else {
    Serial.println(">>> FAIL <<<");
  }
}

/*----------------------------------------------------------------------------------*/
