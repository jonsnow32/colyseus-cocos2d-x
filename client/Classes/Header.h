//
//  Header.h
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/15/17.
//
//

#ifndef Header_h
#define Header_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "storage/local-storage/LocalStorage.h"
#include "audio/include/SimpleAudioEngine.h"
#include "network/WebSocket.h"
#include "msgpack.hpp"
using namespace cocos2d;

template <typename T> std::string u_to_string(const T& n)
{
	std::ostringstream stm;
	stm << n;
	return stm.str();
}


#endif /* Header_h */
