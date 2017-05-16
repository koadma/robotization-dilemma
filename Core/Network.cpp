#include "Network.h"

enum NetworkErrorCodes {
  NetworkErrorCodeInitalize,
  NetworkErrorCodeResolveServerAddress,
  NetworkErrorCodeResolveClientAddress,
  NetworkErrorCodeCreateListenSocket,
  NetworkErrorCodeCreateServerSocket,
  NetworkErrorCodeCreateClientSocket,
  NetworkErrorCodeConnectServer,
  NetworkErrorCodeConnectClient,
  NetworkErrorCodeServerSendData,
  NetworkErrorCodeServerReciveData,
  NetworkErrorCodeClientSendData,
  NetworkErrorCodeClientReciveData,
  NetworkErrorCodeBindListenSocket,
  NetworkErrorCodeServerListen,
  NetworkErrorCodeServerAccept,
};

/*void defaultRecivePacketFunc(unsigned char *Data, int Id, int DataLen) {
  cout << Id << " " << Data << endl;
}
*/
void NetworkError(int ID, int WSAError = 0) {
  cout << "Error code: " << to_string(ID) << endl;
}

NetworkS::NetworkS() {
}
NetworkS::NetworkS(string port, RecivePacketFuncS recivePacketFunc) {
  RecivePacket = recivePacketFunc;
  struct addrinfo *result = NULL;
  int iResult;
  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    NetworkError(NetworkErrorCodeInitalize, iResult);
    return;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve the server address and port
  iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
  if (iResult != 0) {
    NetworkError(NetworkErrorCodeResolveServerAddress, iResult);
    WSACleanup();
    return;
  }

  // Create a SOCKET for connecting to server
  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (ListenSocket == INVALID_SOCKET) {
    NetworkError(NetworkErrorCodeCreateListenSocket);
    freeaddrinfo(result);
    WSACleanup();
    return;
  }

  // Setup the TCP listening socket
  iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    NetworkError(NetworkErrorCodeBindListenSocket);
    freeaddrinfo(result);
    closesocket(ListenSocket);
    WSACleanup();
    return;
  }

  freeaddrinfo(result);

  //LISTEN

  iResult = listen(ListenSocket, SOMAXCONN);
  if (iResult == SOCKET_ERROR) {
    NetworkError(NetworkErrorCodeServerListen);
    closesocket(ListenSocket);
    WSACleanup();
    return;
  }

  // Accept a client socket
  ClientSocket = accept(ListenSocket, NULL, NULL);
  if (ClientSocket == INVALID_SOCKET) {
    NetworkError(NetworkErrorCodeServerAccept);
    closesocket(ListenSocket);
    WSACleanup();
    return;
  }

  // No longer need server socket
  closesocket(ListenSocket);

  ReciveLoopThread = thread(&NetworkS::Loop, this);

}
NetworkS::~NetworkS() {
  Running = false;
  // shutdown the connection since we're done
  shutdown(ClientSocket, SD_SEND);
  // cleanup
  closesocket(ClientSocket);
  //ReciveLoopThread.join();
  WSACleanup();
}

void NetworkS::Loop()
{
  int iResult;
  // Receive until the peer shuts down the connection
  do {
    iResult = ReciveData();
    if (iResult <= 0) {
      if (iResult == 0) {
      }
      else {
        NetworkError(NetworkErrorCodeServerReciveData  );
        closesocket(ClientSocket);
        WSACleanup();
        return;
      }
    }
  } while (iResult > 0 && Running);
  Running = false;
}

