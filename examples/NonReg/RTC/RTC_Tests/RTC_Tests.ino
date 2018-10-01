/*
  RTC_Tests

  This sketch allows to test STM32RTC library API.

  Creation 25 Apr 2018
  by Frederic Pillon for STMicroelectronics

  This example code is in the public domain.

  https://github.com/stm32duino/STM32RTC

*/

#include <STM32RTC.h>

/* Get the rtc object */
STM32RTC& rtc = STM32RTC::getInstance();

/* Change these values to set the current initial time
 *
 * format: date: "Dec 31 2017" and time: "23:59:56"
 * by default use built date and time
 */
static const char* mydate = __DATE__;
static const char* mytime = __TIME__;
//static const char* mydate = "Dec 31 2017";
//static const char* mytime = "23:59:56";

/* Change these values to set user (a)synchronous prescalers
 * value to test.
 *
 * Default 99/9999 are for RTCCLK = 1MHz
 * Example for DISCO_F746NG with a HSE of 25MHz
 * HSE divider will be 25 and RTCCLK will be 1MHz. (HSE/HSEdiv)
 *
 * To have a calendar clock of 1 Hz:
 * clk = RTCCLK / ((predA +1) * (predS +1))
 * clk = 1000000 / ((99 +1) * (9999+1)) = 1 Hz
 */
static int8_t userPredA = 99;
static int16_t userPredS = 9999;

/* */
static byte seconds = 0;
static byte minutes = 0;
static byte hours = 0;

static byte weekDay = 1;
static byte day = 0;
static byte month = 0;
static byte year = 0;

static STM32RTC::Hour_Format hourFormat = STM32RTC::HOUR_24;
static STM32RTC::AM_PM period = STM32RTC::AM;

#ifndef STM32F1xx
static STM32RTC::Alarm_Match SS_MATCH = STM32RTC::MATCH_SS;
static STM32RTC::Alarm_Match MMSS_MATCH = STM32RTC::MATCH_MMSS;
static STM32RTC::Alarm_Match HHMMSS_MATCH = STM32RTC::MATCH_HHMMSS;
#endif
static STM32RTC::Alarm_Match DHHMMSS_MATCH = STM32RTC::MATCH_DHHMMSS;

void setup()
{
  Serial.begin(115200);
  while(!Serial) {}
}

