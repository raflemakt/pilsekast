#pragma once

#include "ICM_20948.h"

extern ICM_20948_I2C myICM;
extern float accVerdi;

void ICMsetup();
float absValuePrint(float x, float y, float z);

void printAccVerdi(ICM_20948_I2C *sensor);
void ICMloop();