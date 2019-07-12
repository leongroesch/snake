#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <vector>

#include "global.h"
#include "snake.h"
#include "food.h"

class game
{
private:
  bool gameOver;
  std::vector<food> meals;
  std::vector<Snake> snakes;
  sf::RenderWindow window;
  std::unique_ptr<sf::Text> gameOverLabel;
  sf::SoundBuffer eatBuffer;
  sf::Sound eatSound;
  sf::SoundBuffer overBuffer;
  sf::Sound overSound;

  void initGameOverLabel()
  {
    static sf::Font font;
    font.loadFromFile("gameOver.ttf");
    gameOverLabel.reset( new sf::Text("Game Over", font, (unsigned)100));
    gameOverLabel->setFillColor(sf::Color::Red);
    gameOverLabel->setPosition(windowWidth/2, windowHeight/2);
    auto gb = gameOverLabel->getGlobalBounds();
    gameOverLabel->move(-gb.width/2, -gb.height/2);
  }
  void eventLoop()
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
        window.close();
      if(event.type == sf::Event::KeyPressed)
      {
        for(auto& snake : snakes){
          snake.keyPressed();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          window.close();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameOver){
          for(auto& snake : snakes)
            snake.respawn();
          gameOver = false;
        }
      }
      if(event.type == sf::Event::Resized)
      {
        windowWidth = window.getSize().x;
        windowHeight = window.getSize().y;
      }
    }
  }

  void updateMeals()
  {
    for(auto& food : meals)
    {
      for(auto& snake : snakes)
      {
        if(food.gotEaten(snake.getHeadBounds())){
          snake.addTail();
          eatSound.play();
          food.respawn();
        }
      }
    }
  }

  void drawMeals()
  {
    for(auto& food : meals)
      window.draw(food);
  }

  void updateSnakes()
  {
    for(auto& snake : snakes)
    {
      snake.update();
      if(snake.gameOverCondition())
      {
        gameOver = true;
        overSound.play();
      }
      for(auto& other : snakes)
      {
        if(snake != other && snake.gameOverCondition(other.getHeadBounds()))
        {
          gameOver = true;
          overSound.play();
        }
      }
    }
  }

  void drawSnakes()
  {
    for(auto& snake : snakes)
      snake.draw(window);
  }

public:
  game(int snakeCount, int foodCount)
    : window(sf::VideoMode::getFullscreenModes()[0], "Snake", sf::Style::Fullscreen), gameOver(false)
  {
      window.setMouseCursorVisible(false);
      windowWidth = window.getSize().x;
      windowHeight = window.getSize().y;
      initGameOverLabel();
      if(!eatBuffer.loadFromFile("sound.wav"))
        throw "sound.wav could not be loaded";
      eatSound.setBuffer(eatBuffer);
      if(!overBuffer.loadFromFile("error.wav"))
        throw "error.wav could not be loaded";
      overSound.setBuffer(overBuffer);

      if(snakeCount == 2)
      {
        snakes.push_back(Snake(sf::Color(0, 0, 255), sf::Vector2f(windowWidth/2 -100, windowHeight/2), sf::Keyboard::A, sf::Keyboard::D));
        snakes.push_back(Snake(sf::Color(255, 0, 0), sf::Vector2f(windowWidth/2 +100, windowHeight/2), sf::Keyboard::Left, sf::Keyboard::Right));
      }
      else
        snakes.push_back(Snake(sf::Color(255, 0, 0), sf::Vector2f(windowWidth/2, windowHeight/2), sf::Keyboard::Left, sf::Keyboard::Right));
      for(int i=0; i < foodCount; i++)
        meals.push_back(food());
  }

  void update()
  {
    eventLoop();
    window.clear();
    drawMeals();
    drawSnakes();
    if(!gameOver)
    {
      updateMeals();
      updateSnakes();
    }
    else{
      window.draw(*gameOverLabel);
    }
    window.display();
  }

  bool toContinue()
  {
    return window.isOpen();
  }

};
