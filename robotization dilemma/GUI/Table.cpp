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

  getRect();
}

void TableRow::getRect() {
  int chx = 0;

  auto it = data.begin();

  while (it != data.end()) {
    (*it)->getRect(cbx - cax, cby - cay, chx, cay);
    chx = (*it)->cby;
    ++it;
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

  getRect();
}

void Table::getRect() {
  int chy = cay - scroll;

  auto it = data.begin();

  while (it != data.end()) {
    (*it)->getRect(cbx - cax, cby - cay, cax, chy);
    chy = (*it)->cby;
    ++it;
  }
}