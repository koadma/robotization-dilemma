#include "Main.h"

NetworkS* newClientBind;
Game g;

void bindRecv(unsigned char* data, int id, int dataLen) {

}

int main(int argc, char** argv)
{
  newClientBind = new NetworkS("1111", bindRecv);
  //Game game(2);
  return 0;
}
