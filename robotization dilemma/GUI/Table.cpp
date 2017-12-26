#include "Table.h"

void TableRow::render() {
  for (auto&& it : data) {
    it->render();
  }
}

void TableRow::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  int n = 0;

  auto it = data.begin();

  while (it != data.end()) {
    (*it)->getRect((cbx - cax) / data.size(), cby - cay, cax + n / float(data.size()) * (cbx - cax), cay);
    ++it;
    ++n;
  }
}

void Table::render() {
  glViewport(cax, cay, cbx - cax, cby - cay);

  for(auto&& it : data) {
    it->render();
  }

  Graphics::resetViewport();
}

void Table::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  float chy = height.GetY(cby - cay);

  int n = 0;

  auto it = data.begin();
  
  while (it != data.end()) {
    (*it)->getRect(cbx - cax, chy, cax, cay + n*chy - scroll);
    ++it;
    ++n;
  }
}