//
//  Room.cpp
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#include "Room.hpp"

#include <sstream>
#include <iostream>
using namespace cocos2d;

#define MAX_LEN_STATE_STR 4096

Room::Room (Client *client,const std::string& name)
{
    _client = client;
    _name = name;
    _id = -1;
    _state = new DeltaContainer(nullptr);
    _previousState = NULL;
    _previousStateSize = 0;
}
Room::~Room()
{
    if (_previousState)
        delete _previousState;
}
void Room::setState(msgpack::object state, int remoteCurrentTime, int remoteElapsedTime)
{
    this->_state->Set(state);
    msgpack::sbuffer temp_sbuffer;
    msgpack::packer<msgpack::sbuffer> packer(&temp_sbuffer);
    packer.pack(state);
    
    if (_previousState)
        delete _previousState;
    _previousState = NULL;
    
    _previousStateSize = temp_sbuffer.size();
    _previousState = new char[temp_sbuffer.size()];
    memcpy(_previousState, temp_sbuffer.data(), temp_sbuffer.size());
    RoomUpdateEventArgs *args = new RoomUpdateEventArgs (this, _state->data, "");
    args->autorelease();
    
    if(_onSetRoomState)
        this->_onSetRoomState(this, args);
}
void Room::leave(bool requestLeave)
{
    if (requestLeave && this->_id > 0) {
        this->_client->send ((int)Protocol::LEAVE_ROOM,this->_id);
    } else {
        log("MAY BE WAITING FOR JOIN RESPONSE");
        if(_onLeave)
            _onLeave(this,nullptr);
    }
}

void Room::receiveData (NetworkData* data)
{
    if(_onData)
        this->_onData(this, new MessageEventArgs (this, data));
}

#include "fossilize.c"
void Room::applyPatch (const char* delta ,int len)
{
    char * temp = new char[MAX_LEN_STATE_STR];
    _previousStateSize = delta_apply(_previousState,_previousStateSize, delta, len, temp);

    CCASSERT(_previousStateSize < MAX_LEN_STATE_STR,"MAX_LEN_STATE_STR not enought!!");
    
    delete [] _previousState;
    _previousState = NULL;
    _previousState = new char[_previousStateSize];
    memcpy(_previousState, temp, _previousStateSize);
    delete [] temp;
    
    msgpack::object_handle oh = msgpack::unpack(_previousState, _previousStateSize);
    this->_state->Set(oh.get());

//    if(_onUpdate )
//        _onUpdate(this, new RoomUpdateEventArgs(this, this->_state->data, nullptr));
}

void Room::emitError (MessageEventArgs *args)
{
    this->_onError (this, args);
}
