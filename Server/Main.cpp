#include "Main.h"

NetworkS* newClientBind;
thread* newClientThread;

void detachCreateClientBind() {
  if (newClientBind && !newClientThread->joinable()) {
    throw 1;
  }
  if(newClientBind) {
    newClientThread->join();
    delete newClientThread;
  }
  newClientThread = new thread(createClientBind); //Set up new listener
}

void createClientBind() {
  newClientBind = new NetworkS("1111", loginRecv);
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
    if (data->_children.size() < 4) {
      return LoginErrorGameFull;
    }
  }
  if (data->_children.size() >= 4 && data->_children[3]->_core->toType<string>().length()) {
    if(game->shipAuth.count(data->_children[3]->_core->toType<string>())) {
      return LoginErrorTryRejoin;
    }
    else {
      return LoginErrorInvalidAuth;
    }
  }
  return LoginErrorOk;
}

bool loginRecv(DataElement* data, int id, NetworkS* thisptr, Ship* ship) {
  if(ship == NULL) {
    int loginState = checkLogin(data, id);
    if (loginState == LoginErrorOk) { //If can join
      Ship* newShip = game->addShip();

      newShip->connectedClient = thisptr;
      thisptr->ConnectedShip = newShip;

      DataElement* de = new DataElement();

      DataElement* state = new DataElement();
      state->_core->fromType<int>(loginState);
      de->addChild(state);

      DataElement* ide = new DataElement();
      ide->_core->fromType<uint64_t>(ship->_droneID);
      de->addChild(ide);

      DataElement* codee = new DataElement();
      codee->_core->fromType<string>(ship->code);
      de->addChild(codee);

      thisptr->SendData(de, PacketLogin);

      cout << "Client " << ship->code << " accepted!" << endl;

      game->tryGameStart();

      detachCreateClientBind();
      //Thread will self terminate when connection is established.

      return 0;
    }
    if (loginState == LoginErrorTryRejoin) {
      string code = data->_children[3]->_core->toType<string>();

      auto it = game->shipAuth.find(code);

      if (it != game->shipAuth.end()) {
        if (!it->second->connectedClient->Running) {
          delete it->second->connectedClient;

          it->second->connectedClient = thisptr;
        
          thisptr->ConnectedShip = it->second;

          DataElement* de = new DataElement();

          DataElement* state = new DataElement();
          state->_core->fromType<int>(LoginErrorOk);
          de->addChild(state);

          DataElement* ide = new DataElement();
          ide->_core->fromType<uint64_t>(it->second->_droneID);
          de->addChild(ide);

          DataElement* codee = new DataElement();
          codee->_core->fromType<string>(code);
          de->addChild(codee);

          thisptr->SendData(de, PacketLogin);

          detachCreateClientBind();

          cout << "Client " << code << " re-connected!" << endl;

          return 0;
        }
      }
      loginState = LoginErrorInvalidAuth;
    }
    
    DataElement* de = new DataElement();
    
    DataElement* state = new DataElement();
    state->_core->fromType<int>(loginState);
    de->addChild(state);

    thisptr->SendData(de, PacketLogin);

    detachCreateClientBind();

    //this_thread::sleep_for(10s);

    cout << "Client rejected " << loginState << "!" << endl;

    return 1;
  }
  else {
    return ship->packetRecv(data, id, thisptr);
  }
}

int main(int argc, char** argv)
{  
  ran1(-time(0));

  game = new Game(2);
  detachCreateClientBind(); //Begin accepting clients

  int n;
  cin >> n;

  return 0;
}