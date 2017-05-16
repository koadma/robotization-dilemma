#pragma once

#include "../Core/Polynomial.h"

#ifdef WIN32
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

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

class NetworkC;
class NetworkS;
class Ship;

typedef void(*RecivePacketFuncS)(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr, Ship* ship);
typedef void(*RecivePacketFuncC)(unsigned char *Data, int Id, int DataLen, NetworkC* thisptr, Ship* ship);


//void defaultRecivePacketFunc(unsigned char *Data, int Id, int DataLen);

class NetworkS {
public:
  NetworkS();
  NetworkS(string port, RecivePacketFuncS recivePacketFunc);
  ~NetworkS();
  void Loop();
  int SendData(char *Data, int Id, int DataLen);
  int SendData(unsigned char *Data, int Id, int DataLen);
  template<typename T> int SendData(T& data, int Id) {
    stringstream ss;
    ss << data;
    std::string datas = ss.str();

    char* id_c = new char[datas.size() + 1];
    strcpy_s(id_c, datas.size(), datas.c_str());
    int val = SendData(id_c, Id, datas.length());

    delete[] id_c;
    return val;
  }
  int ReciveData();
  thread ReciveLoopThread;
  bool Running = true;
  RecivePacketFuncS RecivePacket;
  Ship* ConnectedShip;
private:
  WSADATA wsaData;
  SOCKET ListenSocket = INVALID_SOCKET;
  SOCKET ClientSocket = INVALID_SOCKET;
  struct addrinfo hints;
};

class NetworkC {
public:
  NetworkC();
  NetworkC(string IP, string port, RecivePacketFuncC recivePacketFunc);
  ~NetworkC();
  void Loop();
  int SendData(char *Data, int Id, int DataLen);
  int SendData(unsigned char *Data, int Id, int DataLen);
  template<typename T> int SendData(T& data, int Id) {
    stringstream ss;
    ss << data;
    std::string datas = ss.str();

    char* id_c = new char[datas.size() + 1];
    strcpy_s(id_c, datas.size(), datas.c_str());

    int val = SendData(id_c, Id, datas.length());

    delete[] id_c;
    return val;
  }
  int ReciveData();
  bool Running = true;
  thread ReciveLoopThread;
  Ship* ConnectedShip;
private:
  WSADATA wsaData;
  SOCKET ConnectSocket = INVALID_SOCKET;
  struct addrinfo hints;
  RecivePacketFuncC RecivePacket;
};


void concat(vector<pair<unsigned char*, int> > in, unsigned char** C, int &lenC);

void split(unsigned char* data, int dataLen, vector<pair<unsigned char**, int> > &split);