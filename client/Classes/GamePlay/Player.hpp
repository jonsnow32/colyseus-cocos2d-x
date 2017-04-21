//
//  Team.hpp
//  CocosEngine
//
//  Created by Hoang Hung on 4/7/17.
//
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Header.h"

enum class PlayerType
{
    PLAYER_NONE = 0,
    
    PLAYER_RED,
    PLAYER_BLUE,
    PLAYER_GREEN,
    PLAYER_YELLOW,
    
    
};
class Player : public cocos2d::Ref
{
    CC_SYNTHESIZE(std::string, _id, ID);
    CC_PROPERTY(PlayerType, _type, Type);
    CC_SYNTHESIZE(cocos2d::Color3B, _color, Color);
public:
    Player();
};
#endif /* Team_hpp */
