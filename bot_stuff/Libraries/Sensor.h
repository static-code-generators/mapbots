#include "Constants.h"
#include <Arduino.h>

#ifndef READ_SENSOR_H
#define READ_SENSOR_H

void scanAll(float distCm[]);
float singleSensorReading(int sensor);

#endif
