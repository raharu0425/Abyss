#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "PlatformManager.h"

class Title : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    Size window_size;
    
    //タイトル作成
    void crateTitle();
    
    
    CREATE_FUNC(Title);
};

#endif // __TITLE_SCENE_H__