int NetworkS::SendData(char *Data, int Id, int DataLen) {
  char* SendRaw;
  SendRaw = new char[DataLen + 2 * PACKET_HEADER_LEN];
  for (int i = 0; i < DataLen; i++) {
    SendRaw[i + 2 * PACKET_HEADER_LEN] = Data[i];
  }

  Packet_Header_Convertor conv;
  conv.i = DataLen;
  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    SendRaw[i] = conv.chararr.chars[i];
  }

  conv.i = Id;
  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    SendRaw[i + PACKET_HEADER_LEN] = conv.chararr.chars[i];
  }

  int iSendResult = send(ClientSocket, SendRaw, DataLen + 2 * PACKET_HEADER_LEN, 0);

  delete SendRaw;

  if (iSendResult != DataLen + 2 * PACKET_HEADER_LEN) {
    NetworkError(NetworkErrorCodeServerSendData  );
    closesocket(ClientSocket);
    WSACleanup();
    return -1;
  }
  //NetLog.LogString("Bytes sent: " + to_string(iSendResult));
  return iSendResult;
}

int NetworkS::SendData(unsigned char *Data, int Id, int DataLen) {
  SendData(reinterpret_cast<char*>(Data), Id, DataLen);
}

int NetworkS::ReciveData() {
  char len[PACKET_HEADER_LEN];
  Packet_Header_Convertor conv;
  PACKET_HEADER_TYPE dlen;
  PACKET_HEADER_TYPE pid;
  //DATA LENGTH
  int iRes = recv(ClientSocket, len, PACKET_HEADER_LEN, 0);
  if (iRes != PACKET_HEADER_LEN) {
    return 0;
  }


  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    conv.chararr.chars[i] = len[i];
  }

  dlen = conv.i;
  if (dlen == 0) {
    return 0;
  }

  //PACKET ID
  iRes = recv(ClientSocket, len, PACKET_HEADER_LEN, 0);
  if (iRes != PACKET_HEADER_LEN) {
    return 0;
  }

  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    conv.chararr.chars[i] = len[i];
  }

  pid = conv.i;

  //DATA

  char* data;
  data = new char[dlen];
  iRes = 0;
  while (iRes != dlen) {
    iRes += recv(ClientSocket, data + iRes, dlen - iRes, 0);
    //NetLog.LogString("RECiRes: " + to_string(iRes));
  }

  RecivePacket(reinterpret_cast<unsigned char*>(data), pid, dlen, this, ConnectedShip);
  delete data;
  return dlen;
}

NetworkC::NetworkC() {
}
NetworkC::NetworkC(string IP, string port, RecivePacketFuncC recivePacketFunc) {
  RecivePacket = recivePacketFunc;
  struct addrinfo *result, *ptr;
  result = NULL;
  ptr = NULL;
  int iResult;
  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    NetworkError(NetworkErrorCodeInitalize);
    return;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(IP.c_str(), port.c_str(), &hints, &result);
  if (iResult != 0) {
    NetworkError(NetworkErrorCodeResolveServerAddress);
    WSACleanup();
    return;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
      ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
      NetworkError(NetworkErrorCodeCreateClientSocket  );
      WSACleanup();
      return;
    }

    // Connect to server.
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      closesocket(ConnectSocket);
      ConnectSocket = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (ConnectSocket == INVALID_SOCKET) {
    NetworkError(NetworkErrorCodeConnectServer);
    WSACleanup();
    return;
  }

  ReciveLoopThread = thread(&NetworkC::Loop, this);

}
NetworkC::~NetworkC() {
  Running = false;
  if (ConnectSocket != SOCKET_ERROR) {
    // shutdown the connection since we're done
    shutdown(ConnectSocket, SD_SEND);
    // cleanup
    closesocket(ConnectSocket);
  }
  WSACleanup();
}

void NetworkC::Loop()
{
  int iResult;
  // Receive until the peer shuts down the connection
  do {
    iResult = ReciveData();
    //NetLog.LogString("iResult: " + to_string(iResult));
    if (iResult <= 0) {
      if (iResult == 0) {

      }
      else {
        NetworkError(NetworkErrorCodeClientReciveData, WSAGetLastError());
        closesocket(ConnectSocket);
        //WSACleanup();
        return;
      }
    }
  } while (iResult > 0 && Running);
  Running = false;
}

