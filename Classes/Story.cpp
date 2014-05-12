//
//  Story.cpp
//  Abyss
//
//  Created by raharu on 2014/05/11.
//
//

#include "Story.h"

static Story *s_Shared = nullptr;

//getInstance
Story* Story::getInstance()
{
    s_Shared = new Story();
    
    if (s_Shared && s_Shared->init()) {
        s_Shared->retain();
        return s_Shared;
    }else{
        CC_SAFE_DELETE(s_Shared);
        return NULL;
    }
}

// init
bool Story::init()
{
    return true;
}