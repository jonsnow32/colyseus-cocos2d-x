//
//  Planet.hpp
//  CocosEngine
//
//  Created by Hoang Hung on 4/7/17.
//
//

#ifndef Planet_hpp
#define Planet_hpp

#include <stdio.h>
#include "Header.h"
#include "Player.hpp"

class Satellite;
class Planet : public cocos2d::Layer
{
public:
    enum PLANET_STATE
    {
        FREE = 0,
        BUILDING,
        SPWAN
    };
public:
    virtual ~Planet();
    CREATE_FUNC(Planet);
    bool init() override;
    static Planet* createWithSize(int maxSatellite,float spawnSpeed,Player* player);
    bool initWithSize(int maxSatellite,float spawnSpeed,Player* player);
    int attack(Planet* des);
    void onDamge(Satellite *satellite);
    void setPlayer(Player* player);
    void spawn(float dt);
    void onFocus(bool focus);
    CC_SYNTHESIZE(int, _maxSatellite, MaxSatellite);
    CC_SYNTHESIZE(std::string, _id, ID);
    CC_SYNTHESIZE(float, _spawnSpeed, SpawnSpeed);
    CC_SYNTHESIZE(Player*, _owner , Owner);
    CC_PROPERTY(int, _state , State);
    CC_PROPERTY(int,_nSatelite,nSatelite);
    void reset();
private:
    cocos2d::DrawNode*   _graphics;
    cocos2d::DrawNode*   _focus;
    cocos2d::Label * _info;
};
#endif /* Planet_hpp */
