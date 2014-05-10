#include "Title.h"
#include "Overview.h"
#include "picojson.h"

using namespace picojson;


Scene* Title::createScene()
{
    auto scene = Scene::create();
    auto layer = Title::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool Title::init()
{
    //初期化
    if ( !Layer::init() ) return false;
    
    //状態管理の初期化
    sceneCondition = TitleConditionManager::getInstance();
    
    //コンポーネント読み込み
    httpManager = HTTPManager::getInstance();
    storyManager = StoryManager::getInstance();
    
    //userDefaultの読み込み
    userDefault = UserDefault::getInstance();
    
    //リセットフラグ
    //userDefault->setIntegerForKey("story_max", 0);
    
    
    //窓サイズを取得
    window_size = Director::getInstance()->getWinSize();
    
    //タイトルの読み込み
    this->crateTitle();
    
    //ステータス表示
    status = LabelTTF::create("初期シーケンスを開始...", "Arial", 15);
    status->setPosition(Point(window_size.width / 2 , window_size.height / 100 * 10));
    this->addChild(status);
    
    //DBチェックの開始
    sceneCondition->setCondition(TitleConditionManager::kConditionProcessing);
    sceneCondition->setProcessing(TitleConditionManager::kProcessDbCheck);
    
    scheduleUpdate();

    return true;
}

//フレーム処理
void Title::update(float delta)
{
    if(sceneCondition->isConditionIdle()){
        //status->setString("処理停止中...");
    }else if (sceneCondition->isConditionLoading()){
        status->setString("サーバー通信中...");
        if(sceneCondition->isLoadStoryIdMax()){
            this->loadStoryMax();
        }else if (sceneCondition->isLoadStoryIds()){
            this->loadStoryIds();
        }
    }else if (sceneCondition->isConditionProcessing()){
        status->setString("処理中...");
        if(sceneCondition->isProcessDbCheck()){
            this->dbCheck();
        }
    }else if(sceneCondition->isConditionCompleted()){
        
    }
}

//シーンの切り替え
void Title::complted(float delta)
{
    Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Overview::createScene()));
}

//DBの存在チェック
void Title::dbCheck()
{
    //初期DBの作成
    storyManager->createInitTables();
    
    //StroyIDをロード
    sceneCondition->setCondition(TitleConditionManager::kConditionLoading);
    sceneCondition->setLoading(TitleConditionManager::kLoadStoryIdMax);
}

//MAXIDのロード
void Title::loadStoryMax()
{
    sceneCondition->setCondition(TitleConditionManager::kConditionIdle);
    
    //StorymaxIDの取得
    auto url = "http://raharu.net/naraku/app/get/stroyidmax";
    httpManager->get(this, SEL_HttpResponse(&Title::onStoryMaxCheck), url);
}

//IDのコールバック関数の取得
void Title::onStoryMaxCheck(HttpClient* sender, HttpResponse* response)
{
    //MAXIDを取得
    auto json_value = httpManager->getJsonByResponseData(response);
    picojson::object& all = json_value.get<picojson::object>();
    max_id = Value(all["max_id"].get<std::string>());
    default_max_id= Value(userDefault->getIntegerForKey("story_max", 0));
    
    //怖い話のID配列を取得
    if(max_id.asInt() != default_max_id.asInt()){
        sceneCondition->setCondition(TitleConditionManager::kConditionLoading);
        sceneCondition->setLoading(TitleConditionManager::kLoadStoryIds);
        return;
    }
    
    //シーン遷移
    status->removeFromParentAndCleanup(true);
    scheduleOnce(schedule_selector(Title::complted), 2.0f);
}

//取得していないIDSの作成
void Title::loadStoryIds()
{
    sceneCondition->setCondition(TitleConditionManager::kConditionIdle);
    
    //StorymaxIDの取得
    std::string url = "http://raharu.net/naraku/app/get/storyids?now_default_id=";
    url += default_max_id.asString();
    httpManager->get(this, SEL_HttpResponse(&Title::onStoryIds), url);
}


//IDのコールバック関数の取得
void Title::onStoryIds(HttpClient* sender, HttpResponse* response)
{
    auto json_value = httpManager->getJsonByResponseData(response);
    picojson::array& all = json_value.get<picojson::array>();
    
    //一個ずつ持ってくるべさ
    for (picojson::array::iterator it = all.begin(); it != all.end(); it++) {
        this->loadStory(it->get<std::string>());
    }
    
}

//ストーリーの読み込み
void Title::loadStory(std::string story_id)
{
    //Storyの取得
    std::string url = "http://raharu.net/naraku/app/get/story?story_id=";
    url += story_id;
    httpManager->get(this, SEL_HttpResponse(&Title::onStoryImport), url);
}

//ストーリーのインポート
void Title::onStoryImport(HttpClient* sender, HttpResponse* response)
{
    auto json_value = httpManager->getJsonByResponseData(response);
    picojson::object& record = json_value.get<picojson::object>();
    auto story_id = Value(record["id"].get<std::string>());
    
    //ステータス
    std::string msg = "インポート...";
    msg += story_id.asString();
    status->setString(msg);
    
    storyManager->addStory(json_value);
    
    //取り込んだ所のIDを保存
    userDefault->setIntegerForKey("story_max", story_id.asInt());
    
    //完了
    if(max_id.asInt() == story_id.asInt()){
        status->removeFromParentAndCleanup(true);
        scheduleOnce(schedule_selector(Title::complted), 2.0f);
    }
}

//タイトルの作成
void Title::crateTitle()
{
    //奈落の底と出す
    auto title_1 = LabelTTF::create("奈", "Arial Rounded MT Bold", 50);
    title_1->setPosition(window_size.width / 2, window_size.height / 100 * 80);
    this->addChild(title_1, 1);
    
    
    auto title_2 = LabelTTF::create("落", "Arial Rounded MT Bold", 50);
    title_2->setPosition(window_size.width / 2, window_size.height / 100 * 65);
    this->addChild(title_2, 1);
    
    
    auto title_3 = LabelTTF::create("の", "Arial Rounded MT Bold", 50);
    title_3->setPosition(window_size.width / 2, window_size.height / 100 * 50);
    this->addChild(title_3, 1);
    
    
    auto title_4 = LabelTTF::create("底", "Arial Rounded MT Bold", 50);
    title_4->setPosition(window_size.width / 2, window_size.height / 100 * 35);
    title_4->setColor(Color3B(255,0,0));
    this->addChild(title_4, 1);
    
    //底をまわす
    auto rotation = RotateBy::create(30, 30 * 108);
    title_4->runAction(rotation);
    
    //iOSとAndroidで分ける
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto subtitle = LabelTTF::create("for iOS", "Arial Rounded MT Bold", 25);
    #else
        auto subtitle = LabelTTF::create("for Android", "Arial Rounded MT Bold", 25);
    #endif
    
    //サブタイトル
    subtitle->setPosition(window_size.width / 2, window_size.height / 100 * 20);
    this->addChild(subtitle, 1);
    
}


