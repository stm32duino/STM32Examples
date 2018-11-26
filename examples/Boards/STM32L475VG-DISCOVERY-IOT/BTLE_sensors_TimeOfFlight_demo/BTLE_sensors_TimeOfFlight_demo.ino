/*

 BTLE_sensors_tof_demo

 This sketch provides a default demo to be used on STMicroelectronics Discovery L475VG IoT board.
 It will use several components of the board and you need to install corresponding libraries :
   * Low energy Bluetooth (SPBTLE_RF) : https://github.com/stm32duino/SPBTLE-RF
   * Temperature and pressure sensor (LPS22HB) : https://github.com/stm32duino/LPS22HB
   * Temperature and humidity sensor (HTS221) : https://github.com/stm32duino/HTS221
   * Time of flight sensor (VL53L0X) : https://github.com/stm32duino/VL53L0X

 You can find more information on this board here :
   http://www.st.com/en/evaluation-tools/b-l475e-iot01a.html

 This sketch will launch 3 services on BLE : Acc, Environnemental and Time.
 For testing the sketch, you need to download BlueNRG application from playstore provided by
 STMicroelectronics. (https://play.google.com/store/apps/details?id=com.st.blunrg for Android or
 https://itunes.apple.com/fr/app/bluenrg/id705873549?mt=8 for Apple)
 Compile and download the demo on your board, then, on your smartphone, enable Bluetooth, launch
 the application and connect it to the BLueNRG device. You will see all the services, you can
 click on each one.

 Temperature, humidity and pressure are sent to the environment service and updated on regular
 basis.
 To move the cube on motion tab, you have to swipe on top of the vl53l0x sensor. To change
 direction of the motion, use a tap. The gesture has to be done at less than 1 meter from the
 sensor.

 Do not forget to check information on the console.

 */

#include <SPI.h>
#include <SPBTLE_RF.h>
#include <sensor_service.h>

#include <HTS221Sensor.h>
#include <LPS22HBSensor.h>

#include <vl53l0x_class.h>
#include <tof_gestures.h>
#include <tof_gestures_SWIPE_1.h>
#include <tof_gestures_TAP_1.h>

/* Configure SPI3 for BLE
  MOSI: PC12, MISO: PC11, SCLK: PC10
  */
SPIClass SPI_3(PC12, PC11, PC10);

/* Configure BTLE pins
      csPin = PD13, spiIRQ = PE6, reset = PA8, led = LED4
*/
SPBTLERFClass BTLE(&SPI_3, PD13, PE6, PA8, LED4);

/* BLE device name and address */
const char *name = "BlueNRG";
uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03}; // update this value in case you have several demo in the same place...

/* i2c sensors */
HTS221Sensor  *HumTemp;
LPS22HBSensor *PressTemp;
VL53L0X       *sensor_vl53l0x;

TwoWire *dev_i2c;
#define I2C2_SCL    PB10
#define I2C2_SDA    PB11

/* global variables */
AxesRaw_t axes_data;                     // structure to update position on BLE motion part
uint32_t previousSecond = 0;             // used to trigger environmental data update
Gesture_SWIPE_1_Data_t gestureSwipeData; // vl53l0x swipe detection
Gesture_TAP_1_Data_t gestureTapData;     // vl53l0x tap detection
uint32_t distance_top;                   // used for ToF gesture recognition
uint32_t count_swipe;                    // count for swipe animation
int axis_to_update;                      // axis to update on a swipe detection

#define UPDATE_AXIS_X 0
#define UPDATE_AXIS_Y 1
#define UPDATE_AXIS_Z 2


/***************************************************************************************
 * Private functions
 ***************************************************************************************/

