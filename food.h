#ifndef _FOOD_H_
#define _FOOD_H_

#include <SFML/Graphics.hpp>
#include <stdlib.h>

#include "global.h"

class food : public sf::RectangleShape
{
public:
  food() : RectangleShape(sf::Vector2f(10, 10))
  {
    time_t t;
    float x, y;
    srand((unsigned)time(&t));
    x = rand() % (windowWidth-20) + 10;
    y = rand() % (windowHeight-20) + 10;
    setPosition(x, y);
    setFillColor(sf::Color::Green);
  }

  void respawn()
  {
    float x, y;
    x = rand() % (windowWidth-20) + 10;
    y = rand() % (windowHeight-20) + 10;
    setPosition(x, y);
  }

  bool gotEaten(sf::FloatRect const& headBounds) const
  {
    return getGlobalBounds().intersects(headBounds);
  }
};

#endif
