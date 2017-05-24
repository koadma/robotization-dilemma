#pragma once

#include "Game.h"

using namespace std;

void createClientBind();

bool loginRecv(unsigned char* data, int id, int dataLen, NetworkS* thisptr, Ship* ship);

int main(int argc, char** argv);