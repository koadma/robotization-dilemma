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
void drawXZPlane(float camcx, float camcy, float camcz, float d, int n);

void renderNewRound(int id);

power_type_W getCurrentMaxGeneratedPower();
power_type_W getCurrentGeneratedPower();
power_type_W getCurrentMaxUseablePower();
power_type_W getCurrentRequestedPower();
power_type_W getCurrentUsedPower();
energy_type_J getCurrentMaxEnergy();
energy_type_J getCurrentStoredEnergy();

string isSurefire();

void createMainMenu();

void createReconnectScreen();

void createSettings(Graphics::TableHwnd& table);