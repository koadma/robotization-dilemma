#pragma once

#include "Game.h"

#include <windows.h>
#include <Psapi.h>

using namespace std;

void createClientBind();

bool loginRecv(DataElement* data, int id, NetworkS* thisptr, Ship* ship);

int main(int argc, char** argv);