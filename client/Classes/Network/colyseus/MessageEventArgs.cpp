//
//  MessageEventArgs.cpp
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#include "MessageEventArgs.hpp"
#include "Room.hpp"
MessageEventArgs::MessageEventArgs (Room *room, NetworkData* data)
{
    this->_room = room;
    this->_data = data;
}
MessageEventArgs::~MessageEventArgs()
{
    CC_SAFE_RELEASE_NULL(_room);
    CC_SAFE_RELEASE_NULL(_data);
}

RoomUpdateEventArgs::RoomUpdateEventArgs (Room* room, msgpack::object_handle * state, std::string patches)
{
    this->_room = room;
    this->_state = state;
    this->_patches = patches;
}
RoomUpdateEventArgs::~RoomUpdateEventArgs()
{
    //CC_SAFE_RELEASE_NULL(_room);
    //CC_SAFE_DELETE(_state);
}
