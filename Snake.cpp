//
//  Snake.cpp
//  Snake
//
//  Created by Kevin on 2017/3/24.
//  Copyright © 2017年 Kevin. All rights reserved.
//

#include "Snake.hpp"

using namespace std;

Snake::Snake(int w, int h)
:width(w), height(h), ShouldStreth(false)
{
    Position head, tail;
    
    /*
     * x0.2 + randx0.6 so that snake won't born in edge
     */
    
    head.x = static_cast<int>(width * 0.2 + arc4random() % width * 0.6);
    head.y = static_cast<int>(height * 0.2 + arc4random() % height * 0.6);
    
    /* it's a snake not a dot */
    tail.x = head.x + 2;
    tail.y = head.y;
    
    joints.push_front(head);
    joints.push_back(tail);
    
    HeadDirection = head_direction();
};

const Position&
Snake::Forward()
{
    auto &head = joints.front();
    
    switch (HeadDirection) {
        case SUP:
            head.y++;
            break;
        case SDOWN:
            head.y--;
            break;
        case SRIGHT:
            head.x++;
            break;
        case SLEFT:
            head.x--;
            break;
        default:
            break;
    }
    
    auto taildirection = tail_direction();
    
    /*
     * snake got some food and would stretch, alternatively, this time won't
     * move its tail.
     */
    
    if ( !ShouldStreth ) {
        auto &tail = joints.back();
        
        switch (taildirection) {
            case SUP:
                tail.y++;
                break;
            case SDOWN:
                tail.y--;
                break;
            case SLEFT:
                tail.x--;
                break;
            case SRIGHT:
                tail.x++;
                break;
            default:
                break;
        }
    } else ShouldStreth = false;
    
    /* remove redundent tail since we may pushed tail */
    tail_direction();
    
    return joints.front();
}

void
Snake::Stretch()
{
    ShouldStreth = true;
}

Direction
Snake::Turn(Direction d)
{
    
    /* 
     * if snake is moving up/down then only turning left/right is allowed,
     * vice versa.
     */
    
    if ( HeadDirection == SUP || HeadDirection == SDOWN ) {
        if ( d == SRIGHT || d == SLEFT ) {
            HeadDirection = d;
            joints.push_front(Position(joints.front()));
        }
    } else if ( HeadDirection == SRIGHT || HeadDirection == SLEFT ) {
        if ( d == SUP || d == SDOWN ) {
            HeadDirection = d;
            joints.push_front(Position(joints.front()));
        }
    }
    return HeadDirection;
}


Direction
Snake::tail_direction()
{
    if ( joints.size() < 2 )
        throw runtime_error("Snake Must Have At Least 2 Joints");
    
    /* 
     * remove redundent tail joint and
     * call tail_direction() recursively
     * to determinate tail direction.
     */
    
    if ( *(joints.end() - 1) == *(joints.end() - 2) ) {
        joints.pop_back();
        return tail_direction();
    }
    
    auto d = direction(joints.end() - 1, joints.end() - 2);
    if ( d == SUP )
        return SDOWN;
    else if ( d == SDOWN )
        return SUP;
    else if ( d == SRIGHT )
        return SLEFT;
    else return SRIGHT;
}

Direction
Snake::head_direction()
{
    if ( joints.size() < 2 )
        throw runtime_error("Snake Must Have At Least 2 Joints");
    
    return direction(joints.begin(), joints.begin() + 1);
}

Direction
Snake::direction(const decltype(joints.cbegin()) h,
                 const decltype(joints.cbegin()) s)
{
    
    if ( h->x == s->x ) {
        if ( h->y > s-> y)
            return SUP;
        else return SDOWN;
    } else if ( h->y == s->y ) {
        if ( h->x > s->x )
            return SRIGHT;
        else return SLEFT;
    }
    
    /* this snake must be stray */
    return SUP;
}

bool
operator==(const Position& p1, const Position& p2)
{
    if ( p1.x == p2.x && p1.y == p2.y )
        return true;
    return false;
}

bool
operator!=(const Position& p1, const Position& p2)
{
    return !(p1 == p2);
}

