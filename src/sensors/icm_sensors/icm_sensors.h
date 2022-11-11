#pragma once

#include "ICM_20948.h"

extern ICM_20948_I2C myICM;

void ICMsetup();
float absValuePrint(float x, float y, float z);

float printAccVerdi(ICM_20948_I2C *sensor);
float ICMupdate();