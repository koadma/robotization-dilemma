#include "Main.h"

NetworkS* newClientBind;
thread* newClientThread;

string port;
int players;

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
  newClientBind = new NetworkS(port, loginRecv);
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
  if (data->_children.size() >= 4) {
    if (data->_children[3]->_core->toType<string>().length()) {
      if(game->shipAuth.count(data->_children[3]->_core->toType<string>())) {
        return LoginErrorTryRejoin;
      }
    }
    return LoginErrorInvalidAuth;
  }
  return LoginErrorOk;
}

bool loginRecv(DataElement* data, int id, NetworkS* thisptr, NetBinder* ship) {
  if(ship == NULL) {
    int loginState = checkLogin(data, id);
    if (loginState == LoginErrorOk) { //If can join
      NetBinder* newShip = game->addShip();

      newShip->connectedClient = thisptr;
      thisptr->ConnectedBinder = newShip;

      DataElement* de = new DataElement();

      DataElement* state = new DataElement();
      state->_core->fromType<int>(loginState);
      de->addChild(state);

      DataElement* ide = new DataElement();
      ide->_core->fromType<uint64_t>(newShip->_droneID);
      de->addChild(ide);

      DataElement* codee = new DataElement();
      codee->_core->fromType<string>(newShip->code);
      de->addChild(codee);

      thisptr->SendData(de, PacketLogin);

      LOG INFO GAME "Client " << newShip->code << " accepted!" END;

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
        
          thisptr->ConnectedBinder = it->second;

          DataElement* de = new DataElement();

          DataElement* state = new DataElement();
          int statev = LoginErrorOk;
          state->_core->fromType<int>(statev);
          de->addChild(state);

          DataElement* ide = new DataElement();
          ide->_core->fromType<uint64_t>(it->second->_droneID);
          de->addChild(ide);

          DataElement* codee = new DataElement();
          codee->_core->fromType<string>(code);
          de->addChild(codee);

          thisptr->SendData(de, PacketLogin);

          detachCreateClientBind();

          LOG INFO "Client " << code << " re-connected!" END;

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

    LOG WARN "Client rejected " << loginState << "!" END;

    return 1;
  }
  else {
    return ship->packetRecv(data, id, thisptr);
  }
}


int VersionA = 0;
int VersionB = 8;
int VersionC = 3;

int main(int argc, char** argv)
{  
  ran1(-time(0));

  port = "1111";
  players = 2;

  for (int opt = 1; opt<argc; ++opt) {
    if (!strcmp(argv[opt], "-p"))      port = atoi(argv[++opt]);
    else if (!strcmp(argv[opt], "-n")) players = atoi(argv[++opt]);
    else { cerr << "ERROR: bad cmd line arg: " << argv[opt] END; exit(2); }
  }

  LOG INFO MISC "Starting server on port " << port << " with " << players << " players" END;

  game = new Game(players);
  detachCreateClientBind(); //Begin accepting clients

  int n;
  cin >> n;

  return 0;
}