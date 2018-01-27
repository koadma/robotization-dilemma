#include "LabelBind.h"

void LabelBind::render() {
  //if(text != NULL) {
    renderBitmapString((cax + ((align) ? cbx : cax)) / 2, (cay + cby) / 2, text->tostr(), textColor, align);
  //}
}

