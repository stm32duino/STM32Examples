/***************************************
** Brief
***************************************/
/*
  HardwareTimer Non Regression test
  This example is used to verify whether major HardwareTimer usecase are working.

  Designed to work on Nucleo_L476RG
  TIM1 ch1N and ch2 are used for output generation (TIM1_CH1N_PIN, and TIM1_CH2_PIN).
  It is important that both channel are on the same timer, in order to check influence on channel dependant API.
  Management of interruption is done with variable incrementation.
  TIM8 ch1/ch2 are used to measure frequence and duty cycle of TIM1_CH1N output generated signal
  TIM8 ch3/ch4 are used to measure frequence and duty cycle of TIM1_CH2 output generated signal

  Regular channel and complementary channel (TIM1_CH1N) are tested.

  Please wire :
      TIM1_CH1N_PIN to TIM8_CH1_PIN
      TIM1_CH2_PIN to TIM8_CH3_PIN

*/

/***************************************
** Prerequisite
***************************************/
/*
  Please wire :
      TIM1_CH1N_PIN to TIM8_CH1_PIN
      TIM1_CH2_PIN to TIM8_CH3_PIN
*/

/***************************************
** Defines
***************************************/
#if !defined(STM32_CORE_VERSION) || (STM32_CORE_VERSION  < 0x01090000)
#error "Due to API change, this sketch is compatible with STM32_CORE_VERSION  >= 0x01090000"
#endif


#if !defined(ARDUINO_NUCLEO_L476RG)
#error "Sketch is applicable to NUCLEO_L476RG"
#endif
// For L476 only
// TIM ouput
#define TIM1_CH1N_PIN PB13 // PB_13  CN_10 pin 30
#define TIM1_CH2_PIN PA9   // PA_9  D8

#define Output1_channel 1
#define Output2_channel 2

// TIM input
#define TIM8_CH1_PIN PC6 // CN10 pin 4   Use also channel2 for freq/duty measurement
#define Freq1_channelRising 1
#define Freq1_channelFalling 2

#define TIM8_CH3_PIN PC8 // CN10 pin 2   Use also channel4 for freq/duty measurement
#define Freq2_channelRising 3
#define Freq2_channelFalling 4

#define OUTPUT_FREQUENCY 10000 // Hz
#define OUTPUT_DUTY1 20        // percentage
#define OUTPUT_DUTY2 30        // percentage

#define TEMPO_BEFORE_MEASUREMENT 500 // milisecondes
#define TOLERANCE 10                 // Percentage
#define EXPECTED_INTERRUPT_COUNT ((TEMPO_BEFORE_MEASUREMENT * OUTPUT_FREQUENCY) / 1000)

enum Status
{
  PASSED = 0,
  FAILED = 1
};

/***************************************
** Global variables
***************************************/

uint32_t channel1;
uint32_t channel2;

HardwareTimer *MyTim_output;
HardwareTimer *MyTim_input;

uint32_t Output_Update = 0;
uint32_t Output_Compare1 = 0;
uint32_t Output_Compare2 = 0;

volatile uint32_t Frequency1_Measured;
volatile uint32_t Dutycycle1_Measured;
volatile uint32_t LastPeriod1_Capture = 0;
volatile uint32_t Current1_Capture;
volatile uint32_t HighState1_Measured;
volatile uint32_t Frequency2_Measured;
volatile uint32_t Dutycycle2_Measured;
volatile uint32_t LastPeriod2_Capture = 0;
volatile uint32_t Current2_Capture;
volatile uint32_t HighState2_Measured;
uint32_t input_freq = 0;

volatile uint32_t rolloverCompare1Count = 0;
volatile uint32_t rolloverCompare2Count = 0;

uint32_t test_step = 0;
uint32_t test_Status = PASSED;

/***************************************
** Interrupt callback
***************************************/
/********    Output    *****/
void output_Update_IT_callback(void)
{
  Output_Update++;
}

void Output_Compare1_IT_callback(void)
{
  Output_Compare1++;
}

void Output_Compare2_IT_callback(void)
{
  Output_Compare2++;
}

