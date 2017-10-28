//
//  SnakeEngine.cpp
//  Snake
//
//  Created by Kevin on 2017/3/24.
//  Copyright © 2017年 Kevin. All rights reserved.
//

#include "SnakePlane.hpp"
#include <vector>

using namespace std;

SnakePlane::SnakePlane(int w, int h)
: width(w), height(h)
{}

std::shared_ptr<Snake>
SnakePlane::NewSnake()
{
    auto snake = make_shared<Snake>(width, height);
    GenerateFood(snake);
    return snake;
}

bool
SnakePlane::CollisionDetect(std::shared_ptr<Snake> snake) {
    
    auto ites = snake->get_joints();
    auto head = ites.first;
    
    if ( head->x < 0 || head->x >= width || head->y < 0 || head->y >= height )
        return true;
    
    if (SnakeCollideWithPoint(snake, *head, true))
        return true;
    else {
        if ((*head) == food) {
            snake->Stretch();
            GenerateFood(snake);
        }
    }
    return false;
}

bool
SnakePlane::SnakeCollideWithPoint(std::shared_ptr<const Snake> snake, const Position& p, bool himself)
{
    auto ites = snake->get_joints();
    auto head = ites.first;
    auto tail = ites.second;
    
    /* head will never hit neck(space between the first and the second joints), 
     * but it always hit neck since it is the first value of neck,
     * so we need escape from neck when testing if snake hit himself or not.
     */
    
    if (himself)
        head++;
    
    for (auto i = head, j = i + 1; j < tail; j++, i++) {
        if ( i->x == j->x ) {
            if ( p.x == i->x )
                if ( p.y >= min(j->y, i->y) && p.y <= max(j->y, i->y) )
                    return true;
        } else if ( i->y == j->y ) {
                if ( p.y == i->y )
                    if ( p.x >= min(j->x, i->x) && p.x <= max(j->x, i->x) )
                        return true;
        }
    }
    return false;
}

bool
SnakePlane::GenerateFood(std::shared_ptr<const Snake> snake)
{
// FIXME: this bug needs to be fixed : block forever if snake filled plant
    do {
        food.x = static_cast<int>(arc4random() % width);
        food.y = static_cast<int>(arc4random() % height);
    } while (SnakeCollideWithPoint(snake, food, false));
    return true;
}
