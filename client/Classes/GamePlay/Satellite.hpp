//
//  Satellite.hpp
//  CocosEngine
//
//  Created by Hoang Hung on 4/7/17.
//
//

#ifndef Satellite_hpp
#define Satellite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Header.h"
class Planet;

class Satellite : public cocos2d::Sprite
{
public:
    enum class STATUS
    {
        IDLE = 0,
        MOVING,
        DIE,
    };
public:
    Satellite();
    CREATE_FUNC(Satellite);
    bool init();
    CC_PROPERTY(Planet*, _planet, Planet);
    CC_SYNTHESIZE(Planet*, _planetTarget, PlanetTarget);
    CC_SYNTHESIZE(STATUS, _status, Status);
    CC_SYNTHESIZE(bool, _isCollistion, IsCollistion);

    cocos2d::DrawNode*   _graphics;
    void update(float dt);
    
};
#endif /* Satellite_hpp */
