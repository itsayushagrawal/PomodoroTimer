#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  mpu.initialize();

  Serial.println(mpu.testConnection() ? "MPU Connected" : "MPU Not Connected");
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float ax_g = ax / 16384.0;      //Convert raw data into g values
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  // Serial.print(ax_g); Serial.print(" ");
  // Serial.print(ay_g); Serial.print(" ");
  // Serial.println(az_g);

  if(ax_g < 0.2 && ax_g > -0.2)    // Screen not facing up or down
  {
    if(ay_g < 0.2 && ay_g > -0.2)
    {
      if(az_g<-0.8)
        Serial.println("Side C");

      if(az_g>0.8)
        Serial.println("Side A");
    }

    else if(az_g < 0.2 && az_g > -0.2)
    {
      if(ay_g<-0.8)
        Serial.println("Side D");

      if(ay_g>0.8)
        Serial.println("Side B");
    }
  }

  delay(500);
}
