//
//  SceneBattle.cpp
//  CocosEngine
//
//  Created by Hoang Hung on 4/7/17.
//
//

#include "SceneBattle.hpp"
#include "Planet.hpp"
#include "Player.hpp"
#include "Satellite.hpp"
#include "Network/colyseus/Client.hpp"
#include "Network/colyseus/Room.hpp"

SceneBattle::~SceneBattle()
{
    _gameRoom->leave(true);
    _instance = nullptr;
}

SceneBattle * SceneBattle::_instance = nullptr;
SceneBattle* SceneBattle::getInstance()
{
    return _instance;
};



bool SceneBattle::init()
{
	if (!Scene::init()) return false;

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SceneBattle::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SceneBattle::onTouchEnd, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(SceneBattle::onTouchMove, this);
    
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	Client::getInstance()->start("ws://13.76.179.224:2667", CC_CALLBACK_1(SceneBattle::onConnectToServer, this));

	_instance = this;
	return true;
}


void SceneBattle::onConnectToServer(int response)
{
	switch (response) {
	case Client::RESPONSE_TYPE::ON_OPEN:
		regirsterServerListener();
		break;
	case Client::RESPONSE_TYPE::ON_CLOSE:
	case Client::RESPONSE_TYPE::ON_INIT_FAIL:
	case Client::RESPONSE_TYPE::ON_ERROR:
		//this->stopAllTask();
		//SceneManager::getInstance()->replaceScene(SceneBattle::create()); //cheat
		break;

	default:
		break;
	}
}


bool SceneBattle::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    std::string id = Client::getInstance()->getClientID();
    Vec2 pos = touch->getLocation();
    for(int i = 0 ; i < _planets.size() ; i++)
    {
        if(_planets[i]->getOwner() && _planets[i]->getOwner()->getID() == id &&  _planets[i]->getPosition().distance(pos) < _planets[i]->getMaxSatellite())
        {
            _planetsSelected.push_back(_planets[i]);
            _planets[i]->onFocus(true);
        }
    }
    return true;
}

void SceneBattle::onTouchMove(cocos2d::Touch* touch, cocos2d::Event* event)
{
    onTouchBegan(touch,event);
}

void SceneBattle::moveSatellites(Planet* src, Planet *target,bool isCollistion)
{
    if(src->getID() == target->getID())
        return;
    
    int n_satellite = 0;
    if(isCollistion)
        n_satellite = src->attack(target);
    else
        n_satellite = src->getnSatelite();
    
    while(n_satellite > 0)
    {
        Satellite* obj = Satellite::create();
        this->addChild(obj);
        obj->setPosition(src->getPosition());
        obj->setPlanetTarget(target);
        obj->setPlanet(src);
        obj->setIsCollistion(isCollistion);
        float duration = random(1.0f,5.0f);
        ccBezierConfig c;
        c.endPosition = target->getPosition();
        c.controlPoint_1 = Vec2(0,0);
        c.controlPoint_2 = Vec2(200,300);
        BezierTo* bezier = BezierTo::create(duration, c);
        
        //MoveTo *moveTo = MoveTo::create(duration, target->getPosition());
        obj->setStatus(Satellite::STATUS::MOVING);
        obj->setVisible(true);
        obj->runAction(bezier);
        n_satellite--;
    }
}

void SceneBattle::onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event)
{
    Planet* target = nullptr;
    Vec2 pos = touch->getLocation();

    for(int i = 0 ; i < _planets.size() ; i++)
    {
         if(_planets[i]->getPosition().distance(pos) < _planets[i]->getMaxSatellite())
         {
             target = _planets[i];
             break;
         }
        
    }
    for(int i = 0 ; i < _planetsSelected.size() ; i++)
    {
        if(target)
            moveSatellites(_planetsSelected[i],target);
        _planetsSelected[i]->onFocus(false);
    }
    _planetsSelected.clear();
}


PlayerType SceneBattle::getPlayerWin()
{
    PlayerType winer = _planets[0]->getOwner()->getType();
    for(auto it = _planets.begin() ; it != _planets.end() ; it++)
    {
        if(winer != (*it)->getOwner()->getType())
            return PlayerType::PLAYER_NONE;
    }
    return winer;
}

void SceneBattle::addPlayer(Player* player)
{
    std::pair<std::string,Player*> pair(player->getID(),player);
    _players.insert(pair);
}
void SceneBattle::removePlayer(std::string playerID)
{
    
}

