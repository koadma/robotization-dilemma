#include "Main.h"

bool recivePacket(DataElement* data, int id, NetworkC* client, Ship* lship) {
  if (ship == NULL) {
    switch (id) {
    case PacketLogin:
      if (data->_children[0]->_core->toType<int>() == LoginErrorOk) {
        Connection->ConnectedShip = ship = new Ship(data->_children[1]->_core->toType<uint64_t>());
        
        ofstream last("lastserver.dat");
        last << client->_IP << endl;
        last << client->_port << endl;
        last << data->_children[2]->_core->toType<string>() << endl;
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
            MainGameCanvas::keyManager,
            MainGameCanvas::specialKeyManager,
            MainGameCanvas::keyUpManager,
            MainGameCanvas::specialKeyUpManager,
            MainGameCanvas::mouseEntryManager,
            MainGameCanvas::mouseMoveManager,
            MainGameCanvas::mouseClickManager,
            MainGameCanvas::mouseWheelManager
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
            MainGameShipCanvas::keyManager,
            MainGameShipCanvas::specialKeyManager,
            MainGameShipCanvas::keyUpManager,
            MainGameShipCanvas::specialKeyUpManager,
            MainGameShipCanvas::mouseEntryManager,
            MainGameShipCanvas::mouseMoveManager,
            MainGameShipCanvas::mouseClickManager,
            MainGameShipCanvas::mouseWheelManager
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
  i >> port;
  if (!i.good()) {
    return false;
  }
  i >> code;
  i.close();
  
  data = {ip, port, code};

  return true;
}

void testKey(key k) {
  cout << k.keycode << " " << k.toName() << endl;
}

int main() {
  setlocale(LC_ALL, "");
  srand(time(NULL));

  InitGraphics();

  /*
  FlowGraph<double, Fraction, double> f;

  FlowVertex<double, Fraction, double>* v1 = f.addVertex(-5, 10, -10, 100, 30, -1);
  FlowVertex<double, Fraction, double>* v2 = f.addVertex(0, 20, -30, 100, 50, -1);
  FlowVertex<double, Fraction, double>* v3 = f.addVertex(10, 0, 0, 0, 0, 0);

  f.addSymmetricEdge(v1, v2, 30);
  f.addSymmetricEdge(v2, v3, 10);

  for(int i = 0; i < 4; i++) {
    f.goTo(i);
  }
  f.goTo(4, v2, -100);
  for (int i = 5; i < 18; i++) {
    f.goTo(i);
  }
  f.goTo(18, v2, 120);
  f.goTo(19);
  v3->_goal = 1;
  for (int i = 20; i < 35; i++) {
    f.goTo(i);
  }
  v3->_goal = -5;
  for (int i = 35; i < 61; i++) {
    f.goTo(i);
  }
  

  Graphics::PlotHwnd plt = Graphics::createPlot(objectGameSubWindow, "plot", Coordiante{0, 0}, Coordiante{1, 1}, hexToInt("ff000000"), hexToInt("ffffffff"), hexToInt("ff00ff00"));
  plt->plotData.push_back(new PlotLineVUT<linear<double, Fraction, double>, double, double>(&(v2->_val), hexToInt("ffff0000")));
  plt->plotData.push_back(new PlotLineVUT<linear<double, Fraction, double>, double, double>(&(v1->_val), hexToInt("ffffff00")));// */

  //Graphics::ControlHwnd ctrl = Graphics::createControlHwnd(objectGameSubWindow, "ctrltest", Coordiante{ 0.5, 0.5, -50, -15 }, Coordiante{ 0.5, 0.5, 50, 15 }, hexToInt("ff070918"), hexToInt("ff190077"), hexToInt("ff838fa1"), key{0}, testKey);
  
  /*
  vector<string> reConnectData;
  if (hasSaveFile(reConnectData)) {
    createReconnectScreen();
    connectServer(reConnectData[0], reConnectData[1], reConnectData[2]);
  }
  else {
    createMainMenu();
  }// */

  loadKeybinds("html/keybinds.cfg");

  Graphics::TableHwnd table = Graphics::createTable("asd", {0,0}, {1, 1}, 0);
  createSettings(table);
  Graphics::addElement(objectGameSubWindow, table);

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
