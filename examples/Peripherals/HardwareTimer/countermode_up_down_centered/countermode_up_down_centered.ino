/*
  Countermode up down centered
  This example shows how to configure HardwareTimer's counter mode.
  
  The following countermodes are demonstrated:
  LL_TIM_COUNTERMODE_UP
   _ — ‾ | _ — ‾ | _ — ‾ | _ — ‾ |
  
  LL_TIM_COUNTERMODE_DOWN
   ‾ — _ | ‾ — _ | ‾ — _ | ‾ — _ |
  
  LL_TIM_COUNTERMODE_CENTER_UP_DOWN (with over/undeflow callback)
  _ — ‾ | ‾ — _ | _ — ‾ | ‾ — _ |

  LL_TIM_COUNTERMODE_CENTER_UP_DOWN (with only undeflow callback)
  _ — ‾ ‾ — _ | _ — ‾ ‾ — _ |
*/

/*

*/

#if !defined(STM32_CORE_VERSION) || (STM32_CORE_VERSION < 0x01090000)
#error "Due to API change, this sketch is compatible with STM32_CORE_VERSION  >= 0x01090000"
#endif

// Change Serial1 pins or use Serial (if available)
#define TX PB3
#define RX PB4
HardwareSerial Serial1(RX, TX);
HardwareTimer *timer;

// this callback occurs when the timer reaches is maximum value and resets to 0
void overflow_interrupt(void)
{ 
  // Note: It is bad practice to do expensive operations like Serial.print in an interrupt!             
  Serial1.print(" |");
}

// this callback occurs when timer counter == the ch1's compare register value (set to 25% of overflow below)
void compare_ch1_interrupt(void)
{ 
  Serial1.print(" _");
}

// this callback occurs when timer counter == the ch2's compare register value (set to 50% below)
void compare_ch2_interrupt(void)
{ 
  Serial1.print(" —");
}

// this callback occurs when timer counter == the ch3's compare register value (set to 75% below)
void compare_ch3_interrupt(void)
{ 
  Serial1.print(" ‾");
}

void setup()
{
  Serial1.begin(115200);
  delay(750);
   
  TIM_TypeDef *inst = TIM1;  // not all timers can count down/updown.  Timer1 should be good.
  timer = new HardwareTimer(inst);
  timer->setOverflow(1, HERTZ_FORMAT); // this will setup prescaler and overflow so that overflow event happens once a second
  timer->attachInterrupt(overflow_interrupt); //this callback happens when timer over/underflows

  Serial1.print("Timer base freq: "); Serial1.println(timer->getTimerClkFreq());  // some timers are slower (e.g.DIV2) 
  Serial1.print("Prescaler: "); Serial1.println(timer->getPrescaleFactor()); // the prescaler was set as a side affect of defining 1hz overflow above
  Serial1.print("Overflow: "); Serial1.println(timer->getOverflow()); //the overflow (max count) was set as a side affect of defining 1hz overflow above

  // prepare timer1 channel1 at 25% of oveflow
  timer->setMode(1, TIMER_OUTPUT_COMPARE);
  timer->setCaptureCompare(1, 25, PERCENT_COMPARE_FORMAT);
  timer->attachInterrupt(1, compare_ch1_interrupt);

  // prepare timer1 channel2 at 50% of oveflow
  timer->setMode(2, TIMER_OUTPUT_COMPARE);
  timer->setCaptureCompare(2, 50, PERCENT_COMPARE_FORMAT);
  timer->attachInterrupt(2, compare_ch2_interrupt);

  // prepare timer1 channel3 at 75% of oveflow
  timer->setMode(3, TIMER_OUTPUT_COMPARE);
  timer->setCaptureCompare(3, 75, PERCENT_COMPARE_FORMAT);
  timer->attachInterrupt(3, compare_ch3_interrupt);

  // we are not using timer1 channel4!

  // start the timer
  timer->resume();

}

// Note: We could remove all the code below and it would still print!
void loop()
{
  // _ — ‾ | _ — ‾ | _ — ‾ | _ — ‾ |
  Serial1.println("\ncountermode up (default)");
  delay(4001); 

  // ‾ — _ | ‾ — _ | ‾ — _ | ‾ — _ |
  Serial1.println("\ncountermode down");
  LL_TIM_SetCounterMode(timer->getHandle()->Instance, LL_TIM_COUNTERMODE_DOWN);
  delay(4001); // Wait for 4 overflows 

  // _ — ‾ | ‾ — _ | _ — ‾ | ‾ — _ |
  Serial1.println("\ncountermode center up/down (overflow and underflow events)");
  LL_TIM_SetCounterMode(timer->getHandle()->Instance, LL_TIM_COUNTERMODE_CENTER_UP_DOWN);
  timer->refresh(); // this makes the above command happen immediately, otherwise it happens after next overflow
  delay(4001); // Wait for 4 overflows

  // _ — ‾ ‾ — _ | _ — ‾ ‾ — _ |
  Serial1.println("\ncountermode center up/down (underflow only events)");
  LL_TIM_SetCounterMode(timer->getHandle()->Instance, LL_TIM_COUNTERMODE_CENTER_UP_DOWN);
  LL_TIM_SetRepetitionCounter(timer->getHandle()->Instance,1);
  timer->refresh(); // refresh() makes the above command happen immediately, otherwise it happens after next overflow
  delay(4001); // Wait for 4 overflows
  
  // reset to default countermod (up) for next loop
  LL_TIM_SetCounterMode(timer->getHandle()->Instance, LL_TIM_COUNTERMODE_UP);
  LL_TIM_SetRepetitionCounter(timer->getHandle()->Instance,0);
  timer->refresh();
}
