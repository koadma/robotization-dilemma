#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

const int INITIAL_VISIBILITY = 5;
const double SOL = 3e8; // m/s
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
const double THREE_PI    = 9.4247779607694;
const double TWO_PI      = 6.2831853071796;
const double PI          = 3.1415926535898;
const double HALF_PI     = 1.5707963267949;
const double THIRD_PI    = 1.0471975511966;
const double QUARTER_PI  = 0.7853981633974;
const int ACTIVE_SENSOR_MINVIS = 0;
const int PASSIVE_SENSOR_MINVIS = 10;

#define ElementBaseColor 0xff070918
#define ElementActiveColor 0xff190077
#define ElementTextColor 0xff838FA1
#define ElementBackColor 0xff030014

#define VersionA 0
#define VersionB 1
#define VersionC 0
//0.1.0

enum LoginError {
  LoginErrorOk = 0,
  LoginErrorInvalidCredentials = 1,
  LoginErrorGameFull = 2,
  LoginErrorVersionError = 3,
  LoginErrorProtocolError = 4
};

enum CommandIds {
  CommandAccel = 1,
  CommandShoot = 2,
};

enum PacketIds {
  PacketDisconnect = 0,
  PacketLogin = 1,
  PacketGameOver = 2,
  PacketNewRound = 3,
  PacketCommand = 4,
  PacketCommit = 5,
  PacketSensor = 6,
  PacketCommandHistory = 7,
  PacketShipData = 8,
};

#endif
