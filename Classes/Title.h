#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include <HttpRequest.h>
#include <HttpClient.h>
#include "TitleConditionManager.h"
#include "HTTPManager.h"
#include "StoryManager.h"

USING_NS_CC;
using namespace network;

class Title : public cocos2d::Layer
{
    
    // dependecies
    UserDefault* userDefault;
    TitleConditionManager* sceneCondition;
    HTTPManager *httpManager;
    StoryManager* storyManager;
    
    // properties
    Value default_max_id;
    Value max_id;
    Size window_size;
    LabelTTF* status;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Title);
    
    //タイトル作成
    void crateTitle();
    
    //DBの存在チェック
    void dbCheck();
    
    //HTTPLoad
    void loadStoryMax();
    void loadStoryIds();
    void loadStory(std::string);
    
    //コールバック
    void onStoryMaxCheck(HttpClient* sender, HttpResponse* response);
    void onStoryIds(HttpClient* sender, HttpResponse* response);
    void onStoryImport(HttpClient* sender, HttpResponse* response);
    
    //フレーム処理
    void update(float delta);
};

#endif // __TITLE_SCENE_H__
