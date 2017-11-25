#pragma once

#include "GUI/Graphics.h"

extern Graphics::WinHwnd objectMainWindow;
extern Graphics::PanelHwnd objectGameSubWindow;
extern Graphics::CanvasHwnd objectMainGameCanvas;
extern Graphics::CanvasHwnd objectMainGameShipCanvas;
extern Graphics::PanelHwnd objectMenuSubWindow;

extern NetworkC *Connection;

float wrapAngle(float angle);
void drawPointingVector(float camcx, float camcy, float camcz, float x, float y, float z, int l);
void drawCoordinatingSystem(float camcx, float camcy, float camcz, float d);

void renderNewRound(int id);

power_type_W getCurrentUsedPower();
power_type_W getCurrentMaxPower();
power_type_W getCurrentGeneratedPower();

string isSurefire();

void createMainMenu();

void createReconnectScreen();