Planet* SceneBattle::getPlanetByID(std::string id)
{
    for(int i = 0 ; i < _planets.size() ; i++)
    {
        if(_planets[i]->getID() == id)
            return _planets[i];
    }
}
void SceneBattle::regirsterServerListener()
{
    _gameRoom = Client::getInstance()->join("gameRoom", nullptr);
    _gameRoom->_onSetRoomState = CC_CALLBACK_2(SceneBattle::onGameRoomInit,this);
    _gameRoom->_onData = CC_CALLBACK_2(SceneBattle::onRoomDataUpdate,this);
    _gameRoom->_onError = CC_CALLBACK_2(SceneBattle::onRoomError,this);
    
    DeltaContainer* gameRoomState = _gameRoom->getState();
    gameRoomState->Listen("world/planets/:id/playerID/",    "replace", CC_CALLBACK_2(SceneBattle::onUpdatePlanet,this));
    gameRoomState->Listen("players/:id",    "add", CC_CALLBACK_2(SceneBattle::onAddPlayer,this));
    gameRoomState->Listen("players/:id",    "remove", CC_CALLBACK_2(SceneBattle::onPlayerRemoved,this));
    gameRoomState->Listen("world/planets/:id/nSatellite/",    "replace", CC_CALLBACK_2(SceneBattle::onSatelliteUpdate,this));
    gameRoomState->Listen("world/planets/:id/currentState/",    "replace", CC_CALLBACK_2(SceneBattle::onPlanetChangeState,this));
}

float getNumber(msgpack::object object)
{
    switch (object.type) {
        case msgpack::type::object_type::NEGATIVE_INTEGER:
        case msgpack::type::object_type::POSITIVE_INTEGER:
            return object.via.i64;
        case msgpack::type::object_type::FLOAT32:
        case msgpack::type::object_type::FLOAT64:
            return object.via.f64;
            break;
            
        default:
            break;
    }
    assert(0);
    return -1;
}
void SceneBattle::onGameRoomInit(cocos2d::Ref* sender,cocos2d::Ref* args)
{
    auto winSize = Director::getInstance()->getVisibleSize();
    //Room* room = (Room*)sender;
    RoomUpdateEventArgs *data = (RoomUpdateEventArgs*)args;
    msgpack::object state = data->_state->get();
    
    msgpack::object_map players = state.via.map.ptr[1].val.via.map;
    for(int i = 0 ; i < players.size ; i++)
    {
        std::string id;
        players.ptr[i].val.via.map.ptr[0].val.convert(id);
        PlayerType type  = (PlayerType)players.ptr[i].val.via.map.ptr[1].val.via.i64;
        Player *player = new Player();
        player->setID(id);
        player->setType(type);
        std::pair<std::string,Player*> pair(id,player);
        _players.insert(pair);
    }
    
    msgpack::object_map planets = state.via.map.ptr[0].val.via.map.ptr[1].val.via.map;
    
    float width = getNumber(state.via.map.ptr[0].val.via.map.ptr[2].val);
    float height = getNumber(state.via.map.ptr[0].val.via.map.ptr[3].val);
    
    float scaleX = winSize.width/width;
    float scaleY = winSize.height/height;
    
    for(int i = 0 ; i < planets.size ; i++)
    {
        msgpack::object_map obj = planets.ptr[i].val.via.map;
        std::string plannetID;
        planets.ptr[i].key.convert(plannetID);
        std::cout<<"planet ID " << plannetID << std::endl;
        float x = getNumber(obj.ptr[0].val)*scaleX;
        float y = getNumber(obj.ptr[1].val)*scaleY;
        int teamID = obj.ptr[2].val.via.i64;
        msgpack::object planetOfPlayerID = obj.ptr[3].val;
        
        std::string playerId;
        planetOfPlayerID.convert(playerId);
        
        int maxSatellite = getNumber(obj.ptr[4].val);
        float spawnSpeed = getNumber(obj.ptr[5].val);
        int nSatellite = getNumber(obj.ptr[7].val);
        int state = getNumber(obj.ptr[8].val);
        
        Player *player = nullptr;
        auto it = _players.find(playerId);
        if(it != _players.end())
            player = it->second;
        
        Planet* pl = Planet::createWithSize(maxSatellite, spawnSpeed, player);
        pl->setID(plannetID);
        pl->setPosition(x,y);
        pl->setState(state);
        pl->setnSatelite(nSatellite);
        addChild(pl);
        _planets.push_back(pl);
    }
}

