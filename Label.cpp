#include "Label.h"

/*Button::Button(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, ClickCallback lclickCallback) {
x = lx;
y = ly;
width = lwidth;
height = lheight;
bgColor = lbg;
activeColor = lactive;
textColor = ltextColor;
text = ltext;
clickCallback = lclickCallback;
active = false;
}*/

void Label::render() {
  renderBitmapString(x + width / 2.0f, y + height / 2.0f, text, textColor, align);
  //shapesPrintf(0, 0, text.c_str());
}

