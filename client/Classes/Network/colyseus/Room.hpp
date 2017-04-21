//
//  Room.hpp
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#ifndef Room_hpp
#define Room_hpp

#include <stdio.h>
#include "MessageEventArgs.hpp"
#include "Protocol.hpp"
#include "Client.hpp"
#include "DeltaContainer.h"

typedef std::function<void(cocos2d::Ref*,cocos2d::Ref*)> RoomEventHandle;

class Room : public cocos2d::Ref
{
public:
    virtual ~Room();
    Room (Client *client,const std::string& name);
    void setState(msgpack::object state, int remoteCurrentTime, int remoteElapsedTime);
    void leave(bool requestLeave = true);
    void receiveData (NetworkData* data);
    void applyPatch (const char* bytes, int len);
    void emitError (MessageEventArgs *args);
private:
    Client *_client;
    std::string _name;
    CC_SYNTHESIZE(DeltaContainer *, _state, State);
    CC_SYNTHESIZE(int, _id, ID);
    CC_SYNTHESIZE(char *, _previousState, PreviousState);
    CC_SYNTHESIZE(int, _previousStateSize, PreviousStateSize);
public:
    RoomEventHandle _onJoinRoom;
    RoomEventHandle _onError;
    RoomEventHandle _onLeave;
    RoomEventHandle _onPatch;
    RoomEventHandle _onData;
    RoomEventHandle _onSetRoomState;
    template <typename T>
    inline void sendData (T data)
    {
        this->_client->send((int)Protocol::ROOM_DATA,this->_id,data);
    }

};
#endif /* Room_hpp */
