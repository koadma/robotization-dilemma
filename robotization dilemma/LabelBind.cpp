#include "LabelBind.h"

void LabelBind::render() {
  renderBitmapString((cax + cbx) / 2, (cay + ((align) ? cby : cay)) / 2, text->tostr(), textColor, align);
}

