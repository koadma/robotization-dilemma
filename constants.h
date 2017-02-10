#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

const int INITIAL_VISIBILITY = 5;
const float SOL = 3e8; // m/s
const int INITIAL_MAX_VELOCITY = 0.1*SOL;
const int G = 10; // m/s^2
const int INITIAL_MAX_ACC = 25*G;
const int INITIAL_HULL_RADIUS = 1000; // m
const int INITIAL_MAX_SENSORENERGY = 10;
const float MAP_SIZE = 1e5; //m //Solar System has a diameter of 9.09 billion km (if it ends at Neptune), might be too big
const float INITIAL_VELOCITY = 1e-6*SOL; //1e-2*SOL;
const int ROUND_TIME = 20; // s
//const int MAP_SCALE = 10; // 1 coord = MAP_SCALE real meters 
const float PI = 3.1415926535898;
const int ACTIVE_SENSOR_MINVIS = 0;
const int PASSIVE_SENSOR_MINVIS = 10;

#endif
