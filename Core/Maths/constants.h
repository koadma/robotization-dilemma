#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

//1 Basic info
//2 Round info
//4 Round simulation details
//8 All events
#define LOG_LVL 31

typedef double time_type_s;
typedef double energy_type_J;
typedef double power_type_W;
typedef double distance_type_m;
typedef double mass_type_kg;
typedef double vel_type_mpers;
typedef double acc_type_mperss;
typedef double scalar_type;
typedef double en_flux_type_Jpermm;
//typedef double en_flux_type_Jy; //1Jy = 10e-26 J/m m
typedef double mass_density_type_kgpermmm;

const energy_type_J BUBBLE_REMOVE = 1e-7; //Dont emit

///!!!!!!!! ######## &&&&&&&&&
const vel_type_mpers SOL = 3e5; ///TODO FIX 3e8 ######### ######### !!!!!!!!! ERROR &&&&&&&& WARNING !!!!!!!!!
///(Changed for better testing, to reduce floating point errors)


const time_type_s ROUND_TIME = 2;

#define THREE_PI    9.4247779607694
#define TWO_PI      6.2831853071796
#define PI          3.1415926535898
#define HALF_PI     1.5707963267949
#define THIRD_PI    1.0471975511966
#define QUARTER_PI  0.7853981633974

#define E           2.71828182845904523536028747135266249775724709369995

constexpr long double operator"" _deg(long double deg)
{
  return deg*3.141592 / 180;
}

const int VersionA = 0;
const int VersionB = 5;
const int VersionC = 1;

const acc_type_mperss MAX_ACCEL = 100;


enum LoginError {
  LoginErrorOk = 0,
  LoginErrorInvalidCredentials = 1,
  LoginErrorGameFull = 2,
  LoginErrorVersionError = 3,
  LoginErrorProtocolError = 4,
  LoginErrorInvalidAuth = 5,
  LoginErrorTryRejoin = 6
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

enum KeyConfigID {
  KeyCameraUp = 0,
  KeyCameraLeft = 1,
  KeyCameraDown = 2,
  KeyCameraRight = 3,
  KeyCenterWorld = 4,
  KeyCenterShip = 5,
  KeyCenterSighting = 6,
  KeyPlotZoomX = 7,
  KeyPlotZoomY = 8,
  KeyPlotReset = 9
};

#define SightingSize 0.02
#define ShipSize 0.03

#endif
