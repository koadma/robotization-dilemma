#pragma once

#include "../Core/Ship.h"

void ingameMenuCommitButton(string sender);

void ingameMenuExitButton(string sender);

void joinMenuInput(string inp);

void startServer(int players);

void connectServer(string ip, string port, string code);

void joinMenuInputButton(string sender);

void mainMenuExitButton(string sender);

void settingsMenuInput(key nkey, int id);

void bindLabels();

int InitGraphics();