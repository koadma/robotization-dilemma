#include "Main.h"

vector<mVec3> shipStarts;

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
      int sid = game->drones.size();

      Ship* newShip = new Ship(sid, shipStarts[sid]);

      newShip->connectedClient = thisptr;
      thisptr->ConnectedShip = newShip;

      string code = game->addShip(newShip);

      DataElement* de = new DataElement();

      DataElement* state = new DataElement();
      state->_core->fromType<int>(loginState);
      de->addChild(state);

      DataElement* ide = new DataElement();
      ide->_core->fromType<uint64_t>(sid);
      de->addChild(ide);

      DataElement* codee = new DataElement();
      codee->_core->fromType<string>(code);
      de->addChild(codee);

      thisptr->SendData(de, PacketLogin);

      cout << "Client " << code << " accepted!" << endl;

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
  /*cout << sizeof(NetworkC) << endl;
  cout << sizeof(NetworkS) << endl;
  cout << sizeof(Ship) << endl;
  cout << sizeof(Sighting) << endl;
  cout << sizeof(Object) << endl;
  cout << sizeof(Movement) << endl;
  cout << sizeof(Path) << endl;
  cout << sizeof(long double) << endl;*/

  ran1(-time(0));

  shipStarts = randstartpos(1000000.0);

  game = new Game(2);
  detachCreateClientBind(); //Begin accepting clients

  /*ScriptIBlock root;

  ScriptData* d;

  std::ifstream file("prog.prg");
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  xml_document<> doc;
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  root.load(doc.first_node("root"));
  d = new ScriptData();
  DeletePtr(root.run(*d));
  DeletePtr(d);
  */

  int n;
  cin >> n;

  return 0;
}