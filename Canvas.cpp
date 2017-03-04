#include "Canvas.h"

int Canvas::mouseEnter(int state) {
  return managers.mouseEntryManager(state);
}

int Canvas::mouseMoved(int mx, int my) {
  return managers.mouseMoveManager(mx, my);
}

int Canvas::mouseClicked(int mx, int my) {
  return managers.mouseClickManager(0, 0, mx, my);
}

int Canvas::keyPressed(unsigned char key, int mx, int my) {
  return managers.keyManager(key, mx, my);
}

int Canvas::specialPressed(int key, int mx, int my) {
  return managers.specialKeyManager(key, mx, my);
}

int Canvas::mouseWheel(int a, int b, int mx, int my) {
  return managers.mouseWheelManager(a, b, mx, my);
}

void Canvas::render() {
  managers.renderManager(cax, cay, cbx, cby);
}

