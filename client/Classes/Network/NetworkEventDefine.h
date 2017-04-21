//
//  NetworkEventDefine.h
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#ifndef NetworkEventDefine_h
#define NetworkEventDefine_h

#include "Header.h"

class NetworkData : public cocos2d::Ref
{
public:
    NetworkData(msgpack::object * data): _data(data)
    {
        
    }
    ~NetworkData()
    {
        delete _data;
    }
    msgpack::object * _data;
};

#endif /* NetworkEventDefine_h */
