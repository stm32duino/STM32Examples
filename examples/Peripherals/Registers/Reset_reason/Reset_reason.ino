/* Last Reset Reason Sketch
* This sketch will determine what caused the last reset on the STM32 MCU. Most microcontrollers
* have a register dedicated to storing the last reason of the chip, weather being from a 
* low power condition, software caused brown-out. Test it by resetting the MCU via the USER button
* which triggers the Reset_my_MCU() function or unplug the USB cable and repluggit back. Adjust your 
* UART, USER Button pin and registers accordingly. Use the MCU's datasheet and/or stm32yyyxxx.h for reference.
* The code is provided "as is" with no liability.
*/

#include "stm32yyxx_ll_rcc.h"
#include "IWatchdog.h"

#define USER_BTN_PIN USER_BTN  // Adjust this for your board

// Enumerator for combining reset flag bits into one byte then display them
enum reset_reason {
  UNKNOWN_RESET = 0,
  BROWN_OUT = 1 << 0,
  NRST_PIN = 1 << 1,
  SOFTWARE_RST = 1 << 2,
  INDEPENDENT_WDG = 1 << 3,
  WINDOW_WDG = 1 << 4,
  LOW_POWER = 1 << 5,
  OPTION_BYTE_LOADER = 1 << 6,
  POWER_ON_DOWN = 1 << 7
};

reset_reason last_reset_reason = UNKNOWN_RESET;
static int default_button_state = LOW;

void Reset_My_MCU() {
  // There are a few reset conditions.
  // Keep the one you wish to use and comment out the others.
  // Below is the Software reset condition
  // NVIC_SystemReset();

  // Below is the Watchdog Timer reset condition
  IWatchdog.begin(1000);  //1ms tick then reset
  while (1)
    ;  // Wait for reset
}

void setup() {
  pinMode(USER_BTN_PIN, INPUT);
  default_button_state = digitalRead(USER_BTN_PIN);
  Serial.begin(115200);
  while (!Serial)
    ;  // Wait for Serial

#ifdef RCC_CSR_BORRSTF
  if (LL_RCC_IsActiveFlag_BORRST()) last_reset_reason = (reset_reason)(last_reset_reason | BROWN_OUT);
#endif
  if (LL_RCC_IsActiveFlag_PINRST()) last_reset_reason = (reset_reason)(last_reset_reason | NRST_PIN);
  if (LL_RCC_IsActiveFlag_SFTRST()) last_reset_reason = (reset_reason)(last_reset_reason | SOFTWARE_RST);
#if defined(RCC_RSR_IWDG1RSTF)
  if (LL_RCC_IsActiveFlag_IWDG1RST()) last_reset_reason = (reset_reason)(last_reset_reason | INDEPENDENT_WDG);
#else
  if (LL_RCC_IsActiveFlag_IWDGRST()) last_reset_reason = (reset_reason)(last_reset_reason | INDEPENDENT_WDG);
#endif
#if defined(RCC_RSR_WWDG1RSTF)
  if (LL_RCC_IsActiveFlag_WWDG1RST()) last_reset_reason = (reset_reason)(last_reset_reason | WINDOW_WDG);
#else
  if (LL_RCC_IsActiveFlag_WWDGRST()) last_reset_reason = (reset_reason)(last_reset_reason | WINDOW_WDG);
#endif
  if (LL_RCC_IsActiveFlag_LPWRRST()) last_reset_reason = (reset_reason)(last_reset_reason | LOW_POWER);
#if defined(RCC_CSR_OBLRSTF) || defined(RCC_CSR2_OBLRSTF)
  if (LL_RCC_IsActiveFlag_OBLRST()) last_reset_reason = (reset_reason)(last_reset_reason | OPTION_BYTE_LOADER);
#endif
#ifdef RCC_CSR_PORRSTF
  if (LL_RCC_IsActiveFlag_PORRST()) last_reset_reason = (reset_reason)(last_reset_reason | POWER_ON_DOWN);
#endif

  // Clear reset flags
  LL_RCC_ClearResetFlags();
}

void loop() {
  Serial.println("Last reset reason:");

  if (last_reset_reason & BROWN_OUT) Serial.println(" - Brown-out reset");
  if (last_reset_reason & SOFTWARE_RST) Serial.println(" - Software reset");
  if (last_reset_reason & INDEPENDENT_WDG) Serial.println(" - Independent Watchdog reset");
  if (last_reset_reason & WINDOW_WDG) Serial.println(" - Window Watchdog reset");
  if (last_reset_reason & LOW_POWER) Serial.println(" - Low-power reset");
  if (last_reset_reason & OPTION_BYTE_LOADER) Serial.println(" - Option byte loader reset");
  if (last_reset_reason & NRST_PIN) Serial.println(" - Pin reset (NRST or software)");  //last case so the rest take precedence before issuing NRST
  if (last_reset_reason & POWER_ON_DOWN) Serial.println(" - Power on or power down reset");
  if (last_reset_reason == UNKNOWN_RESET) Serial.println(" - Unknown or no flags set");
  last_reset_reason = UNKNOWN_RESET;

  // Trigger software reset on button press
  if (digitalRead(USER_BTN_PIN) != default_button_state) {
    Serial.println("Button pressed → Triggering reset...");
    delay(300);  // Debounce
    Reset_My_MCU();
  }

  delay(1000);
}
