#include "zmod4410.h"

zmod44xx_dev_t dev;
int8_t ret;
uint8_t stabilization_samples = 0;
uint8_t zmod44xx_status = 0;
uint8_t adc_result[30] = { 0 };
float rmox[1] = { 0 };

  
void setup() {

  SerialUSB.begin(115200);
  Wire.begin();

  dev.read = arduino_i2c_read;
  dev.write = arduino_i2c_write;
  dev.i2c_addr = ZMOD4410_I2C_ADDRESS;
  
  delay(10);
  
  ret = zmod44xx_read_sensor_info(&dev);
  if (ret) {
    SerialUSB.print(ret);
    SerialUSB.println("   error:1");
  }
   

  ret = zmod44xx_init_sensor(&dev);
  if (ret) {
    SerialUSB.print(ret);
    SerialUSB.println("   error:2");
  }

  ret = zmod44xx_init_measurement(&dev);
  if (ret) {
    SerialUSB.print(ret);
    SerialUSB.println("   error:3");
  }

  ret = zmod44xx_start_measurement(&dev);
  if (ret) {
    SerialUSB.print(ret);
    SerialUSB.println("   error:4");
  }

}


void loop() {

  ret = zmod44xx_read_status(&dev, &zmod44xx_status);

  if (ret)
  { SerialUSB.print(ret);
    SerialUSB.println("   error:5");
  }

  if (STATUS_SEQUENCER_RUNNING_MASK & zmod44xx_status)
  {
    delay(5);
  }

  ret = zmod44xx_read_adc_results(&dev, adc_result);
  if (ret)
  { SerialUSB.print(ret);
    SerialUSB.println("   error:6");
  }

  /* start a new measurement before result calculation */
  ret = zmod44xx_start_measurement(&dev);
  if (ret)
  { SerialUSB.print(ret);
    SerialUSB.println("   error:7");
  }

  ret = zmod44xx_calc_rmox(&dev, adc_result, rmox);
  if (ret)
  { SerialUSB.print(ret);
    SerialUSB.println("   error:8");
  }

  SerialUSB.println(rmox[0]);

  delay(500);
}
