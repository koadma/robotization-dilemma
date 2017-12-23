#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__


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

const en_flux_type_Jpermm BUBBLE_REMOVE = 10e-7;

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

#define ElementBaseColor 0xff070918
#define ElementActiveColor 0xff190077
#define ElementTextColor 0xff838FA1
#define ElementBackColor 0xff030014

#define VersionA 0
#define VersionB 4
#define VersionC 0
//0.40
//Converge to 137.035999139

const acc_type_mperss MAX_ACCEL = 250;


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

#endif
