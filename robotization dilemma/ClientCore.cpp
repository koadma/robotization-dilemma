#include "GUI/GraphicsUtils.h"

vector<pair<key, string>> keybinds; //key, display name

void keybindReply(key nkey, int id) {
  if (id < keybinds.size()) {
    keybinds[id].first = nkey;
  }
}

void loadKeybinds(string filename) {
  ifstream binds;
  binds.open(filename);

  keybinds.clear();

  int id;
  string display;
  int valt;
  int valv;

  while (binds >> id >> display >> valt >> valv) {
    replaceChar(display, '_', ' ');

    if (keybinds.size() <= id) {
      keybinds.resize(id + 1);
    }

    keybinds[id] = {key(valv, valt), display};
  }

  binds.close();
}

void saveKeybinds(string filename) {
  ofstream binds;
  binds.open(filename);

  int id = 0;
  for(auto&& it : keybinds) {
    string name = it.second;
    replaceChar(name, ' ', '_');

    binds << id << " " << name << " " << it.first._type << " " << it.first._keycode << endl;

    id++;
  }

  binds.close();
}