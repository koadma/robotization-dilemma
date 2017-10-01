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
      int sid = game->drones.size();
      Ship* newShip = new Ship(sid);
      newShip->connectedClient = thisptr;
      thisptr->ConnectedShip = newShip;

      DataElement* de = new DataElement();

      DataElement* state = new DataElement();
      state->_core->fromType<int>(loginState);
      de->addChild(state);

      DataElement* ide = new DataElement();
      ide->_core->fromType<uint64_t>(sid);
      de->addChild(ide);

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

// Use to convert bytes to MB
#define DIV 1048576

// Use to convert bytes to MB
//#define DIV 1024

// Specify the width of the field in which to print the numbers. 
// The asterisk in the format specifier "%*I64d" takes an integer 
// argument and uses it to pad and right justify the number.

#define WIDTH 7

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

  /*ScriptIConstant cons0;
  cons0._val = new ScriptData();
  cons0._val->_data.fromType<double>(0);
  cons0._val->type = ScriptData::TNUMERIC;
  ScriptIConstant cons1;
  cons1._val = new ScriptData();
  cons1._val->_data.fromType<double>(1);
  cons1._val->type = ScriptData::TNUMERIC;
  ScriptIConstant cons2;
  cons2._val = new ScriptData();
  cons2._val->_data.fromType<double>(2);
  cons2._val->type = ScriptData::TNUMERIC;
  ScriptIConstant cons5;
  cons5._val = new ScriptData();
  cons5._val->_data.fromType<double>(5);
  cons5._val->type = ScriptData::TNUMERIC;
  ScriptIConstant cons6;
  cons5._val = new ScriptData();
  cons5._val->_data.fromType<double>(6);
  cons5._val->type = ScriptData::TNUMERIC;
  ScriptIConstant cons7;
  cons5._val = new ScriptData();
  cons5._val->_data.fromType<double>(7);
  cons5._val->type = ScriptData::TNUMERIC;
  ScriptIConstant cons10;
  cons5._val = new ScriptData();
  cons5._val->_data.fromType<double>(10);
  cons5._val->type = ScriptData::TNUMERIC;
  
  ScriptIVariable ln1lhsarr;
  ln1lhsarr._arg = "a";

  ScriptIIndex ln1lhs;
  ln1lhs._arg = &ln1lhsarr;
  ln1lhs._ind = &cons0;

  ScriptIAssign ln1;
  ln1._to = &ln1lhs;
  ln1._val = &cons5;

  ScriptIVariable ln2lhs;
  ln2lhs._arg = "b";

  ScriptIVariable ln2rhsarr;
  ln2rhsarr._arg = "a";

  ScriptIIndex ln2rhs;
  ln2rhs._arg = &ln2rhsarr;
  ln2rhs._ind = &cons0;

  ScriptIAssign ln2;
  ln2._to = &ln2lhs;
  ln2._val = &ln2rhs;

  ScriptIVariable ln3lhs;
  ln3lhs._arg = "b";

  ScriptIVariable ln3rhsr;
  ln3rhsr._arg = "b";

  ScriptIMath ln3rhs;
  ln3rhs._arg1 = &cons2;
  ln3rhs._arg2 = &ln3rhsr;
  ln3rhs._oper = ScriptIMath::PLUS;

  ScriptIAssign ln3;
  ln3._to = &ln3lhs;
  ln3._val = &ln3rhs;

  ScriptIBlock root;
  root._instructions.push_back(&ln1);
  root._instructions.push_back(&ln2);
  root._instructions.push_back(&ln3);*/

  /*ScriptIBlock root;

  ScriptData* d;

  std::ifstream file("prog.prg");
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  xml_document<> doc;
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  ScriptInstruction* ni;

  root.load(doc.first_node("root"));

  //Memory safety test
  for(int i=0;i<1000000; i++) {

    d = new ScriptData();

    DeletePtr(root.run(*d));

    DeletePtr(d);
  }*/

  int n;
  cout << "Press any key to exit" << endl;
  cin >> n;

  return 0;
}