//
//  MessageEventArgs.hpp
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#ifndef MessageEventArgs_hpp
#define MessageEventArgs_hpp

#include <stdio.h>
#include "NetworkEventDefine.h"

class Room;

class MessageEventArgs : public cocos2d::Ref
{
public:
    Room *_room;
    NetworkData* _data;
    MessageEventArgs (Room *room, NetworkData* data = NULL);
    ~MessageEventArgs();
};

class RoomUpdateEventArgs : public cocos2d::Ref
{
public:
    Room *_room;
    msgpack::object_handle *_state;
    std::string _patches;
    RoomUpdateEventArgs (Room* room, msgpack::object_handle * state, std::string patches);
    ~RoomUpdateEventArgs();
};

#endif /* MessageEventArgs_hpp */
