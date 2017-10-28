//
//  SnakeEngine.hpp
//  Snake
//
//  Created by Kevin on 2017/3/24.
//  Copyright © 2017年 Kevin. All rights reserved.
//

#ifndef SnakeEngine_hpp
#define SnakeEngine_hpp

#include <stdio.h>
#include <vector>
#include "Snake.hpp"

class SnakePlane {
public:
    SnakePlane(int w, int h);
    std::shared_ptr<Snake> NewSnake();
    bool CollisionDetect(std::shared_ptr<Snake> snake);
    Position food;
    
private:
    int width;
    int height;
    
    bool SnakeCollideWithPoint(std::shared_ptr<const Snake> snake, const Position& p, bool himself);
    bool GenerateFood(std::shared_ptr<const Snake> snake);
};

#endif /* SnakeEngine_hpp */
