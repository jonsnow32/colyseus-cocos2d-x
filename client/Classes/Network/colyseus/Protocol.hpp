//
//  Protocol.hpp
//  CocosEngine
//
//  Created by Hung Hoang Manh on 3/22/17.
//
//

#ifndef Protocol_hpp
#define Protocol_hpp

#include <stdio.h>
enum class Protocol {
    USER_ID = 1,
    JOIN_ROOM = 10,
    JOIN_ERROR = 11,
    LEAVE_ROOM = 12,
    ROOM_DATA = 13,
    ROOM_STATE = 14,
    ROOM_STATE_PATCH = 15,
    BAD_REQUEST = 50,
};

#endif /* Protocol_hpp */
