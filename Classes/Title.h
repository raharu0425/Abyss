#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

class Title : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    //サイズ
    cocos2d::Size window_size;
    
    //タイトル作成
    void crateTitle();
    
    //外部サーバーとの接続
    //void loadDataBase();
    
    
    CREATE_FUNC(Title);
};

#endif // __TITLE_SCENE_H__