/********    Input 1   *****/
void Input_Capture1_Rising_IT_callback(void)
{
  Current1_Capture = MyTim_input->getCaptureCompare(Freq1_channelRising);
  /* frequency computation */
  if (Current1_Capture > LastPeriod1_Capture)
  {
    Frequency1_Measured = input_freq / (Current1_Capture - LastPeriod1_Capture);
    Dutycycle1_Measured = (HighState1_Measured * 100) / (Current1_Capture - LastPeriod1_Capture);
  }
  else if (Current1_Capture <= LastPeriod1_Capture)
  {
    /* 0x1000 is max overflow value */
    Frequency1_Measured = input_freq / (0x10000 + Current1_Capture - LastPeriod1_Capture);
    Dutycycle1_Measured = (HighState1_Measured * 100) / (0x10000 + Current1_Capture - LastPeriod1_Capture);
  }

  LastPeriod1_Capture = Current1_Capture;
  rolloverCompare1Count = 0;
}

void Input_Capture1_Falling_IT_callback(void)
{
  /* prepare DutyCycle computation */
  Current1_Capture = MyTim_input->getCaptureCompare(Freq1_channelFalling);

  if (Current1_Capture > LastPeriod1_Capture)
  {
    HighState1_Measured = Current1_Capture - LastPeriod1_Capture;
  }
  else if (Current1_Capture <= LastPeriod1_Capture)
  {
    /* 0x1000 is max overflow value */
    HighState1_Measured = 0x10000 + Current1_Capture - LastPeriod1_Capture;
  }
}

/********    Input 2   *****/
void Input_Capture2_Rising_IT_callback(void)
{
  Current2_Capture = MyTim_input->getCaptureCompare(Freq2_channelRising);
  /* frequency computation */
  if (Current2_Capture > LastPeriod2_Capture)
  {
    Frequency2_Measured = input_freq / (Current2_Capture - LastPeriod2_Capture);
    Dutycycle2_Measured = (HighState2_Measured * 100) / (Current2_Capture - LastPeriod2_Capture);
  }
  else if (Current2_Capture <= LastPeriod2_Capture)
  {
    /* 0x1000 is max overflow value */
    Frequency2_Measured = input_freq / (0x10000 + Current2_Capture - LastPeriod2_Capture);
    Dutycycle2_Measured = (HighState2_Measured * 100) / (0x10000 + Current2_Capture - LastPeriod2_Capture);
  }

  LastPeriod2_Capture = Current2_Capture;
  rolloverCompare2Count = 0;
}

void Input_Capture2_Falling_IT_callback(void)
{
  /* prepare DutyCycle computation */
  Current2_Capture = MyTim_input->getCaptureCompare(Freq2_channelFalling);

  if (Current2_Capture > LastPeriod2_Capture)
  {
    HighState2_Measured = Current2_Capture - LastPeriod2_Capture;
  }
  else if (Current2_Capture <= LastPeriod2_Capture)
  {
    /* 0x1000 is max overflow value */
    HighState2_Measured = 0x10000 + Current2_Capture - LastPeriod2_Capture;
  }
}

/********    Input rollover   *****/
/* In case of timer rollover, frequency is to low to be measured set values to 0
   To reduce minimum frequency, it is possible to increase prescaler. But this is at a cost of precision. */
void Rollover_IT_callback(void)
{
  rolloverCompare1Count++;
  rolloverCompare2Count++;
  if (rolloverCompare1Count > 1)
  {
    Frequency1_Measured = 0;
    Dutycycle1_Measured = 0;
  }

  if (rolloverCompare2Count > 1)
  {
    Frequency2_Measured = 0;
    Dutycycle2_Measured = 0;
  }
}

