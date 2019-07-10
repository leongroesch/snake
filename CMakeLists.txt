cmake_minimum_required(VERSION 2.6)

project(Snake)

add_executable(snake global.h main.cpp game.h block.h snake.h food.h)
target_link_libraries(snake sfml-graphics sfml-window sfml-system sfml-audio)
