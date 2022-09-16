#include "zmod4410.h"

zmod44xx_dev_t dev;
int8_t ret;
uint8_t stabilization_samples = 0;
uint8_t zmod44xx_status = 0;
uint8_t adc_result[30] = { 0 };
float rmox[1] = { 0 };

void setup()
  {
  SerialUSB.begin(115200);

  Wire.begin();

  Serial.println("Wire begin successfully");

  uint8_t buffer[20];
  uint8_t data_reg = 0x88;
  uint8_t i2c_address = 0x32;
  uint8_t size = 20;

  dev.read = arduino_i2c_read(ZMOD4410_I2C_ADDRESS, data_reg, buffer, size);
  dev.write = arduino_i2c_write(ZMOD4410_I2C_ADDRESS, data_reg, buffer, size);
  dev.i2c_addr = ZMOD4410_I2C_ADDRESS;

  delay(10);

  Serial.println("This is ZMOD4410 test!");

  ret = zmod44xx_read_sensor_info(&dev);
  //Serial.print("Ret for Sensor Info       : ");
  //Serial.println (ret);
  if(ret)
        {
        Serial.print("Error ");
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD read sensor info is success!");
        }

  ret = zmod44xx_init_sensor(&dev);
  //Serial.print("Ret for init sensor       : ");
  //Serial.println (ret);
  if(ret)
        {
        Serial.print("Error ");
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD sensor init is success!");
        }

  ret = zmod44xx_init_measurement(&dev);
  //Serial.print("Ret for init measurement  : ");
  //Serial.println (ret);
  if(ret)
        {
        Serial.print("Error ");
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD init measurement is success!");
        }

  /*ret = zmod44xx_start_measurement(&dev);
  //Serial.print("Ret for Start Measurement : ");
  //Serial.println (ret);
  if(ret)
        {
        Serial.print("Error ");
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD start measurement is success!");
        }*/
  }

void loop()
  {
  ret = zmod44xx_read_status(&dev, &zmod44xx_status);
  //Serial.print("Ret for read status       : ");
  //Serial.println (ret);

  if(ret)
        {
        Serial.print("Error ");
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD read status is success!");
        }

  if (STATUS_SEQUENCER_RUNNING_MASK & zmod44xx_status)
  {
    delay(5);
  }

  ret = zmod44xx_read_adc_results(&dev, adc_result);
  //Serial.print("Ret for ADC result        : ");
  //Serial.println (ret);
  if(ret)
        {
        Serial.print("Error " );
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD init is success!");
        }

  /* start a new measurement before result calculation */
  ret = zmod44xx_start_measurement(&dev);
  //Serial.print("Ret for Start Meas 2      : ");
  //Serial.println (ret);
  if(ret)
        {
        Serial.print("Error ");
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD start measurement inside loop is success!");
        }

  ret = zmod44xx_calc_rmox(&dev, adc_result, rmox);
  //Serial.print("Ret for calc rmox         : ");
  //Serial.println (ret);
  if(ret)
        {
        Serial.print("Error ");
        Serial.print(ret);
        Serial.println(" exiting program!");
        }

    if(ret == 0)
        {
        Serial.println("ZMOD Calculate Rmox is success!");
        }

  SerialUSB.print("Rmox : ");
  SerialUSB.println(rmox[0]);

  Serial.println("");
  Serial.println("#######################################################");
  Serial.println("");

  delay(1000);
  }