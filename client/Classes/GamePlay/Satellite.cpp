//
//  Satellite.cpp
//  CocosEngine
//
//  Created by Hoang Hung on 4/7/17.
//
//

#include "Satellite.hpp"
#include "Planet.hpp"

using namespace cocos2d;

Satellite::Satellite() :
_planet(0) ,
_planetTarget(0)
{
    //_graphics = DrawNode::create();
    //this->addChild(_graphics);
//    auto sprite = Sprite::createWithSpriteFrameName(Resources::Plist::ChineseChess_Frames_Name::cc_piece_prev_position);
//    addChild(sprite);
//    
}

bool Satellite::init()
{
    initWithFile("dot.png");
    scheduleUpdate();
    return true;
}

Planet* Satellite::getPlanet() const
{
    return _planet;
}

void Satellite::setPlanet(Planet *var)
{
    _planet = var;
    //_graphics->drawDot(Vec2(0,0), 3, Color4F(var->getTeam_()->getColor()));
}

void Satellite::update(float dt)
{
    if(_status == STATUS::MOVING && _planetTarget != nullptr)
    {
        Vec2 pos = _planetTarget->getPosition();
        if(pos.distance(this->getPosition()) < _planetTarget->getMaxSatellite())
        {
            if(_isCollistion)
                _planetTarget->onDamge(this);
            this->removeFromParent();
        }
    }
}
