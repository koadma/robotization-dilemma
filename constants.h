#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

const int INITIAL_VISIBILITY = 5;
const float SOL = 3e8; // m/s
const int INITIAL_MAX_VELOCITY = 0.1*SOL;
const int G = 10; // m/s^2
const int INITIAL_MAX_ACC = 25*G;
const int INITIAL_HULL_RADIUS = 1000; // m
const int INITIAL_MAX_SENSORENERGY = 10;
const int INITIAL_MAX_GENERATORENERGY = 300;
const float MAP_SIZE = 1e5; //m //Solar System has a diameter of 9.09 billion km (if it ends at Neptune), might be too big
const float INITIAL_VELOCITY = 1e-6*SOL; //1e-2*SOL;
const int ROUND_TIME = 20; // s
//const int MAP_SCALE = 10; // 1 coord = MAP_SCALE real meters 
const float THREE_PI    = 9.4247779607694;
const float TWO_PI      = 6.2831853071796;
const float PI          = 3.1415926535898;
const float HALF_PI     = 1.5707963267949;
const float THIRD_PI    = 1.0471975511966;
const float QUARTER_PI  = 0.7853981633974;
const int ACTIVE_SENSOR_MINVIS = 0;
const int PASSIVE_SENSOR_MINVIS = 10;

#define ElementBaseColor 0xff070918
#define ElementActiveColor 0xff190077
#define ElementTextColor 0xff838FA1
#define ElementBackColor 0xff030014

#endif
