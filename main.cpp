#include <SFML/Graphics.hpp>

#include "global.h"
#include "snake.h"
#include "food.h"

sf::Text createLabel(std::string, sf::Color, unsigned int);

int main()
{
  sf::RenderWindow window(sf::VideoMode(1280, 960), "Snake");
  windowWidth = window.getSize().x;
  windowHeight = window.getSize().y;
  Snake snake(sf::Color(255, 0, 0), sf::Vector2f(windowWidth-50, windowHeight/2));
  food fo;
  bool gameOver;

  sf::Text scoreLabel = createLabel(std::to_string(snake.getSize()), sf::Color::Red, 50);
  scoreLabel.setPosition(10, 10);
  sf::Text gameOverLabel = createLabel("Game Over", sf::Color::Red, 100);
  gameOverLabel.setPosition(windowWidth/2, windowHeight/2);
  auto gb = gameOverLabel.getGlobalBounds();
  gameOverLabel.move(-gb.width/2, -gb.height/2);

  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
        window.close();
      if(event.type == sf::Event::KeyPressed)
      {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
          snake.keyPressed(Direction::left);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
          snake.keyPressed(Direction::right);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameOver){
          snake.respawn();
          scoreLabel.setString(std::to_string(snake.getSize()));
          gameOver = false;
        }
      }
      if(event.type == sf::Event::Resized)
      {
        windowWidth = window.getSize().x;
        windowHeight = window.getSize().y;
      }
    }

    window.clear();
    snake.draw(window);
    window.draw(fo);
    window.draw(scoreLabel);
    if(!gameOver)
    {
      snake.update();
      if(snake.gameOverCondition()){
        gameOver = true;
      }
      if(fo.gotEaten(snake.getHeadBounds())){
        snake.addTail();
        fo.respawn();
        scoreLabel.setString(std::to_string(snake.getSize()));
      }
    }
    else{
      window.draw(gameOverLabel);
    }
    window.display();
  }
  return 0;
}

sf::Text createLabel(std::string text, sf::Color color, unsigned int characterSize)
{
  static sf::Font font;
  font.loadFromFile("gameOver.ttf");
  sf::Text label(text, font);
  label.setFillColor(color);
  label.setCharacterSize(characterSize);
  return label;
}
