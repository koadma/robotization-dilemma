#pragma once

#include "../Core/Ship.h"

void ingameMenuSensorInput(string s);

void ingameMenuCommitButton();

void ingameMenuExitButton();

void joinMenuInput(string inp);

void startServer(int players);

void connectServer(string ip, string port, string code);

void joinMenuInputButton();

void gameMenuJoinButton();

void mainMenuPlayButton();

void mainMenuExitButton();

void settingsMenuInput(key nkey, int id);

void bindLabels();

int InitGraphics();