void loop()
{
  int c;  // Serial input
  STM32RTC::Source_Clock clkSource = rtc.LSI_CLOCK;

  // Select RTC clock source: LSI_CLOCK, LSE_CLOCK or HSE_CLOCK.
  Serial.println("Select clock Source:");
  Serial.println("1- LSI_CLOCK");
  Serial.println("2- LSE_CLOCK");
  Serial.println("3- HSE_CLOCK");
  Serial.println();
  // get input
  while (1) {
    while (Serial.read() >= 0) {}
    Serial.print("Enter number [1-3] ");
    while ((c = Serial.read()) < 0) {}
    Serial.println((char)c);
    if (c < '1' || c > '3') {
      Serial.println("Invalid input");
      continue;
    }
    switch(c) {
      case '1':
      default:
        Serial.println("Test will use LSI_CLOCK");
        clkSource = rtc.LSI_CLOCK;
        break;
      case '2':
        Serial.println("Test will use LSE_CLOCK");
        clkSource = rtc.LSE_CLOCK;
        break;
      case '3':
        Serial.println("Test will use HSE_CLOCK");
        clkSource = rtc.HSE_CLOCK;
        break;
    }
    break;
  }

  Serial.println("Testing asynchronous and synchronous prescaler setting");
  int8_t a;
  int16_t s;
  rtc.getPrediv(&a, &s);
  Serial.print("Async/Sync for default LSI clock: ");
  Serial.print(a); Serial.print('/'); Serial.println(s);
  rtc_config(clkSource, rtc.HOUR_24, mydate, mytime);
  Serial.print("Async/Sync for selected clock: ");
  rtc.getPrediv(&a, &s);
  Serial.print(a); Serial.print('/'); Serial.println(s);
  rtc.end();

  if(clkSource == rtc.HSE_CLOCK) {
    Serial.print("User Async/Sync set to ");
    Serial.print(userPredA);
    Serial.print("/");
    Serial.print(userPredS);
    Serial.print(": ");
    rtc.setPrediv(userPredA, userPredS);
    rtc_config(clkSource, rtc.HOUR_24, mydate, mytime);
    rtc.getPrediv(&a, &s);
    Serial.print(a); Serial.print('/'); Serial.println(s);
    printDateTime(10, 1000, false);
  }

  Serial.print("User Async/Sync reset use the computed one: ");
  rtc.setPrediv(-1, -1);
  rtc_config(clkSource, rtc.HOUR_24, mydate, mytime);
  rtc.getPrediv(&a, &s);
  Serial.print(a); Serial.print('/'); Serial.println(s);

  // Check date change
  Serial.println("Testing date and time");
  Serial.println("24H format, new year");
  rtc_config(clkSource, rtc.HOUR_24, "Dec 31 2017", "23:59:56");
  printDateTime(8, 1000, false);
  Serial.println();

#ifndef STM32F1xx
  Serial.println("12H format, new year");
  rtc_config(clkSource, rtc.HOUR_12, "Dec 31 2017", "23:59:56");
  printDateTime(8, 1000, false);
  Serial.println();

  Serial.println("12H format, from AM to PM");
  rtc_config(clkSource, rtc.HOUR_12, "Dec 31 2017", "11:59:56");
  printDateTime(8, 1000, false);
  Serial.println();
#endif //STM32F1xx

  Serial.println("Using Epoch API, set to Jan 1, 2016");
  rtc.setEpoch(1451606400); // Jan 1, 2016
  for (uint32_t i=0; i<8; i++) {
    Serial.print("Unix time = ");
    Serial.println(rtc.getEpoch());
    Serial.print("Seconds since Jan 1 2000 = ");
    Serial.println(rtc.getY2kEpoch());
    printDateTime(1, 1000, false);
  }

  Serial.println("\nTesting alarm");
  rtc_config(clkSource, rtc.HOUR_24, mydate, mytime);
  byte alarmSeconds = ((seconds+5)<60) ? seconds+5 : 5;
  byte alarmMinutes = ((seconds+5)<60) ? minutes : ((minutes+1)<60) ? minutes+1 : 0;
  byte alarmHours = ((seconds+5)<60) ? hours : ((minutes+1)<60) ? hours : ((hours+1)<24) ? hours+1 : 0;
  byte alarmDay = (hours==alarmHours)? day: ((day+1)<=31) ? day+1 : 1;

#ifndef STM32F1xx
  rtc.attachInterrupt(alarmMatch, (void*)&SS_MATCH);
  rtc.setAlarmSeconds(alarmSeconds);
  rtc.enableAlarm(rtc.MATCH_SS);
  printDateTime(20, 1000, true);
  rtc.disableAlarm();
  rtc.detachInterrupt();

  Serial.println("\nEvery hours");
  rtc_setTime(mydate, mytime);
  rtc.attachInterrupt(alarmMatch, (void*)&MMSS_MATCH);
  rtc.setAlarmMinutes(alarmMinutes);
  rtc.enableAlarm(rtc.MATCH_MMSS);
  printDateTime(20, 1000, true);
  rtc.disableAlarm();
  rtc.detachInterrupt();

  Serial.println("\nEvery day");
  rtc_setTime(mydate, mytime);
  rtc.attachInterrupt(alarmMatch, (void*)&HHMMSS_MATCH);
  rtc.setAlarmHours(alarmHours);
  rtc.enableAlarm(rtc.MATCH_HHMMSS);
  printDateTime(20, 1000, true);
  rtc.disableAlarm();
  rtc.detachInterrupt();
#endif // STM32F1xx

  Serial.println("\nEvery month");
  rtc_setTime(mydate, mytime);
  rtc.attachInterrupt(alarmMatch, (void*)&DHHMMSS_MATCH);
  rtc.setAlarmTime(alarmHours, alarmMinutes, alarmSeconds);
  rtc.setAlarmDay(alarmDay);
  rtc.enableAlarm(rtc.MATCH_DHHMMSS);
  printDateTime(20, 1000, true);
  rtc.disableAlarm();
  rtc.detachInterrupt();

  rtc_config(clkSource, rtc.HOUR_24, mydate, mytime);
  Serial.println("\nAlarm disabled. Printing each 10s.");
  printDateTime(-1, 10000, false);
}

void alarmMatch(void *data)
{
  STM32RTC::Alarm_Match m = *(STM32RTC::Alarm_Match*)data;

  Serial.print("Alarm Match ");
  switch(m) {
      case STM32RTC::MATCH_OFF:
        Serial.println("MATCH_OFF could not happen");
        break;
      case STM32RTC::MATCH_YYMMDDHHMMSS://kept for compatibility
      case STM32RTC::MATCH_MMDDHHMMSS:  //kept for compatibility
      case STM32RTC::MATCH_DHHMMSS:
        Serial.println("MATCH_DHHMMSS");
        rtc.setMonth(((rtc.getMonth()+1)<13)? rtc.getMonth()+1: 1);
        rtc.setEpoch(rtc.getEpoch()- 10);
        break;
      case STM32RTC::MATCH_HHMMSS:
        Serial.println("MATCH_HHMMSS");
        rtc.setEpoch(rtc.getEpoch()+86395);
        break;
      case STM32RTC::MATCH_MMSS:
        Serial.println("MATCH_MMSS");
        rtc.setEpoch(rtc.getEpoch()+3595);
        break;
      case STM32RTC::MATCH_SS:
        Serial.println("MATCH_SS");
        rtc.setEpoch(rtc.getEpoch()+55);
        break;
      default:
        Serial.println("Unknown STM32RTC::Alarm_Match type");
      break;
  }
}

