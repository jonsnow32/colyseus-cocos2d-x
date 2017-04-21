//
//  Planet.cpp
//  CocosEngine
//
//  Created by Hoang Hung on 4/7/17.
//
//

#include "Planet.hpp"
#include "Satellite.hpp"
#include "SceneBattle.hpp"

Planet::~Planet()
{
    CC_SAFE_DELETE(_owner);
}

bool Planet::init()
{
    if(!Layer::init())
        return false;
    
    _maxSatellite = random(30, 100);
    _spawnSpeed = (int)10/_maxSatellite; //10%
    _nSatelite = 0;
    
    _info = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 20);
    addChild(_info);
    
    _graphics =  cocos2d::DrawNode::create();
    this->addChild(_graphics);
    
    auto player = new Player();
    setPlayer(player);
    schedule(schedule_selector(Planet::spawn), _spawnSpeed);

    return true;
}

Planet* Planet::createWithSize(int maxSatellite,float spawnSpeed,Player* player)
{
    Planet * pRet = new Planet();
    if(pRet && pRet->initWithSize(maxSatellite, spawnSpeed, player))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool Planet::initWithSize(int maxSatellite,float spawnSpeed,Player* player)
{
    if(!Layer::init())
        return false;
    
    _maxSatellite = maxSatellite;
    _spawnSpeed = spawnSpeed; //10%
    _nSatelite = 0;
    
    _info = Label::createWithTTF("0","fonts/Marker Felt.ttf", 20);
    addChild(_info);
    
    _graphics =  cocos2d::DrawNode::create();
    this->addChild(_graphics);
    
    setPlayer(player);
    schedule(schedule_selector(Planet::spawn), _spawnSpeed);
    return true;
}


void Planet::spawn(float dt)
{
#ifdef OFFLINE_VERSION
    if(_state == PLANET_STATE::SPWAN)
    {
        if(_nSatelite < _maxSatellite)
        {
            _nSatelite++;
            _info->setString(u_to_string(_nSatelite));
            //_graphics->drawDot(Vec2(0,0),_statellites->size(),Color4F(_team->getColor()));
        }
    }
#endif
}

void Planet::setnSatelite(int var)
{
    _nSatelite = var;
    _info->setString(u_to_string(_nSatelite));
}
int Planet::getnSatelite() const
{
    return _nSatelite;
}
void Planet::onFocus(bool focus)
{
    
}
int Planet::attack(Planet* des)
{
    int temp = _nSatelite - 1;
    _nSatelite = 1;
    SceneBattle::getInstance()->depart(this, des);
    return temp;
}
void Planet::onDamge(Satellite *satellite)
{
    if(this->getOwner() == nullptr)
        setPlayer(satellite->getPlanet()->getOwner());

    if(satellite->getPlanet() && satellite->getPlanet()->getOwner()->getType() == this->getOwner()->getType())
    {
        _nSatelite++;
    }
    if(satellite->getPlanet() == nullptr)
    {
        
    }
    else
    {
        _nSatelite--;
        if(_nSatelite <= 0)
        {
            setPlayer(satellite->getPlanet()->getOwner());
        }
    }
    _info->setString(u_to_string(_nSatelite));
    
    SceneBattle::getInstance()->attack(satellite->getPlanet(),satellite->getPlanetTarget(),1);
    //_graphics->drawDot(Vec2(0,0),_statellites->size(),Color4F(_team->getColor()));
}

void Planet::setPlayer(Player* player)
{
    _owner = player;
    Color4F color((_owner && _state == SPWAN) ? _owner->getColor() : Color3B::GRAY);
    _graphics->clear();
    _graphics->drawCircle(Vec2(0,0), _maxSatellite, 0, 100, false, 1.0, 1.0, color);
    _info->setColor(_owner ? _owner->getColor() : Color3B::GRAY);
    
    
}

void Planet::setState(int state)
{
    _state = state;
    switch (_state) {
        case PLANET_STATE::FREE:
            
            break;
        case PLANET_STATE::BUILDING:
            
            break;
        case PLANET_STATE::SPWAN:
        {
            Color4F color((_owner && _state == SPWAN) ? _owner->getColor() : Color3B::GRAY);
            _graphics->clear();
            _graphics->drawCircle(Vec2(0,0), _maxSatellite, 0, 100, false, 1.0, 1.0, color);

            break;
        }
        default:
            break;
    }
}
int Planet::getState() const
{
    return _state;
}
void Planet::reset()
{
    _owner = nullptr;
    _info->setColor(Color3B::GRAY);
    _nSatelite = 0;
    _graphics->clear();
    _graphics->drawCircle(Vec2(0,0), _maxSatellite, 0, 100, false, 1.0, 1.0, Color4F::GRAY);
    _info->setString(u_to_string(_nSatelite));
}