/***************************************
** Verify_output
***************************************/
void Verify_output(uint32_t index, uint32_t expected_freq, uint32_t expected_duty)
{
  int32_t delta_freq;
  int32_t delta_duty;
  volatile uint32_t Frequency_Measured, Dutycycle_Measured;

  if (index == 1)
  {
    // Wait before 1st measurement only
    delay(TEMPO_BEFORE_MEASUREMENT);

    Frequency_Measured = Frequency1_Measured;
    Dutycycle_Measured = Dutycycle1_Measured;
    Serial.print("");
    Serial.print("Step ");
    Serial.println(test_step);
  }
  else
  {
    Frequency_Measured = Frequency2_Measured;
    Dutycycle_Measured = Dutycycle2_Measured;
  }

  delta_freq = expected_freq - Frequency_Measured;
  delta_duty = expected_duty - Dutycycle_Measured;

  Serial.print((String) "  Freq " + index);

  if ((uint32_t)abs(delta_freq) <= ((expected_freq * TOLERANCE) / 100))
  {
    Serial.println((String) "    PASSED :   Freq = " + Frequency_Measured);
  }
  else
  {
    Serial.print((String) "    FAILED : ");
    Serial.print((String) "   expected Freq = " + expected_freq);
    Serial.println((String) "   Measured Freq = " + Frequency_Measured);
    test_Status = FAILED;
  }

  Serial.print((String) "  Duty " + index);
  if ((uint32_t)abs(delta_duty) <= ((expected_duty * TOLERANCE) / 100))
  {
    Serial.println((String) "    PASSED");
  }
  else
  {
    Serial.print((String) "    FAILED : ");
    Serial.print((String) "   expected Duty = " + expected_duty);
    Serial.println((String) "   Measured Duty = " + Dutycycle_Measured);
    test_Status = FAILED;
  }
}

/***************************************
** Verify_output_interrupts
***************************************/
void Verify_output_interrupts(uint32_t expected_update, uint32_t expected_compare1, uint32_t expected_compare2)
{
  int32_t delta_Output_Update = expected_update - Output_Update;
  int32_t delta_Output_Compare1 = expected_compare1 - Output_Compare1;
  int32_t delta_Output_Compare2 = expected_compare2 - Output_Compare2;

  if ((uint32_t)abs(delta_Output_Compare1) > ((expected_compare1 * TOLERANCE) / 100))
  {
    Serial.println((String) "  Interrupt FAILED: CC1 expected " + expected_compare1 + " get " + Output_Compare1 + "delta_Output_Compare1" + delta_Output_Compare1);
    test_Status = FAILED;
  }
  else if ((uint32_t)abs(delta_Output_Compare2) > ((expected_compare2 * TOLERANCE) / 100))
  {
    Serial.println((String) "  Interrupt FAILED: CC2 expected " + expected_compare1 + " get " + Output_Compare2);
    test_Status = FAILED;
  }
  else

    if ((uint32_t)abs(delta_Output_Update) > ((expected_update * TOLERANCE) / 100))
    {
      Serial.println((String) "  Interrupt FAILED: UPDATE expected" + expected_update + " get " + Output_Update);
      test_Status = FAILED;
    }
    else
    {
      Serial.println("  Interrupt PASSED:");
    }
}

void reset_interrupt_count()
{
  Output_Update = 0;
  Output_Compare1 = 0;
  Output_Compare2 = 0;
}

/***************************************
** Setup
***************************************/

void setup()
{
  // Serial monitor
  Serial.begin(115200);

  // No need to configure timer ouput/input pins, it will be done by HardwareTimer configuration

  /*****    Output    *****/
  TIM_TypeDef *Instance_output = TIM1;

  // Instantiate HardwareTimer object. Thanks to 'new' instantiation, HardwareTimer is not destructed when setup function is finished.
  MyTim_output = new HardwareTimer(Instance_output);

  /*****    Input    *****/
  TIM_TypeDef *Instance_input = TIM8;

  // Instantiate HardwareTimer object. Thanks to 'new' instantiation, HardwareTimer is not destructed when setup function is finished.
  MyTim_input = new HardwareTimer(Instance_input);

  // Compute this input scale factor only once
  input_freq = MyTim_input->getTimerClkFreq() / MyTim_input->getPrescaleFactor();

  Serial.println("--- Start HardwareTimer test ---");
  Serial.println("Did you wire:");
  Serial.println("* PB13 to PC6  ?");
  Serial.println("* PA9 to PC8  ?");
  Serial.println("");
}

/***************************************
** loop
***************************************/

