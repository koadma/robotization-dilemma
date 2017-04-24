#include "Main.h"

NetworkS* newClientBind;
Game* game;

void createClientBind() {
  newClientBind = new NetworkS("1111", bindRecv);
}

bool isCompatible(int va, int vb, int vc) {
  return (va == VersionA);
}

int checkLogin(unsigned char* data, int id, int dataLen) {
  if (id != PacketLogin) {
    return LoginErrorProtocolError;
  }
  string s(reinterpret_cast<char*>(data), dataLen);
  vector<string> args = tokenize(s,';');
  if (args.size() < 3) {
    return LoginErrorProtocolError;
  }
  if (!isCompatible(strTo<int>(args[0]), strTo<int>(args[1]), strTo<int>(args[2]))) {
    return LoginErrorVersionError;
  }
  if (game->state != Game::Joining) {
    return LoginErrorGameFull;
  }
  return LoginErrorOk;
}

void loginRecv(unsigned char* data, int id, int dataLen, NetworkS* thisptr, Ship* ship) {
  int loginState = checkLogin(data, id, dataLen);
  if (loginState == LoginErrorOk) { //If can join
    Ship* newShip = new Ship();
    newShip->connectedClient = thisptr;

    game->addShip(newShip);

    thisptr->ConnectedShip = ship;
    thisptr->RecivePacket = shipPacketRecv; //Hand over processing to ship
    thisptr->SendData(loginState, PacketLogin);

    createClientBind(); //Set up new listener;
  }
  else {
    thisptr->SendData(loginState, PacketLogin);
    thisptr->SendData("", 0, 0);

    delete thisptr; //Remove binding.
  }
}

void bindRecv(unsigned char* data, int id, int dataLen, NetworkS* thisptr, Ship* ship) {
  newClientBind->RecivePacket = loginRecv; //Hand over handling to login
  //OBJECT LOSES POINTER
  createClientBind(); //Set up new listener;
}

int main(int argc, char** argv)
{
  game = new Game(2);
  createClientBind(); //Begin accepting clients
  return 0;
}
