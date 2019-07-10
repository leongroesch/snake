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
  Snake snake2(sf::Color(0, 0, 255), sf::Vector2f(50, windowHeight/2));
  food fo;
  bool gameOver;

  sf::Text scoreLabel1 = createLabel(std::to_string(snake.getSize()), sf::Color::Red, 50);
  scoreLabel1.setPosition(windowWidth-100, 10);
  sf::Text scoreLabel2 = createLabel(std::to_string(snake.getSize()), sf::Color::Blue, 50);
  scoreLabel2.setPosition(10, 10);

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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
          snake2.keyPressed(Direction::left);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
          snake2.keyPressed(Direction::right);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameOver){
          snake.respawn();
          snake2.respawn();
          scoreLabel1.setString(std::to_string(snake.getSize()));
          scoreLabel2.setString(std::to_string(snake2.getSize()));
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
    snake2.draw(window);
    window.draw(fo);
    window.draw(scoreLabel1);
    window.draw(scoreLabel2);
    if(!gameOver)
    {
      snake.update();
      snake2.update();
      if(snake.gameOverCondition(snake2.getHeadBounds())
        || snake2.gameOverCondition(snake.getHeadBounds())){
        gameOver = true;
      }
      if(fo.gotEaten(snake.getHeadBounds())){
        snake.addTail();
        fo.respawn();
        scoreLabel1.setString(std::to_string(snake.getSize()));
      }
      else if(fo.gotEaten(snake2.getHeadBounds())){
        snake2.addTail();
        fo.respawn();
        scoreLabel2.setString(std::to_string(snake2.getSize()));
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
