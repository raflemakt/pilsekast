#include "icm_sensors.h"
#define WIRE_PORT Wire
#define AD0_VAL 1
unsigned long lastTime = 0;

ICM_20948_I2C myICM;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
  };
  WIRE_PORT.begin(21, 22);
  WIRE_PORT.setClock(400000);
  bool initialized = false;
  while (!initialized)
  {
    myICM.begin(WIRE_PORT, AD0_VAL);
    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      Serial.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }
}

float absValuePrint(float x, float y, float z)
{
  return sqrt(sq(x) + sq(y) + sq(z));
}

void printAccVerdi(ICM_20948_I2C *sensor)
{
  float accVerdi = absValuePrint(9.81 / 1000 * (sensor->gyrX()), 9.81 / 1000 * (sensor->gyrY()), 9.81 / 1000 * (sensor->gyrZ()));
  Serial.println(accVerdi);
}

void loop(){
  if (myICM.dataReady())
  {
    myICM.getAGMT();
    printAccVerdi(&myICM);
  }
  else
  {
    Serial.println("Waiting for data");
  }
}
