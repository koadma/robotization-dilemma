#include "ControlSetting.h"

int ControlSetting::guiEvent(gui_event evt, int mx, int my, set<key>& down) {
  bool oactive = active;
  if(evt._key._type == key::type_mouse) { //mouse
    if(evt._type == gui_event::evt_pressed && evt._key._keycode == 0) { //click, left
      active = isIn(mx, my);
    }
    return oactive xor active;
  }
  if (active && (evt._key._type == key::type_key || evt._key._type == key::type_special)) { //keyboard
    if(evt._type == evt.evt_pressed) { //pressed
      selected = evt._key;
      input(selected, id);
      return 1;
    }
  }
  return 0;
}

void ControlSetting::render() {
  glBegin(GL_QUADS);
  if (active) {
    setColor(activeColor);
  }
  else {
    setColor(bgColor);
  }
  glVertex2d(cax, cay);
  glVertex2d(cbx, cay);
  glVertex2d(cbx, cby);
  glVertex2d(cax, cby);
  glEnd();

  renderBitmapString((cax+cbx) / 2.0f, (cay + cby) / 2.0f, selected.toName(), textColor, true);
  //shapesPrintf(0, 0, text.c_str());
}

