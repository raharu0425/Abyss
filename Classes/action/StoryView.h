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
#include "Overview.h"
#include "extensions/cocos-ext.h"
#include "StoryManager.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

class StoryView : public Layer
{
    // dependecies
    UserDefault* userDefault;
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(StoryView);
    
    //prperties
    Size window_size;
    StoryManager *storyManager;
    Story *story;
    
    //methods
    void loadStory();
    void touchDownAction(CCObject *senderz, Control::EventType controlEvent);
};

#endif /* defined(__Abyss__StoryView__) */
