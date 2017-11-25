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
          reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectMainGameCanvasContainer")),
          "objectMainGameCanvas",
          Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f },
          Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f },
          IWindowManagers{
            MainGameCanvas::renderManager,
            MainGameCanvas::resizeManager,
            MainGameCanvas::keyManager,
            MainGameCanvas::specialKeyManager,
            MainGameCanvas::mouseEntryManager,
            MainGameCanvas::mouseMoveManager,
            MainGameCanvas::mouseClickManager,
            MainGameCanvas::mouseWheelManager
          }
        );
        objectMainGameShipCanvas = Graphics::createCanvas(
          reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectMainGameShipCanvasContainer")),
          "objectMainGameShipCanvas",
          Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f },
          Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f },
          IWindowManagers{
            MainGameShipCanvas::renderManager,
            MainGameShipCanvas::resizeManager,
            MainGameShipCanvas::keyManager,
            MainGameShipCanvas::specialKeyManager,
            MainGameShipCanvas::mouseEntryManager,
            MainGameShipCanvas::mouseMoveManager,
            MainGameShipCanvas::mouseClickManager,
            MainGameShipCanvas::mouseWheelManager
           }
        );

        bindLabels();

        glutPostRedisplay();
      }
      else {
        createMainMenu();
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

int main() {
  setlocale(LC_ALL, "");
  srand(time(NULL));

  InitGraphics();
  vector<string> reConnectData;
  if (hasSaveFile(reConnectData)) {
    createReconnectScreen();
    connectServer(reConnectData[0], reConnectData[1], reConnectData[2]);
  }
  else {
    createMainMenu();
  }

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
