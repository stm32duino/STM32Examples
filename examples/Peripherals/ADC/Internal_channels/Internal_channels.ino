/*
  Internal_channels
  This example shows how to read then convert to proper Unit the 3 internal channels + A0.

  analogRead() can now be used to read some internal channels with the following definitions:
    ATEMP: internal temperature sensor
    AVREF: VrefInt, internal voltage reference
    AVBAT: Vbat voltage

  A minimum ADC sampling time is required when reading internal channels
  so default is set it to max possible value.
  It can be defined more precisely by defining:
    ADC_SAMPLINGTIME_INTERNAL to the desired ADC sample time.

  ADC_SAMPLINGTIME and ADC_CLOCK_DIV could also be redefined by the variant or using build_opt.h.

  This example is provided "as it" and can require some update mainly for datasheet values.
*/

#include "stm32yyxx_ll_adc.h"

/* Values available in datasheet */
#if defined(STM32C0xx)
#define CALX_TEMP 30
#else
#define CALX_TEMP 25
#endif

#if defined(STM32C0xx)
#define VTEMP      760
#define AVG_SLOPE 2530
#define VREFINT   1212
#elif defined(STM32F1xx)
#define VTEMP     1430
#define AVG_SLOPE 4300
#define VREFINT   1200
#elif defined(STM32F2xx) || defined(STM32F4xx)
#define VTEMP      760
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
#ifdef STM32U0xx
  /* On some devices Internal voltage reference calibration value not programmed
     during production and return 0xFFFF. See errata sheet. */
  if ((uint32_t)(*VREFINT_CAL_ADDR) == 0xFFFF) {
    return 3300U;
  }
#endif
#ifdef __LL_ADC_CALC_VREFANALOG_VOLTAGE
#ifdef STM32U5xx
  return (__LL_ADC_CALC_VREFANALOG_VOLTAGE(ADC1, analogRead(AVREF), LL_ADC_RESOLUTION));
#else
  return (__LL_ADC_CALC_VREFANALOG_VOLTAGE(analogRead(AVREF), LL_ADC_RESOLUTION));
#endif
#else
  return (VREFINT * ADC_RANGE / analogRead(AVREF)); // ADC sample to mV
#endif
}

#ifdef ATEMP
static int32_t readTempSensor(int32_t VRef)
{
#ifdef __LL_ADC_CALC_TEMPERATURE
#ifdef STM32U5xx
  return (__LL_ADC_CALC_TEMPERATURE(ADC1, VRef, analogRead(ATEMP), LL_ADC_RESOLUTION));
#else
  return (__LL_ADC_CALC_TEMPERATURE(VRef, analogRead(ATEMP), LL_ADC_RESOLUTION));
#endif
#elif defined(__LL_ADC_CALC_TEMPERATURE_TYP_PARAMS)
  return (__LL_ADC_CALC_TEMPERATURE_TYP_PARAMS(AVG_SLOPE, VTEMP, CALX_TEMP, VRef, analogRead(ATEMP), LL_ADC_RESOLUTION));
#else
  return 0;
#endif
}
#endif

static int32_t readVoltage(int32_t VRef, uint32_t pin)
{
#ifdef STM32U5xx
  return (__LL_ADC_CALC_DATA_TO_VOLTAGE(ADC1, VRef, analogRead(pin), LL_ADC_RESOLUTION));
#else
  return (__LL_ADC_CALC_DATA_TO_VOLTAGE(VRef, analogRead(pin), LL_ADC_RESOLUTION));
#endif
}

// The loop routine runs over and over again forever:
void loop() {
#if defined(ICACHE) && defined (HAL_ICACHE_MODULE_ENABLED) && !defined(HAL_ICACHE_MODULE_DISABLED)
  bool icache_enabled = false;
  if (HAL_ICACHE_IsEnabled() == 1) {
    icache_enabled = true;
    /* Disable instruction cache prior to internal cacheable memory update */
    if (HAL_ICACHE_Disable() != HAL_OK) {
      Error_Handler();
    }
  }
#endif /* ICACHE && HAL_ICACHE_MODULE_ENABLED && !HAL_ICACHE_MODULE_DISABLED */
  // Print out the value read
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
#if defined(ICACHE) && defined (HAL_ICACHE_MODULE_ENABLED) && !defined(HAL_ICACHE_MODULE_DISABLED)
  if (icache_enabled)
  {
    /* Re-enable instruction cache */
    if (HAL_ICACHE_Enable() != HAL_OK) {
      Error_Handler();
    }
  }
#endif /* ICACHE && HAL_ICACHE_MODULE_ENABLED && !HAL_ICACHE_MODULE_DISABLED */
}