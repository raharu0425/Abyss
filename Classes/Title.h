#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "sqlite3.h"
#include <HttpRequest.h>
#include <HttpClient.h>
#include "TitleConditionManager.h"

USING_NS_CC;
using namespace network;

class Title : public cocos2d::Layer
{
    UserDefault* userDefault;
    TitleConditionManager* sceneCondition;
    Value max_id;
    Value default_max_id;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Title);
    
    //Properties
    cocos2d::Size window_size;
    cocos2d::LabelTTF* status;
    
    //DBの存在フラグ
    bool isExistDB();
    void createDB();
    
    //sqlite3
    sqlite3* useDataBase = NULL;
    char* errorMessage = NULL;
    
    //タイトル作成
    void crateTitle();
    
    //コールバック
    void onHttpRequestCompleted(HttpClient* sender, HttpResponse* response);
    void onStoryMaxCheck(HttpClient* sender, HttpResponse* response);
    void onStoryIds(HttpClient* sender, HttpResponse* response);
    void onStoryImport(HttpClient* sender, HttpResponse* response);
    
    //story_idの取得
    int getDefaultStroyId();
    
    //DBの存在チェック
    void dbCheck();
    
    //ストーリIDの読み込み
    void loadStoryMax();
    
    //足りないIDを補完
    void loadStoryIds();
    
    //ストーリーの読み込み
    void loadStory(std::string);
    
    
    //フレーム処理
    void update(float delta);
};

#endif // __TITLE_SCENE_H__
