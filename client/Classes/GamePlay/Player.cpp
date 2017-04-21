//
//  Team.cpp
//  CocosEngine
//
//  Created by Hoang Hung on 4/7/17.
//
//

#include "Player.hpp"

Player::Player()
{
    _type = PlayerType::PLAYER_NONE;
    _id = "";
}

PlayerType Player::getType() const
{
    return _type;
}
void Player::setType(PlayerType var)
{
    _type = var;
    switch (var) {
        case PlayerType::PLAYER_RED:
            _color = cocos2d::Color3B::RED;
            break;
        case PlayerType::PLAYER_BLUE:
            _color = cocos2d::Color3B::BLUE;
            break;
        case PlayerType::PLAYER_GREEN:
            _color = cocos2d::Color3B::GREEN;
            break;
        case PlayerType::PLAYER_YELLOW:
            _color = cocos2d::Color3B::YELLOW;
            break;
        case PlayerType::PLAYER_NONE:
            _color = cocos2d::Color3B::GRAY;
            break;
            
        default:
            break;
    }
}
