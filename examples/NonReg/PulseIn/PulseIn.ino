/*
  pulseIn

  This sketch allows to test pulseIn() and pulseInLong() functions.

  A PWM signal is generated on 'pwm' pin and then measures are performed on
  'in' pin for the HIGH and LOW state.
  The min and max value for both are displayed on the Serial console.

  Creation 15 Sep 2017
  by Frederic Pillon

  This example code is in the public domain.

  https://www.arduino.cc/en/Reference/PulseIn
*/


// Use pulseInLong()?
#define USE_PULSEINLONG 1

static uint32_t pwm = 9;
static uint32_t in = 4;
static uint32_t state = HIGH;

void setup() {
  pinMode(in, INPUT);
  Serial.begin(115200);
  analogWrite(pwm, 127);
}
static uint32_t min_duration_low = (uint32_t)(-1);
static uint32_t max_duration_low = 0;
static uint32_t min_duration_high = (uint32_t)(-1);
static uint32_t max_duration_high = 0;
#if USE_PULSEINLONG
static uint32_t min_durationLong_low = (uint32_t)(-1);
static uint32_t max_durationLong_low = 0;
static uint32_t min_durationLong_high = (uint32_t)(-1);
static uint32_t max_durationLong_high = 0;
#endif

void loop() {
  uint32_t duration = 0;
  duration = pulseIn(in, state);
#if USE_PULSEINLONG
  uint32_t durationLong = 0;
  durationLong = pulseInLong(in, state);
#endif
  if (state == HIGH) {
    Serial.print("PulseIn HIGH state duration:");
    min_duration_high=min(min_duration_high,duration);
    max_duration_high=max(max_duration_high,duration);
    Serial.print(" min= ");
    Serial.print(min_duration_high);
    Serial.print(" max= ");
    Serial.print(max_duration_high);
#if USE_PULSEINLONG
    min_durationLong_high=min(min_durationLong_high,durationLong);
    max_durationLong_high=max(max_durationLong_high,durationLong);
    Serial.print(" min (long)= ");
    Serial.print(min_durationLong_high);
    Serial.print(" max (long)= ");
    Serial.print(max_durationLong_high);
#endif
    state = LOW;
  }
  else {
    Serial.print("PulseIn LOW  state duration:");
    min_duration_low=min(min_duration_low,duration);
    max_duration_low=max(max_duration_low,duration);
    Serial.print(" min= ");
    Serial.print(min_duration_low);
    Serial.print(" max= ");
    Serial.print(max_duration_low);
#if USE_PULSEINLONG
    min_durationLong_low=min(min_durationLong_low,durationLong);
    max_durationLong_low=max(max_durationLong_low,durationLong);
    Serial.print(" min (long)= ");
    Serial.print(min_durationLong_low);
    Serial.print(" max (long)= ");
    Serial.print(max_durationLong_low);
#endif
    state = HIGH;
  }
  Serial.println();
  delay(1000);
}
