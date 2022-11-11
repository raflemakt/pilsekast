#include "icm_sensors.h"
#define WIRE_PORT Wire
#define AD0_VAL 1
unsigned long lastTime = 0;

ICM_20948_I2C myICM;

void ICMsetup()
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

float printAccVerdi(ICM_20948_I2C *sensor)
{
  return absValuePrint(9.81 / 1000 * (sensor->gyrX()), 9.81 / 1000 * (sensor->gyrY()), 9.81 / 1000 * (sensor->gyrZ()));
}

float ICMupdate(){
  if (myICM.dataReady())
  {
    myICM.getAGMT();
    float accValue = printAccVerdi(&myICM);
    return accValue;
  }
  return 0;
}

void ICMloop()
{

}
