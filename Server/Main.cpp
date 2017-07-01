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

int checkLogin(DataElement* data, int id) {
  if (id != PacketLogin) {
    return LoginErrorProtocolError;
  }
  
  if (data->_children.size() < 3) {
    return LoginErrorProtocolError;
  }
  if (!isCompatible(
    data->_children[0]->_core->toType<int>(),
    data->_children[1]->_core->toType<int>(),
    data->_children[2]->_core->toType<int>()
  )) {
    return LoginErrorVersionError;
  }
  if (game->state != Game::Joining) {
    return LoginErrorGameFull;
  }
  return LoginErrorOk;
}

bool loginRecv(DataElement* data, int id, NetworkS* thisptr, Ship* ship) {
  if(ship == NULL) {
    int loginState = checkLogin(data, id);
    if (loginState == LoginErrorOk) { //If can join

      Ship* newShip = new Ship();
      newShip->connectedClient = thisptr;
      thisptr->ConnectedShip = newShip;

      DataElement* de = new DataElement();
      de->_core->fromType<int>(loginState);
      thisptr->SendData(de, PacketLogin);

      game->addShip(newShip);

      cout << "Client accepted" << endl;

      detachCreateClientBind();
      //Thread will self terminate when connection is established.

      return 0;
    }
    else {
      DataElement* de = new DataElement();
      de->_core->fromType<int>(loginState);
      thisptr->SendData(de, PacketLogin);

      //this_thread::sleep_for(10s);

      cout << "Client rejected " << loginState << endl;

      return 1;
    }
  }
  else {
    return ship->packetRecv(data, id, thisptr);
  }
}

int main(int argc, char** argv)
{
  cout << sizeof(NetworkC) << endl;
  cout << sizeof(NetworkS) << endl;
  cout << sizeof(Ship) << endl;
  cout << sizeof(Sighting) << endl;
  cout << sizeof(Object) << endl;
  cout << sizeof(Movement) << endl;
  cout << sizeof(Path) << endl;

  game = new Game(2);
  detachCreateClientBind(); //Begin accepting clients

  int n;
  cout << "Press any key to exit" << endl;
  cin >> n;

  return 0;
}
