//
//  Overview.h
//  Abyss
//
//  Created by raharu on 2014/05/10.
//
//

#ifndef __Abyss__Overview__
#define __Abyss__Overview__

#include <iostream>
#include "cocos2d.h"
#include "StoryManager.h"

USING_NS_CC;

class Overview : public cocos2d::Layer
{
    Size window_size;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Overview);
    
    StoryManager *storyManager;
};
#endif /* defined(__Abyss__Overview__) */