/* Setup vl53l0x for single shot mode */
void SetupSingleShot(void){
  int status;
  uint8_t VhvSettings;
  uint8_t PhaseCal;
  uint32_t refSpadCount;
  uint8_t isApertureSpads;

  status = sensor_vl53l0x->StaticInit();
  if( status ){
    Serial.println("StaticInit sensor failed");
  }

  status = sensor_vl53l0x->PerformRefCalibration(&VhvSettings, &PhaseCal);
  if( status ){
    Serial.println("PerformRefCalibration sensor failed");
  }

  status = sensor_vl53l0x->PerformRefSpadManagement(&refSpadCount, &isApertureSpads);
  if( status ){
    Serial.println("PerformRefSpadManagement sensor failed");
  }

  status = sensor_vl53l0x->SetDeviceMode(VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
  if( status ){
    Serial.println("SetDeviceMode sensor failed");
  }

  status = sensor_vl53l0x->SetMeasurementTimingBudgetMicroSeconds(20*1000);
  if( status ){
    Serial.println("SetMeasurementTimingBudgetMicroSeconds sensor failed");
  }
}

/* function to detect swipe on vl53l0x */
bool swipe_detected() {
  int gesture_code;
  int status;
  bool ret = false;

  sensor_vl53l0x->StartMeasurement();

  int top_done = 0;
  uint8_t NewDataReady=0;
  VL53L0X_RangingMeasurementData_t pRangingMeasurementData;

  do
  {
    if(top_done == 0)
    {
      NewDataReady = 0;
      status = sensor_vl53l0x->GetMeasurementDataReady(&NewDataReady);

      if( status ){
        Serial.println("GetMeasurementDataReady sensor failed");
      }

      if(NewDataReady)
      {
        status = sensor_vl53l0x->ClearInterruptMask(0);
        if( status ){
          Serial.println("ClearInterruptMask sensor failed");
        }

        status = sensor_vl53l0x->GetRangingMeasurementData(&pRangingMeasurementData);
        if( status ){
          Serial.println("GetRangingMeasurementData sensor failed");
        }

        if (pRangingMeasurementData.RangeStatus == 0) {
          // we have a valid range.
          distance_top = pRangingMeasurementData.RangeMilliMeter;
        }else {
          distance_top = 1200;
        }

        top_done = 1;
      }
    }
  }while(top_done == 0);

  // Launch gesture detection algorithm.
  gesture_code = tof_gestures_detectSWIPE_1(distance_top, &gestureSwipeData);

  // Check the result of the gesture detection algorithm.
  switch(gesture_code)
  {
    case GESTURES_SINGLE_SWIPE:
      Serial.println(" => Swipe detected");
      ret = true;
      break;
    default:
      // Do nothing
      break;
  }
  return ret;
}

/* function to detect tap on vl53l0x */
bool tap_detected() {
  int gesture_code;
  int status;
  bool ret = false;

  sensor_vl53l0x->StartMeasurement();

  int top_done = 0;
  uint8_t NewDataReady=0;
  VL53L0X_RangingMeasurementData_t pRangingMeasurementData;

  do
  {
    if(top_done == 0)
    {
      NewDataReady = 0;
      status = sensor_vl53l0x->GetMeasurementDataReady(&NewDataReady);

      if( status ){
        Serial.println("GetMeasurementDataReady sensor failed");
      }

      if(NewDataReady)
      {
        status = sensor_vl53l0x->ClearInterruptMask(0);
        if( status ){
          Serial.println("ClearInterruptMask sensor failed");
        }

        status = sensor_vl53l0x->GetRangingMeasurementData(&pRangingMeasurementData);
        if( status ){
          Serial.println("GetRangingMeasurementData sensor failed");
        }

        if (pRangingMeasurementData.RangeStatus == 0) {
          // we have a valid range.
          distance_top = pRangingMeasurementData.RangeMilliMeter;
        }else {
          distance_top = 1200;
        }

        top_done = 1;
      }
    }
  }while(top_done == 0);

  // Launch gesture detection algorithm.
  gesture_code = tof_gestures_detectTAP_1(distance_top, &gestureTapData);

  // Check the result of the gesture detection algorithm.
  switch(gesture_code)
  {
    case GESTURES_SINGLE_TAP:
      Serial.println(" => Tap detected");
      ret = true;
      break;
    default:
      // Do nothing
      break;
  }
  return ret;
}

/* get data from environmental sensor and send update on BTLE */
void update_environment_data(){
  float humidity, temperature;
  float pressure, temperature_lps22hb;

  HumTemp->GetHumidity(&humidity);
  HumTemp->GetTemperature(&temperature);

  PressTemp->GetPressure(&pressure);
  PressTemp->GetTemperature(&temperature_lps22hb);

  //Update environnemental data
  SensorService.Temp_Update(temperature*10);
  SensorService.Press_Update(pressure*100);
  SensorService.Humidity_Update(humidity*10);
}

/**
 * Update the motion data on BLE. This depends of user action in front of vl53l0x
 * sensor.
 * A tap will switch the axis to update and a swipe will rotate the cube
 *
 * @param  AxesRaw_t* p_axes
 * @retval None
 */
void update_motion_data(AxesRaw_t* p_axes)
{
  // change axis to update on each tap detection
  if(tap_detected()){
    axis_to_update++;
    if( axis_to_update > UPDATE_AXIS_Y ) axis_to_update = UPDATE_AXIS_X;
    // test should be on Z axis. But for now, the application on smartphone
    // doesn't do anything with Z axis...
    switch(axis_to_update){
      case UPDATE_AXIS_X:
        Serial.println("A swipe will now move the cube on X axis");
        break;
      case UPDATE_AXIS_Y:
        Serial.println("A swipe will now move the cube on Y axis");
        break;
      case UPDATE_AXIS_Z:
        Serial.println("A swipe will now move the cube on Z axis");
        break;
    }
  }

  if(swipe_detected()){
    count_swipe = 20; // arbitrary value in order to have a cube moving for a certain time.
  }

  if(count_swipe > 0){
    switch(axis_to_update){
      case UPDATE_AXIS_X:
        p_axes->AXIS_X += 100; // arbitrary value to move the cube
        break;
      case UPDATE_AXIS_Y:
        p_axes->AXIS_Y += 100; // arbitrary value to move the cube
        break;
      case UPDATE_AXIS_Z:
        p_axes->AXIS_Z += 100; // arbitrary value to move the cube
        break;
    }
    count_swipe--;
    SensorService.Acc_Update(p_axes);
  }
}

/***************************************************************************************
 * Arduino standard functions
 ***************************************************************************************/
void setup() {
  int ret;

  Serial.begin(9600);

  Serial.println("");
  Serial.println("*****************************************************************************************");
  Serial.println("Demo for STMicroelectronics Disco IoT L475 board");
  Serial.println("");
  Serial.println("Download and install the BlueNRG application from STMicroelectronics on your smartphone, ");
  Serial.println("activate BLE and check the different tabs of the app.");
  Serial.println("");
  Serial.println("Swipe and tap in front of vl53l0x sensor to move the cube.");
  Serial.println("*****************************************************************************************");
  Serial.println("");

  count_swipe = 0;
  axis_to_update = UPDATE_AXIS_X;
  axes_data.AXIS_X = 0;
  axes_data.AXIS_Y = 0;
  axes_data.AXIS_Z = 0;

  /* Configure the User Button in GPIO Mode */
  pinMode(USER_BTN, INPUT);

  // Initialize I2C bus.
  dev_i2c = new TwoWire(I2C2_SDA, I2C2_SCL);
  dev_i2c->begin();

  // Initlialize components.
  HumTemp = new HTS221Sensor (dev_i2c);
  HumTemp->Enable();

  PressTemp = new LPS22HBSensor(dev_i2c);
  PressTemp->Enable();

  sensor_vl53l0x = new VL53L0X(dev_i2c, PC6, PC7);
  sensor_vl53l0x->VL53L0X_Off();
  ret = sensor_vl53l0x->InitSensor(0x10);
  if(ret) {
    Serial.println("Init sensor_vl53l0x failed...");
    while(1);
  }

  // Initialize gesture libraries.
  tof_gestures_initSWIPE_1(&gestureSwipeData);
  tof_gestures_initTAP_1(&gestureTapData);
  SetupSingleShot();

  // Initialize BTLE part
  if(BTLE.begin() == SPBTLERF_ERROR){
    Serial.println("Bluetooth module configuration error!");
    while(1);
  }

  if(SensorService.begin(name, SERVER_BDADDR)){
    Serial.println("Sensor service configuration error!");
    while(1);
  }

  ret = SensorService.Add_Acc_Service();
  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Acc service added successfully.");
  else
    Serial.println("Error while adding Acc service.");

  ret = SensorService.Add_Environmental_Sensor_Service();
  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Environmental Sensor service added successfully.");
  else
    Serial.println("Error while adding Environmental Sensor service.");

  /* Instantiate Timer Service with two characteristics:
   * - seconds characteristic (Readable only)
   * - minutes characteristics (Readable and Notifiable )
   */
  ret = SensorService.Add_Time_Service();
  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Time service added successfully.");
  else
    Serial.println("Error while adding Time service.");
}


void loop() {

  BTLE.update();

  if(SensorService.isConnected() == TRUE)
  {
    //Update accelerometer values
    update_motion_data(&axes_data);

    //Update time
    SensorService.Update_Time_Characteristics();

    if((millis() - previousSecond) >= 1000)
    {
      update_environment_data();
      previousSecond = millis();
    }
  }
  else
  {
    //Keep the Bluetooth module in discoverable mode
    SensorService.setConnectable();

    if(swipe_detected() || tap_detected()){
      Serial.println("Enable the BLE and launch BlueNRG application on your smartphone...");
    }
  }
}
