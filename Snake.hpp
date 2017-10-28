//
//  Snake.hpp
//  Snake
//
//  Created by Kevin on 2017/3/24.
//  Copyright © 2017年 Kevin. All rights reserved.
//

#ifndef Snake_hpp
#define Snake_hpp

#include <stdio.h>
#include <deque>
#include <math.h>
#include <stdlib.h>
#include <iostream>

enum Direction {
    SUP,
    SDOWN,
    SRIGHT,
    SLEFT
};

class Position {
public:
    Position() = default;
    Position(int a, int b): x(a), y(b) {}
    int x;
    int y;
};

bool operator==(const Position&, const Position&);
bool operator!=(const Position&, const Position&);

class Snake {

private:
    int width;
    int height;
    Direction HeadDirection;
    std::deque<Position> joints;
    bool ShouldStreth;
    
    Direction head_direction();
    Direction tail_direction();
    Direction direction(const decltype(joints.cbegin()) h,
                        const decltype(joints.cbegin()) s);
    
public:
    Snake(int w, int h);
    Direction Turn(Direction d);
    const Position& Forward();
    void Stretch();
    void print()
    {
        std::for_each(joints.cbegin(), joints.cend(),
                    [](decltype(*joints.cbegin()) it) { std::cout << it.x << " " << it.y << std::endl; } );
    }
    
    std::pair<decltype(joints.cbegin()), decltype(joints.cend())> get_joints() const
    {
        return std::make_pair(joints.cbegin(), joints.cend());
    }
};

#endif /* Snake_hpp */
