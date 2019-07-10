#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <vector>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "global.h"
#include "block.h"

class Snake
{
private:
  std::vector<std::shared_ptr<block>> recVector;
  sf::FloatRect windowBounds;
  sf::Color color;
  sf::Vector2f startPosition;

  void constStartSnake()
  {
    block head(Direction::up, color);
    head.setPosition(startPosition.x, startPosition.y);
    recVector.push_back(std::make_shared<block>(head));

    for(int i=1; i < startLength; i++)
    {
      addTail();
    }
  }

public:
  Snake(sf::Color _color, sf::Vector2f _startPosition)
    : windowBounds(0, 0, windowWidth, windowHeight), color(_color), startPosition(_startPosition)
  {
    constStartSnake();
  }

  void draw(sf::RenderWindow& window)
  {
    for(auto const& x : recVector)
    {
      window.draw(*x);
    }
  }

  void update()
  {
    for(auto & x : recVector)
    {
      x->update();
    }
  }

  void keyPressed(Direction key)
  {
    auto& frontPos = recVector[0]->getPosition();
    sf::Vector3f tp(frontPos.x, frontPos.y, 0);
    if(key == left)
    {
      tp.z = (recVector[0]->getDirection()+1)%4;
    }
    else if(key == right)
    {
      tp.z = (recVector[0]->getDirection()-1);
      if(tp.z < 0)
        tp.z = Direction::up;
    }
    recVector[0]->setTurnPoint(tp);
  }

  void addTail()
  {
    auto size = recVector.size();
    if(size > 0)
    {
      block b(*recVector[size-1]);
      recVector.push_back(std::make_shared<block>(b));
      recVector[size-1]->setSuccessor(recVector[size]);
    }
  }

  sf::FloatRect getHeadBounds() const
  {
    return recVector[0]->getGlobalBounds();
  }

  bool gameOverCondition() const
  {
    auto headBounds = getHeadBounds();
    for(int i=1; i < recVector.size(); i++)
    {
      if(headBounds.intersects(recVector[i]->getGlobalBounds()))
        return true;
    }

    return !headBounds.intersects(windowBounds);
  }

  void respawn()
  {
    recVector.clear();
    constStartSnake();
  }

  int getSize() const
  {
    return recVector.size();
  }
};

#endif
