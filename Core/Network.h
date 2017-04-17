#pragma once

#include "..\Core\Point.h"

#ifdef WIN32
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include <iostream>
#include <thread>
#include <string>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif


using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define PACKET_HEADER_LEN 4 //Used to store packet ID and data length;
#define PACKET_HEADER_TYPE int

typedef union {
  struct {
    char chars[PACKET_HEADER_LEN];
  } chararr;
  PACKET_HEADER_TYPE i;
} Packet_Header_Convertor;
//Breaks PACKET_HEADER_TYPE into PACKET_HEADER_LEN chars

typedef void(*RecivePacketFunc)(unsigned char *Data, int Id, int DataLen);

void defaultRecivePacketFunc(unsigned char *Data, int Id, int DataLen);

class NetworkS {
public:
  NetworkS();
  NetworkS(string port, RecivePacketFunc recivePacketFunc);
  ~NetworkS();
  void Loop();
  int SendData(char *Data, int Id, int DataLen);
  int ReciveData();
  thread ReciveLoopThread;
  bool Running = true;
private:
  WSADATA wsaData;
  SOCKET ListenSocket = INVALID_SOCKET;
  SOCKET ClientSocket = INVALID_SOCKET;
  struct addrinfo hints;
  RecivePacketFunc RecivePacket;
};

class NetworkC {
public:
  NetworkC();
  NetworkC(string IP, string port, RecivePacketFunc recivePacketFunc);
  ~NetworkC();
  void Loop();
  int SendData(char *Data, int Id, int DataLen);
  int ReciveData();
  thread ReciveLoopThread;
  bool Running = true;
private:
  WSADATA wsaData;
  SOCKET ConnectSocket = INVALID_SOCKET;
  struct addrinfo hints;
  RecivePacketFunc RecivePacket;
};