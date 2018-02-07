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
  cout << k._keycode << " " << k.toName() << endl;
}

int main() {
  setlocale(LC_ALL, "");
  srand(time(NULL));
  ran1(time(NULL));

  /*for (int i = 0; i<10000000; i++) {
    if (!(i % 100000)) {
      cout << i << endl;
    }
    PolynomialD poly(vector<long double>({1}));
    double s1 = ran1() * 0.1 - 3;
    poly = poly * PolynomialD(vector<long double>{-s1, 1 });
    double s2 = ran1() * 0.1 - 3;
    poly = poly * PolynomialD(vector<long double>{-s2, 1 });
    double frs = ran1() * 10 - 5;
    double fval = ran1() * 10 + 1;
    poly = poly * PolynomialD(vector<long double>{ frs*frs+fval, -2 * frs, 1});
    vector<long double> roots = poly.solveNewton();
    bool ok1 = false;
    bool ok2 = false;
    for(int r = 0; r < roots.size(); r++) {
      if (roots[r] - 0.0001 <= s1 && s1 <= roots[r] + 0.0001) {
        ok1 = true;
      }
      if (roots[r] - 0.0001 <= s2 && s2 <= roots[r] + 0.0001) {
        ok2 = true;
      }
    }
    if (!(ok1 && ok2)) {
      cout << "MISS" << roots.size() << '\t' << s1 << '\t' << s2 << '\t' << frs << '\t' << fval << endl;
      poly.solveNewton();
    }
  }*/

  loadKeybinds();
  loadColors();

  InitGraphics();

  vector<string> reConnectData;
  if (hasSaveFile(reConnectData)) {
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
