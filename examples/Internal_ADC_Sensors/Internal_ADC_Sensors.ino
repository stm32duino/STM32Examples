#include "stm32yyxx_ll_adc.h"

/* Values available in datasheet */
#define CALX_TEMP 25
#if defined(STM32F1xx)
#define V25       1430
#define AVG_SLOPE 4300
#define VREFINT   1200
#elif defined(STM32F2xx) || defined(STM32F4xx) && !defined(__LL_ADC_CALC_VREFANALOG_VOLTAGE)
#define V25       760
#define AVG_SLOPE 2500
#define VREFINT   1210
#endif

/* Analog read resolution */
#define LL_ADC_RESOLUTION LL_ADC_RESOLUTION_12B
#define ADC_RANGE 4096

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial);
  analogReadResolution(12);
}

static int32_t readVref()
{
#ifdef __LL_ADC_CALC_VREFANALOG_VOLTAGE
  return (__LL_ADC_CALC_VREFANALOG_VOLTAGE(analogRead(AVREF), LL_ADC_RESOLUTION));
#else
  return (VREFINT * ADC_RANGE / analogRead(AVREF)); // ADC sample to mV
#endif
}

#ifdef ATEMP
static int32_t readTempSensor(int32_t VRef)
{
#ifdef __LL_ADC_CALC_TEMPERATURE
  return (__LL_ADC_CALC_TEMPERATURE(VRef, analogRead(ATEMP), LL_ADC_RESOLUTION));
#elif defined(__LL_ADC_CALC_TEMPERATURE_TYP_PARAMS)
  return (__LL_ADC_CALC_TEMPERATURE_TYP_PARAMS(AVG_SLOPE, V25, CALX_TEMP, VRef, analogRead(ATEMP), LL_ADC_RESOLUTION));
#else
  return 0;
#endif
}
#endif

static int32_t readVoltage(int32_t VRef, uint32_t pin)
{
  return (__LL_ADC_CALC_DATA_TO_VOLTAGE(VRef, analogRead(pin), LL_ADC_RESOLUTION));
}

// the loop routine runs over and over again forever:
void loop() {
  // print out the value you read:
  int32_t VRef = readVref();
  Serial.printf("VRef(mv)= %i", VRef);
#ifdef ATEMP
  Serial.printf("\tTemp(°C)= %i", readTempSensor(VRef));
#endif
#ifdef AVBAT
  Serial.printf("\tVbat(mv)= %i", readVoltage(VRef, AVBAT));
#endif
  Serial.printf("\tA0(mv)= %i\n", readVoltage(VRef, A0));
  delay(200);
}
