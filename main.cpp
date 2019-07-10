#include <SFML/Graphics.hpp>
#include <vector>

#include "game.h"

int main()
{
  srand((unsigned)time(nullptr));
  game g(2, 5);
  while(g.toContinue())
    g.update();

  return 0;
}