int NetworkC::SendData(char *Data, int Id, int DataLen) {
  char* SendRaw;
  SendRaw = new char[DataLen + 2 * PACKET_HEADER_LEN];
  for (int i = 0; i < DataLen; i++) {
    SendRaw[i + 2 * PACKET_HEADER_LEN] = Data[i];
  }

  Packet_Header_Convertor conv;
  conv.i = DataLen;
  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    SendRaw[i] = conv.chararr.chars[i];
  }

  conv.i = Id;
  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    SendRaw[i + PACKET_HEADER_LEN] = conv.chararr.chars[i];
  }

  int iSendResult = send(ConnectSocket, SendRaw, DataLen + 2 * PACKET_HEADER_LEN, 0);

  delete SendRaw;

  if (iSendResult != DataLen + 2 * PACKET_HEADER_LEN) {
    NetworkError(NetworkErrorCodeClientSendData  );
    closesocket(ConnectSocket);
    WSACleanup();
    return -1;
  }
  return iSendResult;
}

int NetworkC::SendData(unsigned char *Data, int Id, int DataLen) {
  SendData(reinterpret_cast<char*>(Data), Id, DataLen);
}


int NetworkC::ReciveData() {
  char len[PACKET_HEADER_LEN];
  Packet_Header_Convertor conv;
  PACKET_HEADER_TYPE dlen;
  PACKET_HEADER_TYPE pid;
  //DATA LENGTH
  int iRes = recv(ConnectSocket, len, PACKET_HEADER_LEN, 0);
  //NetLog.LogString("iRes: " + to_string(iRes));
  if (iRes != PACKET_HEADER_LEN) {
    return 0;
  }


  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    conv.chararr.chars[i] = len[i];
  }

  dlen = conv.i;
  //NetLog.LogString("dlen: " + to_string(dlen));
  if (dlen == 0) {
    return 0;
  }

  //PACKET ID
  iRes = recv(ConnectSocket, len, PACKET_HEADER_LEN, 0);
  //NetLog.LogString("iRes: " + to_string(iRes));
  if (iRes != PACKET_HEADER_LEN) {
    return 0;
  }

  for (int i = 0; i < PACKET_HEADER_LEN; i++) {
    conv.chararr.chars[i] = len[i];
  }

  pid = conv.i;
  //NetLog.LogString("pid: " + to_string(pid));

  //DATA

  char* data;
  data = new char[dlen];
  iRes = 0;
  while (iRes != dlen) {
    iRes += recv(ConnectSocket, data + iRes, dlen - iRes, 0);
    //NetLog.LogString("RECiRes: " + to_string(iRes));
  }

  RecivePacket(reinterpret_cast<unsigned char*>(data), pid, dlen, this, ConnectedShip);
  delete data;
  return dlen;
}



void concat(vector<pair<unsigned char*, int> > in, unsigned char** C, int &lenC, bool destroy) {

  Packet_Header_Convertor conv;

  lenC = 0;

  for (int i = 0; i<in.size(); i++) {

    lenC += PACKET_HEADER_LEN + in[i].second;
  }

  *C = new unsigned char[lenC];

  int prevSector = 0;

  for (int i = 0; i<in.size(); i++) {
    conv.i = in[i].second;

    for (int i = 0; i < PACKET_HEADER_LEN; i++) {
      (*C)[prevSector + i] = conv.chararr.chars[i];
    }

    prevSector += PACKET_HEADER_LEN;

    for (int i = 0; i < in[i].second; i++) {
      (*C)[prevSector + i] = in[i].first[i];
    }

    prevSector += in[i].second;

    if (destroy) {
      delete in[i].first;
    }
  }


}

void split(unsigned char* data, int dataLen, vector<pair<unsigned char**, int> > &split, bool destroy) {
  Packet_Header_Convertor conv;

  int till = 0;
  while (till < dataLen) {
    for (int i = 0; i < PACKET_HEADER_LEN; i++) {
      conv.chararr.chars[i] = data[till + i];
    }

    till += PACKET_HEADER_LEN;

    unsigned char * text = new unsigned char[conv.i];

    for (int i = 0; i < conv.i; i++) {
      text[i] = data[till + i];
    }

    till += conv.i;

    split.push_back({ &text, conv.i });
  }

  if(destroy) {
    delete data;
  }
}