#pragma once

#include "../Core/Path/Bubble.h"
#include "../Core/Path/Shot.h"
#include "../Core/Path/Movement.h"

extern bool recivePacket(DataElement* data, int id, NetworkC* client, Ship* ship);

struct key;

extern vector<pair<key, string>> keybinds; //key, display name

void keybindReply(key nkey, int id);

void loadKeybinds(string filename = "html/keybinds.cfg");
void saveKeybinds(string filename = "html/keybinds.cfg");