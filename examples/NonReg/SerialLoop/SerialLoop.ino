/*
  SerialLoop

  Test all hardware serial configs at several speeds
  by sending all possible data range and comparing Tx/Rx.

  Creation 10 Jul 2017
  by Frederic Pillon <frederic.pillon@st.com>

  This example code is in the public domain.
*/

#include "utils.h"

/*
 * 1 - Connect Rx/Tx of the desired Serial
 * 2 - Define UART_TEST_INSTANCE
 *     ! Ensure Serial feature is enabled thanks 'U(S)ART support menu'!
 * 3 - Optionnal: comment unwanted speed in serialSpeed array.
 */
#define SERIAL_PORT_TESTED SerialTest
#if defined(SERIAL_UART_INSTANCE) && (SERIAL_UART_INSTANCE != 1)
#define UART_TEST_INSTANCE USART1
#elif defined(USART2_BASE)
#define UART_TEST_INSTANCE USART2
#else
#define UART_TEST_INSTANCE LPUART1
#endif
// or
//HardwareSerial Serialx(rxpin, txpin)
HardwareSerial SERIAL_PORT_TESTED(UART_TEST_INSTANCE);

#define DECL_CONFIG(x)  {#x, x}

typedef struct serialTest_s serialTest;
struct serialTest_s {
  const char* name;
  uint32_t config;
};

static serialTest serialConfig[] = {
#ifdef UART_WORDLENGTH_7B
DECL_CONFIG(SERIAL_7N1),
DECL_CONFIG(SERIAL_7N2),
DECL_CONFIG(SERIAL_6E1),
DECL_CONFIG(SERIAL_6E2),
DECL_CONFIG(SERIAL_6O1),
DECL_CONFIG(SERIAL_6O2),
#endif
DECL_CONFIG(SERIAL_8N1),
DECL_CONFIG(SERIAL_8N2),
DECL_CONFIG(SERIAL_7E1),
DECL_CONFIG(SERIAL_8E1),
DECL_CONFIG(SERIAL_7E2),
DECL_CONFIG(SERIAL_7O1),
DECL_CONFIG(SERIAL_8O1),
DECL_CONFIG(SERIAL_7O2),
DECL_CONFIG(SERIAL_8O2),
DECL_CONFIG(SERIAL_8E2)
};

static uint32_t serialSpeed[] = {
  300,
  1200,
  2400,
  4800,
  9600,
  19200,
  38400,
  57600,
  74880,
  115200,
  230400,
  250000,
  500000,
  1000000,
  2000000
};

static uint32_t start_time = 0;
static uint32_t configCur = 0;
static uint32_t configNb = sizeof(serialConfig)/sizeof(serialTest);
static uint32_t speedNb = sizeof(serialSpeed)/sizeof(uint32_t);
static uint32_t nbTestOK = 0;
static uint32_t nbTestKO = 0;

uint32_t dataMask(uint32_t config) {
  uint32_t databits = 0;
  switch(config & 0x07) {
    case 0x02:
      databits = 6;
      break;
    case 0x04:
      databits = 7;
      break;
    case 0x06:
      databits = 8;
      break;
    default:
      databits = 0;
      break;
  }
  return ((1 << databits) - 1);
}

void test_uart(int val)
{
  int recval = 0;
  SERIAL_PORT_TESTED.write(val);
  delay(10);
  while(SERIAL_PORT_TESTED.available()){
    recval = SERIAL_PORT_TESTED.read();
  }
  if(val == recval) {
    nbTestOK++;
  }
  else {
    SERIAL_PORT_MONITOR.print("Send: 0x");
    SERIAL_PORT_MONITOR.print(val,HEX);
    SERIAL_PORT_MONITOR.print("\tReceived: 0x");
    SERIAL_PORT_MONITOR.print(recval,HEX);
    SERIAL_PORT_MONITOR.println(" --> KO <--");
    nbTestKO++;
  }
}

void setup() {
  SERIAL_PORT_MONITOR.begin(115200);
  while(!SERIAL_PORT_MONITOR);
  SERIAL_PORT_MONITOR.print("SerialLoop test on ");
  SERIAL_PORT_MONITOR.println(XSTR(SERIAL_PORT_TESTED));
  SERIAL_PORT_MONITOR.print(configNb);
  SERIAL_PORT_MONITOR.println(" configs to test.");
  SERIAL_PORT_MONITOR.print(speedNb);
  SERIAL_PORT_MONITOR.println(" speed to test.");
  start_time=millis();
}

void loop() {
  uint32_t mask = 0;
  if (configCur == configNb) {
    SERIAL_PORT_MONITOR.println("SerialLoop test done.\nResults:");
    SERIAL_PORT_MONITOR.print("OK: ");
    SERIAL_PORT_MONITOR.println(nbTestOK);
    SERIAL_PORT_MONITOR.print("KO: ");
    SERIAL_PORT_MONITOR.println(nbTestKO);
    SERIAL_PORT_MONITOR.print("Test duration (ms): ");
    SERIAL_PORT_MONITOR.print(millis() - start_time);

    while(1); // End test
  }

  SERIAL_PORT_MONITOR.println("########################");
  SERIAL_PORT_MONITOR.print("Config: ");
  SERIAL_PORT_MONITOR.print(serialConfig[configCur].name);
  SERIAL_PORT_MONITOR.print(" (0x");
  SERIAL_PORT_MONITOR.print(serialConfig[configCur].config, HEX);
  SERIAL_PORT_MONITOR.println(")");
  for (uint32_t s=0; s<speedNb; s++) {
    SERIAL_PORT_MONITOR.print("Test at ");
    SERIAL_PORT_MONITOR.print(serialSpeed[s]);
    SERIAL_PORT_MONITOR.println(" baud");
    SERIAL_PORT_TESTED.begin(serialSpeed[s],serialConfig[configCur].config);
    mask = dataMask(serialConfig[configCur].config);
    for (uint32_t i=0; i<=(0xFF&mask); i++)
    {
      test_uart(i&mask);
    }
    SERIAL_PORT_TESTED.end();
  }
  SERIAL_PORT_MONITOR.println("End.");
  configCur++;
}

