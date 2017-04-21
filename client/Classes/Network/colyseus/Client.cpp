//
//  Client.cpp
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#include "Client.hpp"
#include "msgpack.hpp"
#include "Room.hpp"
#include "Protocol.hpp"

#include <sstream>
#include <iostream>

using namespace cocos2d;
using namespace cocos2d::network;
Client* Client::_instance = nullptr;
Client* Client::getInstance()
{
    if(_instance == nullptr)
        _instance = new Client();
    
    return _instance;
}
void Client::start(const std::string& serverUrl,ClientCallback callback)
{
    _clientCallback = callback;
    _ws = new WebSocket();
    if(!_ws->init(*this, serverUrl))
    {
        CC_SAFE_DELETE(_ws);
        _clientCallback(RESPONSE_TYPE::ON_INIT_FAIL);
    }
}
Client::Client()
{
    _clientCallback = nullptr;
}

Client::~Client()
{
    CC_SAFE_DELETE(_ws);
}

bool Client::parseMsg(const char *data, int len)
{
    msgpack::object_handle oh = msgpack::unpack(data,len);  
    msgpack::object obj = oh.get();
    
    std::cout << "-------------------------RAW---------------------------------" << std::endl;
    std::cout << obj  << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    
    Protocol protocol = (Protocol)obj.via.array.ptr[0].via.i64;
    msgpack::object_array message(obj.via.array);
    
    switch (protocol) {
        case Protocol::USER_ID:
            log("Protocol::USER_ID");
            recvUserHandle(message);
            break;
        case Protocol::JOIN_ROOM:
            log("Protocol::JOIN_ROOM");
            joinRoomHandle(message);
            break;
        case Protocol::JOIN_ERROR:
            log("Protocol::JOIN_ERROR");
            joinRoomErrorDRoomHandle(message);
            break;
        case Protocol::LEAVE_ROOM:
            log("Protocol::LEAVE_ROOM");
            leaveRoomHandle(message);
            break;
        case Protocol::ROOM_DATA:
            log("Protocol::ROOM_DATA");
            roomDataHandle(message);
            break;
        case Protocol::ROOM_STATE:
            log("Protocol::ROOM_STATE");
            roomStateHandle(message);
            break;
        case Protocol::ROOM_STATE_PATCH:
            log("Protocol::ROOM_STATE_PATCH");
            roomPatchStateHandle(message);
            break;
        case Protocol::BAD_REQUEST:
            log("Protocol::BAD_REQUEST");
            badRequestHandle(message);
            break;
            break;
            
        default:
            break;
    }

    return true;
}
void Client::recvUserHandle(msgpack::object_array data)
{
    data.ptr[1].convert(_id);
}
void Client::joinRoomHandle(msgpack::object_array data)
{
    int64_t roomID = data.ptr[1].via.i64;
    std::string roomName;
    data.ptr[2].convert(roomName);
    std::map<const std::string,Room*>::iterator it = this->_rooms.find(roomName);
    if(it != _rooms.end())
    {
        log("JOIN ROOM SUCCESSFUL roomid = %i , roomName = %s",(int)roomID,roomName.c_str());
        (*it).second->setID(roomID);
    }
	else
    {
        log("PLEASE CALL Client::join(), maybe error belong to Server");
    }
}
void Client::joinRoomErrorDRoomHandle(msgpack::object_array data)
{
    std::string roomName;
    data.ptr[2].convert(roomName);
    std::map<const std::string,Room*>::iterator it = this->_rooms.find(roomName);
    if(it != _rooms.end())
    {
        (*it).second->emitError(new MessageEventArgs((*it).second,nullptr));
        _rooms.erase(it);
    }
    
}
void Client::leaveRoomHandle(msgpack::object_array data)
{
    int64_t roomID = data.ptr[1].via.i64;
    std::map<const std::string,Room*>::iterator it = this->_rooms.begin();
    while (it != _rooms.end())
    {
        if(it->second->getID() == roomID)
            return;
    }
}

void Client::roomPatchStateHandle(msgpack::object_array data)
{
    int64_t roomID = data.ptr[1].via.i64;
    
    Room *room = getRoomByID((int)roomID);
    msgpack::object_array patchBytes = data.ptr[2].via.array;

    char * patches = new char[patchBytes.size];
    for(int idx = 0; idx < patchBytes.size ; idx++)
    {
        patches[idx] = patchBytes.ptr[idx].via.i64;
    }
    
    room->applyPatch(patches,patchBytes.size);
    
    delete [] patches;
}
void Client::roomDataHandle(msgpack::object_array data)
{
    int64_t roomID = data.ptr[1].via.i64;
    Room *room = getRoomByID((int)roomID);
    
    msgpack::object roomData;
    data.ptr[2].convert(roomData);
    NetworkData *networkData = new NetworkData(&roomData);
    
    room->receiveData(networkData);
}
void Client::roomStateHandle(msgpack::object_array data)
{
    msgpack::object state;
    data.ptr[2].convert(state);
    int64_t remoteCurrentTime = data.ptr[3].via.i64;
    int64_t remoteElapsedTime = data.ptr[4].via.i64;

    int64_t roomID = data.ptr[1].via.i64;
    Room * room = getRoomByID((int)roomID);
    room->setState(state, (int)remoteCurrentTime, (int)remoteElapsedTime);
}
void Client::badRequestHandle(msgpack::object_array data)
{
    
}
Room* Client::join(const std::string& roomName, cocos2d::Ref* options)
{
    if (this->_rooms.find(roomName) == _rooms.end())
    {
        std::pair<const std::string, Room*> roomPair(roomName,new Room(this, roomName));
        this->_rooms.insert(roomPair);
    }
    this->send((int)Protocol::JOIN_ROOM, roomName);
    return this->_rooms.find(roomName)->second; 
}


Room* Client::getRoomByName(const std::string& name)
{
    std::map<const std::string,Room*>::iterator it = this->_rooms.find(name);
    if (it != _rooms.end())
    {
        return it->second;
    }
    return nullptr;
}

Room* Client::getRoomByID(int ID)
{
    std::map<const std::string,Room*>::iterator it = this->_rooms.begin();
    while (it != _rooms.end())
    {
        if(it->second->getID() == ID)
            return it->second;
        it++;
    }
    return nullptr;
}

void Client::close()
{
    this->_ws->close();
}
std::string Client::getError()
{
    CCASSERT(0, "NOT SUPPORT YET");
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate methods
void Client::onOpen(network::WebSocket* ws)
{
    log("Websocket (%p) opened", ws);
    _clientCallback(RESPONSE_TYPE::ON_OPEN);
}

void Client::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)
{
    parseMsg(data.bytes,data.len);
    _clientCallback(RESPONSE_TYPE::ON_MESSAGE);
}

void Client::onClose(network::WebSocket* ws)
{
    log("websocket instance (%p) closed.", ws);
    CC_SAFE_DELETE(ws);
    _clientCallback(RESPONSE_TYPE::ON_CLOSE);
}

void Client::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)
{
    log("Error was fired, error code: %d", error);
    _clientCallback(RESPONSE_TYPE::ON_ERROR);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
