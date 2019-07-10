#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>

#include "global.h"

class block : public sf::RectangleShape{
private:
  int direction;
  std::queue<sf::Vector3f> turnPoints;
  std::shared_ptr<block> successor;
  sf::Color color;

  bool checkTP(sf::Vector3f const& turnPoint)
  {
    auto const& pos = getPosition();
    switch(direction)
    {
      case Direction::left:
        return pos.x < turnPoint.x;
      case Direction::right:
        return pos.x > turnPoint.x;
      case Direction::up:
        return pos.y < turnPoint.y;
      case Direction::down:
        return pos.y > turnPoint.y;
    }
  }

public:
  block(int _dir, sf::Color _color) : RectangleShape(sf::Vector2f(10, 10)),
     direction(_dir%4), color(_color)
  {
    setFillColor(color);
  };

  block(block& prev) : RectangleShape(sf::Vector2f(10,10)),
    direction(prev.direction), color(prev.color)
  {
    float x = prev.getPosition().x;
    float y = prev.getPosition().y;
    switch(direction)
    {
      case Direction::left:
        x += prev.getSize().x +1; break;
      case Direction::right:
        x -= getSize().x +1; break;
      case Direction::up:
        y += prev.getSize().y+1; break;
      case Direction::down:
        y -= getSize().y +1; break;
    }
    setPosition(x, y);
    setFillColor(color);
  }

  void update()
  {
    if(!turnPoints.empty() && checkTP(turnPoints.front()))
    {
      auto tp = turnPoints.front();
      turnPoints.pop();
      direction = static_cast<int>(tp.z);
      setPosition(tp.x, tp.y);
      if(successor)
      {
        successor->setTurnPoint(tp);
      }
    }

    switch(direction)
    {
      case Direction::left:
        move(-velocity, 0); break;
      case Direction::right:
        move(velocity, 0); break;
      case Direction::up:
        move(0, -velocity); break;
      case Direction::down:
        move(0, velocity); break;
    }
  }

  void setSuccessor(std::shared_ptr<block> _successor)
  {
    successor = _successor;
  }

  void setTurnPoint(sf::Vector3f tp)
  {
    turnPoints.push(std::move(tp));
  }

  void increaseDirection(int value)
  {
    direction += value;
    direction = direction%4;
    if(direction == -1)
      direction = Direction::up;
  }

  int getDirection() const
  {
    return direction;
  }

};

#endif