void loop()
{

  /*********   Configure input measurement for output test  ***/
  // Configure Input measurement
  MyTim_input->setMode(Freq1_channelRising, TIMER_INPUT_FREQ_DUTY_MEASUREMENT, TIM8_CH1_PIN);

  // With a PrescalerFactor = 1, the minimum frequency value to measure is : TIM counter clock / CCR MAX
  //  = (SystemCoreClock) / 65535
  // Example on Nucleo_L476RG with systemClock at 80MHz, the minimum frequency is around 1,2 khz
  // To reduce minimum frequency, it is possible to increase prescaler. But this is at a cost of precision.
  // The maximum frequency depends on processing of both interruptions and thus depend on board used
  // Example on Nucleo_L476RG with systemClock at 80MHz the interruptions processing is around 10 microseconds and thus Max frequency is around 100kHz
  uint32_t PrescalerFactor = 1;
  MyTim_input->setPrescaleFactor(PrescalerFactor);
  MyTim_input->setOverflow(0x10000); // Max Period value to have the largest possible time to detect rising edge and avoid timer rollover
  MyTim_input->attachInterrupt(Freq1_channelRising, Input_Capture1_Rising_IT_callback);
  MyTim_input->attachInterrupt(Freq1_channelFalling, Input_Capture1_Falling_IT_callback);
  MyTim_input->attachInterrupt(Rollover_IT_callback);

  MyTim_input->setMode(Freq2_channelRising, TIMER_INPUT_FREQ_DUTY_MEASUREMENT, TIM8_CH3_PIN);
  MyTim_input->attachInterrupt(Freq2_channelRising, Input_Capture2_Rising_IT_callback);
  MyTim_input->attachInterrupt(Freq2_channelFalling, Input_Capture2_Falling_IT_callback);

  MyTim_input->resume();

  delay(1000);

  /*********   Output test  ***/
  test_step++;

  MyTim_output->setOverflow((1000000 / OUTPUT_FREQUENCY), MICROSEC_FORMAT);
  MyTim_output->attachInterrupt(output_Update_IT_callback);
  MyTim_output->resume();
  reset_interrupt_count();
  Verify_output(1, 0, 0);
  Verify_output(2, 0, 0);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, 0, 0);
  test_step++;

  MyTim_output->detachInterrupt();
  reset_interrupt_count();
  Verify_output(1, 0, 0);
  Verify_output(2, 0, 0);
  Verify_output_interrupts(0, 0, 0);
  test_step++;

  MyTim_output->setMode(Output1_channel, TIMER_OUTPUT_COMPARE_PWM1, TIM1_CH1N_PIN);
  MyTim_output->setOverflow((1000000 / OUTPUT_FREQUENCY), MICROSEC_FORMAT);
  MyTim_output->setCaptureCompare(Output1_channel, OUTPUT_DUTY1, PERCENT_COMPARE_FORMAT);
  MyTim_output->resume();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, 0, 0);
  test_step++;

  MyTim_output->setMode(Output1_channel, TIMER_OUTPUT_COMPARE_TOGGLE, TIM1_CH1N_PIN);
  MyTim_output->resume();
  Verify_output(1, OUTPUT_FREQUENCY / 2, 50); // in PWM2, output is the complementary of PW1
  Verify_output(2, 0, 0);
  test_step++;

  MyTim_output->setMode(Output1_channel, TIMER_OUTPUT_COMPARE_PWM2, TIM1_CH1N_PIN);
  MyTim_output->resume();
  Verify_output(1, OUTPUT_FREQUENCY, 100 - OUTPUT_DUTY1); // in PWM2, output is the complementary of PW1
  Verify_output(2, 0, 0);
  test_step++;

  MyTim_output->setMode(Output1_channel, TIMER_OUTPUT_COMPARE_PWM1, TIM1_CH1N_PIN);
  MyTim_output->resume();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, 0, 0);
  test_step++;

  MyTim_output->setMode(Output2_channel, TIMER_OUTPUT_COMPARE_PWM1, TIM1_CH2_PIN);
  MyTim_output->setCaptureCompare(Output2_channel, OUTPUT_DUTY2, PERCENT_COMPARE_FORMAT);
  MyTim_output->resumeChannel(Output2_channel);
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  test_step++;

  MyTim_output->pauseChannel(Output1_channel);
  Verify_output(1, 0, 0);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  test_step++;

  MyTim_output->resumeChannel(Output1_channel);
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(0, 0, 0); // interrupt not yet activated
  test_step++;

  MyTim_output->attachInterrupt(output_Update_IT_callback);
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, 0, 0); // interrupt activated for update only
  reset_interrupt_count();
  test_step++;

  MyTim_output->pauseChannel(Output1_channel);
  MyTim_output->setMode(Output1_channel, TIMER_OUTPUT_COMPARE);
  MyTim_output->resumeChannel(Output1_channel);
  Verify_output(1, 0, 0); // in PWM2, output is the complementary of PW1
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, 0, 0); // interrupt activated for update only
  test_step++;

  MyTim_output->pauseChannel(Output1_channel);
  MyTim_output->setMode(Output1_channel, TIMER_OUTPUT_COMPARE_PWM1, TIM1_CH1N_PIN);
  MyTim_output->resumeChannel(Output1_channel);
  MyTim_output->attachInterrupt(Output1_channel, Output_Compare1_IT_callback);
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT, 0);

  test_step++;

  MyTim_output->attachInterrupt(Output2_channel, Output_Compare2_IT_callback);
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT);

  test_step++;

  MyTim_output->pause();
  reset_interrupt_count();
  Verify_output(1, 0, 0);
  Verify_output(2, 0, 0);
  Verify_output_interrupts(0, 0, 0);

  test_step++;

  MyTim_output->resumeChannel(Output1_channel);
  MyTim_output->resumeChannel(Output2_channel);
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(0, EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT);
  test_step++;

  MyTim_output->resume();
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT);
  test_step++;

  MyTim_output->pauseChannel(Output1_channel);
  MyTim_output->pauseChannel(Output2_channel);
  reset_interrupt_count();
  Verify_output(1, 0, 0);
  Verify_output(2, 0, 0);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, 0, 0);
  test_step++;

  MyTim_output->resume();
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT);
  test_step++;

  PrescalerFactor = MyTim_output->getPrescaleFactor();
  MyTim_output->setPrescaleFactor(PrescalerFactor + 1);
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY / 2, OUTPUT_DUTY1);
  Verify_output(2, OUTPUT_FREQUENCY / 2, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT / 2, EXPECTED_INTERRUPT_COUNT / 2, EXPECTED_INTERRUPT_COUNT / 2);
  test_step++;

  PrescalerFactor = MyTim_output->getPrescaleFactor();
  MyTim_output->setPrescaleFactor(PrescalerFactor - 1);
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1); // suppose initial prescaler = 1
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT, EXPECTED_INTERRUPT_COUNT);
  test_step++;

  if (!MyTim_output->hasInterrupt() || !MyTim_output->hasInterrupt(Output1_channel) || !MyTim_output->hasInterrupt(Output2_channel))
  {
    Serial.println((String) "  hasInterrupt() FAILED" + MyTim_output->hasInterrupt() + "  " + MyTim_output->hasInterrupt(Output1_channel) + " " + MyTim_output->hasInterrupt(Output1_channel));
    test_Status = FAILED;
  }

  MyTim_output->detachInterrupt(Output1_channel);
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1); // suppose initial prescaler = 1
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, 0, EXPECTED_INTERRUPT_COUNT);
  if (!MyTim_output->hasInterrupt() || MyTim_output->hasInterrupt(Output1_channel) || !MyTim_output->hasInterrupt(Output2_channel))
  {
    Serial.println("  hasInterrupt() FAILED");
    test_Status = FAILED;
  }
  test_step++;

  MyTim_output->detachInterrupt(Output2_channel);
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1); // suppose initial prescaler = 1
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(EXPECTED_INTERRUPT_COUNT, 0, 0);
  if (!MyTim_output->hasInterrupt() || MyTim_output->hasInterrupt(Output1_channel) || MyTim_output->hasInterrupt(Output2_channel))
  {
    Serial.println("  hasInterrupt() FAILED");
    test_Status = FAILED;
  }
  test_step++;

  MyTim_output->detachInterrupt();
  reset_interrupt_count();
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1); // suppose initial prescaler = 1
  Verify_output(2, OUTPUT_FREQUENCY, OUTPUT_DUTY2);
  Verify_output_interrupts(0, 0, 0);
  reset_interrupt_count();
  if (MyTim_output->hasInterrupt() || MyTim_output->hasInterrupt(Output1_channel) || MyTim_output->hasInterrupt(Output1_channel))
  {
    Serial.println("  hasInterrupt() FAILED");
    test_Status = FAILED;
  }
  test_step++;

  MyTim_output->setPWM(Output1_channel, TIM1_CH1N_PIN, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  Verify_output(1, OUTPUT_FREQUENCY, OUTPUT_DUTY1);
  test_step++;

  Serial.print((String) "************* Status:");
  if (test_Status == PASSED)
  {
    Serial.println("  PASSED **********");
  }
  else
  {
    Serial.println("  FAILED **********");
  }

  while (1)
  {
  };
}
