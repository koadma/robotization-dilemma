#include "Main.h"


bool recivePacket(DataElement* data, int id, NetworkC* client, Ship* lship) {
  if (ship == NULL) {
    switch (id) {
    case PacketLogin:
      if (data->_core->toType<int>() == LoginErrorOk) {
        Connection->ConnectedShip = ship = new Ship();

        ship->connectedServer = Connection;

        Graphics::setElements(objectGameSubWindow, "html/game_screen.html");

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


int CALLBACK WinMain(
  HINSTANCE   hInstance,
  HINSTANCE   hPrevInstance,
  LPSTR       lpCmdLine,
  int         nCmdShow
) {
  setlocale(LC_ALL, "");
  srand(time(NULL));
  InitWindow();
  glutMainLoop();
  return 0;
}
