
void rtc_config(STM32RTC::Source_Clock source, STM32RTC::Hour_Format format, const char* date, const char* time) {
  if (!IS_CLOCK_SOURCE(source)) {
    Serial.println("Wrong clock source");
    return;
  }
  if (!IS_HOUR_FORMAT(format)) {
    Serial.println("Wrong hour format");
    return;
  }
  if (rtc.isConfigured())
    rtc.end();

  rtc.setClockSource(source);
  hourFormat = format;
  rtc_setTime(date, time);
}

void rtc_setTime(const char* date, const char* time) {
  initDateTime(date, time);
  rtc.begin(hourFormat);

  // Set the time
  rtc.setHours(hours, period);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);

  // Set the date
  rtc.setWeekDay(weekDay);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  // you can use also
  //rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(weekDay, day, month, year);
}
