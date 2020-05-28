
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
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
  uint32_t subSeconds;

  for (uint32_t i = 0; i < t; i++) {
    rtc.getTime(&hours, &minutes, &seconds, &subSeconds, &period);

    // Print date...
    Serial.printf("%02d/%02d/%02d\t", rtc.getMonth(), rtc.getDay(), rtc.getYear());

    // ...and time
    Serial.printf("%02d:%02d:%02d.%03d", hours, minutes, seconds, subSeconds);

    if (hourFormat == rtc.HOUR_12) {
      Serial.print(period == rtc.AM ? " AM" : " PM");
    }
    if (a) {
      // Print day...
      Serial.printf("\t%02d\t", rtc.getAlarmDay());
      // ...and time
      Serial.printf("%02d:%02d:%02d.%03d", rtc.getAlarmHours(&period), rtc.getAlarmMinutes(),
                    rtc.getAlarmSeconds(), rtc.getAlarmSubSeconds());
      if (hourFormat == rtc.HOUR_12) {
        Serial.print(period == rtc.AM ? " AM" : " PM");
      }
    }
    Serial.println();
    delay(d);
  }
}