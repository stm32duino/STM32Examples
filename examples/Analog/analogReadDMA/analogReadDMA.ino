/*
  analogReadDMA

  This sketch shows how to use analogReadDMA() method.

  analogReadDMA uses ADC with DMA to read samples and increases sampling rate.
*/

// Define the number of sample to read
#define NB_SAMPLE 16

// Array where save the samples
uint32_t samples[NB_SAMPLE] = {0};

// callback parameter
bool readingOver = false;

// callback function
void callback(void *cbParam)
{
  *((bool *)cbParam) = true;
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("-- analogReadDMA sketch --");
  delay(500);

  // Start DMA acquisiton
  analogReadDMA(A0, samples, NB_SAMPLE, callback, &readingOver);
}


void loop() {
  if (readingOver == true)
  {
    for (uint32_t idx=0; idx<NB_SAMPLE; idx++)
    {
      Serial.print("raw: ");
      Serial.println(samples[idx], DEC);
    }
    readingOver = false;
    analogReadDMA(A0, samples, NB_SAMPLE, callback, &readingOver);
  }
}
