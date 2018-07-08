#pragma once

#ifdef M_CLIENT
#define SCRIPT_GUI
#endif
#ifdef M_SERVER
#undef SCRIPT_GUI
#endif

#include "../../../NGin/NGin.h"

extern int VersionA;
extern int VersionB;
extern int VersionC;

enum PacketIds2 {
  PacketGameOver = 2,
  PacketNewRound = 3,
  PacketCommand = 4,
  PacketCommit = 5,
  PacketSensor = 6,
  PacketCommandHistory = 7,
  PacketShipData = 8,
};