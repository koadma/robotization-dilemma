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
  int val;

  while (binds >> id >> display >> val) {
    replaceChar(display, '_', ' ');

    if (keybinds.size() <= id) {
      keybinds.resize(id + 1);
    }

    keybinds[id] = {key{val}, display};
  }

  binds.close();
}