//
//  Client.hpp
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include "Header.h"

class Room;
using namespace cocos2d::network;

typedef std::function<void(int)> ClientCallback;

class Client : public WebSocket::Delegate
{
public:
    enum RESPONSE_TYPE
    {
        ON_OPEN = 0,
        ON_MESSAGE,
        ON_CLOSE,
        ON_ERROR,
        ON_INIT_FAIL,
    };
private:
    virtual void onOpen(WebSocket* ws)override;
    virtual void onMessage(WebSocket* ws, const WebSocket::Data& data)override;
    virtual void onClose(WebSocket* ws)override;
    virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error)override;
    ClientCallback _clientCallback;
public:
    static Client* getInstance();
    void start(const std::string& serverUrl,ClientCallback callback);
    virtual ~Client();
    Room* join(const std::string& roomName, cocos2d::Ref* options);
    
    void recvUserHandle(msgpack::object_array data);
    void joinRoomHandle(msgpack::object_array data);
    void joinRoomErrorDRoomHandle(msgpack::object_array data);
    void leaveRoomHandle(msgpack::object_array data);
    void roomPatchStateHandle(msgpack::object_array data);
    void roomStateHandle(msgpack::object_array data);
    void roomDataHandle(msgpack::object_array data);
    void badRequestHandle(msgpack::object_array data);
    
    void close();
    std::string getError();
private:
    static Client* _instance;
    Client();
    std::string _id;
    bool parseMsg(const char *data, int len);
    WebSocket* _ws;
    std::map<const std::string,Room*> _rooms;
public:
    Room* getRoomByName(const std::string& name);
    Room* getRoomByID(int ID);
    std::string getClientID() { return _id; }
    template <typename... Args>
    inline void send(Args... args)
    {
        msgpack::sbuffer buffer;
        msgpack::packer<msgpack::sbuffer> pk(&buffer);
        msgpack::type::make_define_array(args...).msgpack_pack(pk);
        _ws->send((unsigned char *)buffer.data(),buffer.size());
    }
    inline void send(unsigned char *buffer,unsigned int size)
    {
        _ws->send(buffer,size);
    }

};
#endif /* Client_hpp */
