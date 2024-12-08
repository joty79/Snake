#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <cmath>
#include <SDL.h>

class Apple {
public:
    int x;
    int y;
    int count;
    
    Apple() : x(0), y(0), count(0) {}
};

class Snake {
public:
    int x;
    int y;
    int dir;
    std::shared_ptr<Snake> next;
    
    Snake(int x = 0, int y = 0, int dir = 0) : 
        x(x), y(y), dir(dir), next(nullptr) {}
};

enum Direction {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_LEFT,
    SNAKE_RIGHT
};
