//
//  StoryView.h
//  Abyss
//
//  Created by raharu on 2014/05/18.
//
//

#ifndef __Abyss__StoryView__
#define __Abyss__StoryView__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class StoryView : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(StoryView);
};

#endif /* defined(__Abyss__StoryView__) */
