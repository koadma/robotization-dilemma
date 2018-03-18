#include "Main.h"

bool recivePacket(DataElement* data, int id, NetworkC* client, Ship* lship) {
  if (ship == NULL) {
    switch (id) {
    case PacketLogin:
      if (data->_children[0]->_core->toType<int>() == LoginErrorOk) {

        Connection->ConnectedShip = ship = new Ship(data->_children[1]->_core->toType<uint64_t>());
        
        ofstream last("lastserver.dat");
        last << client->_IP END;
        last << client->_port END;
        last << data->_children[2]->_core->toType<string>() END;
        last.close();

        ship->connectedServer = Connection;

        Graphics::setElements(objectGameSubWindow, "html/game_screen.xml");

        objectMainGameCanvas = Graphics::createCanvas(
          "objectMainGameCanvas",
          Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f },
          Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f },
          IWindowManagers{
            MainGameCanvas::renderManager,
            MainGameCanvas::resizeManager,
            MainGameCanvas::guiEventManager,
            MainGameCanvas::mouseEntryManager,
            MainGameCanvas::mouseMoveManager,
          }
        );
        Graphics::addElement(
          reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectMainGameCanvasContainer")),
          objectMainGameCanvas);
        objectMainGameShipCanvas = Graphics::createCanvas(
          "objectMainGameShipCanvas",
          Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f },
          Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f },
          IWindowManagers{
            MainGameShipCanvas::renderManager,
            MainGameShipCanvas::resizeManager,
            MainGameShipCanvas::guiEventManager,
            MainGameShipCanvas::mouseEntryManager,
            MainGameShipCanvas::mouseMoveManager
           }
        );
        Graphics::addElement(
          reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectMainGameShipCanvasContainer")),
          objectMainGameShipCanvas);

        bindLabels();

        glutPostRedisplay();
      }
      else {
        createMainMenu();
        glutPostRedisplay();
      }
      break;
    }
  }
  else {
    ship->packetRecv(data, id, client);
  }

  return 0;
}

bool hasSaveFile(vector<string>& data) {
  ifstream i("lastserver.dat");
  string ip;
  string port;
  string code;
  if (!i.good()) {
    return false;
  }
  getline(i, ip);
  if (!i.good()) {
    return false;
  }
  getline(i, port);
  if (!i.good()) {
    return false;
  }
  getline(i, code);
  i.close();
  
  data = {ip, port, code};

  return true;
}

void testKey(key k) {
  LOG INFO MISC k._keycode << " " << k.toName() END;
}

int main() {
  /*Fraction a;
  Fraction b;
  
  while (true) {
    cin >> a;
    b = sqrt(a);
    cout << a.a << " " << a.b << " " << a.toDouble() << " " << a.toStr(20) << endl;
    cout << b.a << " " << b.b << " " << b.toDouble() << " " << b.toStr(20) << endl;
  }
  */

  setlocale(LC_ALL, "");
  srand(time(NULL));
  ran1(time(NULL));

  loadKeybinds();
  loadColors();

  InitGraphics();
  
  /*ifstream sensfile("html/sensor_detect.xml");

  std::stringstream buffer;
  buffer << sensfile.rdbuf();
  sensfile.close();

  xml_document<> doc;
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  Container* testc = new Container("ctest,", { 0,0 }, { 1,1 }, 0xff111111);
  ScriptGUI* test = new ScriptGUI("stest,", { 0,0 }, { 1,1 }, 0xff111111, 0xff888888, 0xffffffff, 0xff222222);
  test->code = new ScriptIBlock();
  test->code->load(doc.first_node("root"));
  testc->element = test;

  Graphics::addElement(objectMenuSubWindow, testc);*/

  
  vector<string> reConnectData;
  if (hasSaveFile(reConnectData) && reConnectData[2] != "") {
    createReconnectScreen();
    connectServer(reConnectData[0], reConnectData[1], reConnectData[2]);
  }
  else {
    createMainMenu();
  }
  glutPostRedisplay();
  glutMainLoop();
}

int CALLBACK WinMain(
  HINSTANCE   hInstance,
  HINSTANCE   hPrevInstance,
  LPSTR       lpCmdLine,
  int         nCmdShow
) {
  return main();
}
