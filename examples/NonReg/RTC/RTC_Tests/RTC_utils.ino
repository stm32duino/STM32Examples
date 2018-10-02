
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}

// sample input: date = "Dec 26 2009", time = "12:34:56"
void initDateTime (const char* date, const char* time) {
  year = conv2d(date + 9);
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  switch (date[0]) {
    case 'J': month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
    case 'F': month = 2; break;
    case 'A': month = date[2] == 'r' ? 4 : 8; break;
    case 'M': month = date[2] == 'r' ? 3 : 5; break;
    case 'S': month = 9; break;
    case 'O': month = 10; break;
    case 'N': month = 11; break;
    case 'D': month = 12; break;
  }
  day = conv2d(date + 4);
  hours = conv2d(time);
  if (hourFormat == rtc.HOUR_12) {
    period = hours >= 12 ? rtc.PM : rtc.AM;
    hours = hours >= 13 ? hours - 12 : (hours < 1 ? hours + 12 : hours);
  }
  minutes = conv2d(time + 3);
  seconds = conv2d(time + 6);
}

// t: number of print
// d: delay between each print
// a: display alarm
void printDateTime(uint32_t t, uint32_t d, bool a) {
  for (uint32_t i=0; i<t; i++) {
    // Print date...
    print2digits(rtc.getMonth());
    Serial.print("/");
    print2digits(rtc.getDay());
    Serial.print("/");
    print2digits(rtc.getYear());
    Serial.print("\t");

    // ...and time
    print2digits(rtc.getHours(&period));
    Serial.print(":");
    print2digits(rtc.getMinutes());
    Serial.print(":");
    print2digits(rtc.getSeconds());
    if (hourFormat == rtc.HOUR_12) {
      Serial.print(period == rtc.AM ? " AM":" PM");
    }
    if(a) {
      // Print day...
      Serial.print("\t");
      print2digits(rtc.getAlarmDay());
      Serial.print("\t");

      // ...and time
      print2digits(rtc.getAlarmHours(&period));
      Serial.print(":");
      print2digits(rtc.getAlarmMinutes());
      Serial.print(":");
      print2digits(rtc.getAlarmSeconds());
      if (hourFormat == rtc.HOUR_12) {
        Serial.print(period == rtc.AM ? " AM":" PM");
      }
    }
    Serial.println();
    delay(d);
  }
}
