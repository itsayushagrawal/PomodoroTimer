#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;
int last_side=-1;
int current_side=-1;

int minutes =0;
int seconds =0;

unsigned long lastMillis = 0;
const unsigned long interval = 1000;

char timeString[6];

//function prototypes
// int getSide();
// void set_new_timer();
// void runTimer();
// void printTime();

void setup() 
{
  Wire.begin();
  Serial.begin(115200);
  delay(200);
  Serial.println("setup started");
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
  seconds = 0;
  switch (current_side)
  {
    case 0: minutes = 5;
            break;
    case 1: minutes = 15;
            break;
    case 2: minutes = 30;
            break;
    case 3: minutes = 60;
            break;
  }

}

void runTimer() 
{
  if (minutes == 0 && seconds == 0) return; // timer finished

  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= interval) 
  {
    lastMillis = currentMillis;

    if (seconds == 0) 
    {
      if (minutes > 0) 
      {
        minutes--;
        seconds = 59;
      }
    } 
    else 
    {
      seconds--;
    }

    printTime();
  }
}

void printTime() 
{
  sprintf(timeString,"%02d:%02d",minutes,seconds);
  Serial.println(timeString);


}