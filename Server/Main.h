#pragma once

#include "Game.h"

using namespace std;

void createClientBind();

bool loginRecv(DataElement* data, int id, NetworkS* thisptr, NetBinder* ship);

int main(int argc, char** argv);