#include "Main.h"

NetworkS** newClientBind;

void detachCreateClientBind() {
  thread* s = new thread(createClientBind); //Set up new listener
}

void createClientBind() {
  newClientBind = new NetworkS*;
  *newClientBind = new NetworkS("1111", loginRecv);
}

bool isCompatible(int va, int vb, int vc) {
  return (va == VersionA);
}

int checkLogin(unsigned char* data, int id, int dataLen) {
  if (id != PacketLogin) {
    return LoginErrorProtocolError;
  }
  
  vector<pair<unsigned char*, int> > args;
  split(data, dataLen, args);
  if (args.size() < 3) {
    return LoginErrorProtocolError;
  }
  if (!isCompatible(deserializeT<int>(args[0].first, args[0].second), deserializeT<int>(args[1].first, args[1].second), deserializeT<int>(args[2].first, args[2].second))) {
    return LoginErrorVersionError;
  }
  if (game->state != Game::Joining) {
    return LoginErrorGameFull;
  }
  return LoginErrorOk;
}

bool loginRecv(unsigned char* data, int id, int dataLen, NetworkS* thisptr, Ship* ship) {
  if(ship == NULL) {
    int loginState = checkLogin(data, id, dataLen);
    if (loginState == LoginErrorOk) { //If can join
      Ship* newShip = new Ship();
      newShip->connectedClient = thisptr;

      game->addShip(newShip);

      thisptr->ConnectedShip = newShip;
      thisptr->SendData(loginState, PacketLogin);

      cout << "Client accepted" << endl;

      detachCreateClientBind();
      //Thread will self terminate when connection is established.

      return 0;
    }
    else {
      thisptr->SendData(loginState, PacketLogin);

      //this_thread::sleep_for(10s);

      cout << "Client rejected " << loginState << endl;

      return 1;
    }
  }
  else {
    return ship->packetRecv(data, id, dataLen, thisptr);
  }
}

int main(int argc, char** argv)
{
  game = new Game(2);
  detachCreateClientBind(); //Begin accepting clients

  int n;
  cout << "Press any key to exit" << endl;
  cin >> n;

  return 0;
}
