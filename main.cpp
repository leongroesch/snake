#include <SFML/Graphics.hpp>
#include <vector>

#include "game.h"

int main(int argc, char** argv)
{
  int snakes = 2;
  if(argc > 1)
    snakes = 1;
  srand((unsigned)time(nullptr));
  game g(snakes, 5);
  while(g.toContinue())
    g.update();

  return 0;
}