void SceneBattle::onAddPlayer(std::vector<std::string> matches ,msgpack::object value)
{
    std::cout<<"----------------------onAddPlayer---------------------"<< std::endl;
    for(int i = 0 ; i < matches.size() ; i++)
        std::cout<<matches[i] << std::endl;
    std::cout<< value << std::endl;
    
    
    std::string playerId = matches[1];

    PlayerType type  = (PlayerType)value.via.map.ptr[1].val.via.i64;
    Player *player = new Player();
    player->setID(playerId);
    player->setType(type);
    std::pair<std::string,Player*> pair(playerId,player);
    _players.insert(pair);
}
void SceneBattle::onPlayerRemoved(std::vector<std::string> matches ,msgpack::object value)
{
    std::cout<<"----------------------onPlayerRemoved---------------------"<< std::endl;
    for(int i = 0 ; i < matches.size() ; i++)
        std::cout<<matches[i] << std::endl;
    std::cout<< value << std::endl;
    
    std::string playerId = matches[1];
    auto it = _players.begin();
    while (it != _players.end()) {
        if(it->second->getID() == playerId)
            it = _players.erase(it);
        else
            it++;
    }
    auto it2 = _planets.begin();
    while(it2 != _planets.end())
    {
        if((*it2)->getOwner() && (*it2)->getOwner()->getID() == playerId)
            (*it2)->reset();
        it2++;
    }
        
}

void SceneBattle::onSatelliteUpdate(std::vector<std::string> matches ,msgpack::object value)
{
    std::cout<<"----------------------onSatelliteUpdate---------------------"<< std::endl;
    for(int i = 0 ; i < matches.size() ; i++)
        std::cout<<matches[i] << std::endl;
    std::cout<< value << std::endl;
    
    std::string planetID = matches[2];
    int nSatellite = value.via.i64;
    
    auto it2 = _planets.begin();
    while(it2 != _planets.end())
    {
        if((*it2)->getID() == planetID)
            (*it2)->setnSatelite(nSatellite);
        it2++;
    }
    
}

void SceneBattle::onPlanetChangeState(std::vector<std::string> matches ,msgpack::object value)
{
    std::cout<<"----------------------onPlanetChangeState---------------------"<< std::endl;
    for(int i = 0 ; i < matches.size() ; i++)
        std::cout<<matches[i] << std::endl;
    std::cout<< value << std::endl;
    
    std::string planetID = matches[2];
    int state = value.via.i64;
    
    auto it2 = _planets.begin();
    while(it2 != _planets.end())
    {
        if((*it2)->getID() == planetID)
            (*it2)->setState(state);
        it2++;
    }
}
void SceneBattle::onUpdatePlanet(std::vector<std::string> matches ,msgpack::object value)
{
    std::cout<<"----------------------onUpdatePlanet---------------------"<< std::endl;
    for(int i = 0 ; i < matches.size() ; i++)
        std::cout<<matches[i] << std::endl;
    std::cout<< value << std::endl;
    
    std::string planetID = matches[2];
    std::string playerID;
    value.convert(playerID);
    
    Player *player = nullptr;
    
    auto itPlayer = _players.find(playerID);
    if(itPlayer != _players.end())
        player = itPlayer->second;
    
    if(player)
    {
        auto itPlanet = _planets.begin();
        while(itPlanet != _planets.end())
        {
            std::cout<< "PLANET ID " << (*itPlanet)->getID() << std::endl;
            if((*itPlanet)->getID() == planetID)
                (*itPlanet)->setPlayer(player);
            itPlanet++;
        }
    }
}

void SceneBattle::onRoomError(cocos2d::Ref* room,cocos2d::Ref* data)
{
    std::cout<<"----------------------onRoomError---------------------"<< std::endl;
}

void SceneBattle::onRoomDataUpdate(cocos2d::Ref* room,cocos2d::Ref* netWorkData)
{
    msgpack::object *data = ((MessageEventArgs*)netWorkData)->_data->_data;
    int command = data->via.array.ptr[0].via.i64;
    switch (command) {
        case Command::SEND_ATTACK_TO_ONOTHER:
        {
            std::string srcPlanetId;
            data->via.array.ptr[1].convert(srcPlanetId);
            std::string desPlanetId;
            data->via.array.ptr[2].convert(desPlanetId);
            
            moveSatellites(getPlanetByID(srcPlanetId), getPlanetByID(desPlanetId),false);
            break;
        }
        default:
            break;
    }
}


void SceneBattle::depart(Planet* src, Planet *des)
{
    msgpack::type::tuple<int,std::string,std::string,int> message((int)SceneBattle::Command::DEPART,src->getID(), des->getID(),src->getnSatelite());
    _gameRoom->sendData(message);
}
void SceneBattle::attack(Planet* src, Planet *des,int nSatellite)
{
    msgpack::type::tuple<int,std::string,std::string,int> message((int)SceneBattle::Command::ATTACK,src->getID(), des->getID(),nSatellite);
    _gameRoom->sendData(message);
}


