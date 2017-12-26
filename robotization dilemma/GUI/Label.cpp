#include "Label.h"

void Label::render() {
  renderBitmapString((cax + cbx) / 2, (cay + ((align) ? cby : cay)) / 2, text, textColor, align);
  //shapesPrintf(0, 0, text.c_str());
}

