/*
  Timebase callback
  This example shows how to configure HardwareTimer to execute a callback with some parameter at regular interval.
  Callback toggles pin.
  Once configured, there is only CPU load for callbacks executions.
*/

#if !defined(STM32_CORE_VERSION) || (STM32_CORE_VERSION  < 0x01090000)
#error "Due to API change, this sketch is compatible with STM32_CORE_VERSION  >= 0x01090000"
#endif

#if defined(LED_BUILTIN)
#define pin  LED_BUILTIN
#else
#define pin  D2
#endif


uint32_t MyData = 1; // Parameter used for callback is arbitrarily a pointer to uint32_t, it could be of other type.

// Every second, print on serial MyData. And increment it.
void Update_IT_callback(uint32_t* data)
{
  Serial.println(*data);
  *data = *data + 1;
}

void setup()
{
  Serial.begin(9600); 
#if defined(TIM1)
  TIM_TypeDef *Instance = TIM1;
#else
  TIM_TypeDef *Instance = TIM2;
#endif

  // Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
  HardwareTimer *MyTim = new HardwareTimer(Instance);

  // configure pin in output mode
  pinMode(pin, OUTPUT);

  MyTim->setOverflow(1, HERTZ_FORMAT); // 1 Hz
  MyTim->attachInterrupt(std::bind(Update_IT_callback, &MyData)); // bind argument to callback: When Update_IT_callback is called MyData will be given as argument
  MyTim->resume();
}


void loop()
{
  /* Nothing to do all is done by hardware. Even no interrupt required. */
}