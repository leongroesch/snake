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
  sf::Keyboard::Key leftKey, rightKey;
  std::unique_ptr<sf::Text> scoreLabel;

  static int globalId;
  int id;

  void initScoreLabel()
  {
    static sf::Font font;
    font.loadFromFile("gameOver.ttf");
    scoreLabel.reset(new sf::Text(std::to_string(recVector.size()), font, 50));
    scoreLabel->setFillColor(color);
    scoreLabel->setPosition(startPosition.x, 20);
  }
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

  void changeDirection(Direction key)
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

public:
  Snake(sf::Color _color, sf::Vector2f _startPosition, sf::Keyboard::Key _leftKey, sf::Keyboard::Key _rightKey)
    : windowBounds(0, 0, windowWidth, windowHeight), color(_color), startPosition(_startPosition),
      leftKey(_leftKey), rightKey(_rightKey), id(globalId)
  {
    globalId++;
    initScoreLabel();
    constStartSnake();
  }

  void draw(sf::RenderWindow& window)
  {
    for(auto const& x : recVector)
      window.draw(*x);
    window.draw(*scoreLabel);
  }

  void update()
  {
    for(auto & x : recVector)
      x->update();
  }

  void addTail()
  {
    auto size = recVector.size();
    if(size > 0)
    {
      block b(*recVector[size-1]);
      recVector.push_back(std::make_shared<block>(b));
      recVector[size-1]->setSuccessor(recVector[size]);
      //Update Score
      scoreLabel->setString(std::to_string(recVector.size()));
    }
  }

  sf::FloatRect getHeadBounds() const
  {
    return recVector[0]->getGlobalBounds();
  }

  bool gameOverCondition(sf::FloatRect const& otherHead) const
  {
    auto headBounds = getHeadBounds();

    if(headBounds.intersects(otherHead))
      return true;
    for(int i=1; i < recVector.size(); i++)
    {
      if(headBounds.intersects(recVector[i]->getGlobalBounds())
        ||otherHead.intersects(recVector[i]->getGlobalBounds()))
        return true;
    }

    return !headBounds.intersects(windowBounds);
  }

  void respawn()
  {
    recVector.clear();
    constStartSnake();
  }

  void keyPressed()
  {
    if(sf::Keyboard::isKeyPressed(leftKey))
      changeDirection(Direction::left);
    if(sf::Keyboard::isKeyPressed(rightKey))
      changeDirection(Direction::right);
  }

  int getSize() const
  {
    return recVector.size();
  }

  int getId() const
  {
    return id;
  }
};

int Snake::globalId{0};

bool operator !=(Snake const& left, Snake const& right)
{
  return left.getId() != right.getId();
}

#endif
