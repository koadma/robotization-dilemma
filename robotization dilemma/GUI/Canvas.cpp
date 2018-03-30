#include "Canvas.h"

int Canvas::mouseEnter(int state) {
  return managers.mouseEntryManager(state);
}

int Canvas::mouseMoved(int mx, int my) {
  return managers.mouseMoveManager(mx, my);
}

int Canvas::guiEvent(gui_event evt, int mx, int my, set<key_location>& down) {
  return managers.guiEventManager(evt, mx, my, down, this);
}

void Canvas::render() {
  managers.renderManager(cax, cay, cbx, cby);
}

Canvas::~Canvas() {

}