#include "Main.h"

int main()
{
  setlocale(LC_ALL, "");
  srand(time(NULL));
  InitWindow();
  glutMainLoop();
  return 0;
}
