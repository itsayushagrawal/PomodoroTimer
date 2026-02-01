#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;
int last_side=-1;
int current_side=-1;

int min =0;
int sec =0;

unsigned long lastMillis = 0;
const unsigned long interval = 1000;

void setup() 
{
  Wire.begin();
  Serial.begin(9600);

  mpu.initialize();

  Serial.println(mpu.testConnection() ? "MPU Not Connected" : "MPU Connected");
}

void loop() 
{
  current_side= getSide();

  if (current_side != last_side) 
  {
    set_new_timer(current_side);
    printTime();
    last_side = current_side;
    delay(2000);
    // lastMillis = millis();
  }
  runTimer();
  delay(500);
}

int getSide()
{
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float ax_g = ax / 16384.0;      //Convert raw data into g values
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  if (az_g > 0.8) 
  {
    return 0;
  }
  else if (az_g < -0.8) 
  {
    return 2;
  }
  else if (ay_g > 0.8) 
  {
    return 1;
  }
  else if (ay_g < -0.8) 
  {
    return 3;
  }
  else
  {
    Serial.println("Detection Failed");
  }
  return -1;
}

void set_new_timer(int current_side)
{
  sec = 0;
  switch (current_side)
  {
    case 0: min = 5;
            break;
    case 1: min = 15;
            break;
    case 2: min = 30;
            break;
    case 3: min = 60;
            break;
  }

}

void runTimer() 
{
  if (min == 0 && sec == 0) return; // timer finished

  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= interval) 
  {
    lastMillis = currentMillis;

    if (sec == 0) 
    {
      if (min > 0) 
      {
        min--;
        sec = 59;
      }
    } 
    else 
    {
      sec--;
    }

    printTime();
  }
}

void printTime() 
{
  if (min < 10)           //minute formatting
    Serial.print("0");
  Serial.print(min);

  Serial.print(":");

  if (sec < 10)         // seconds formatting
    Serial.print("0");
  Serial.println(sec);
}
