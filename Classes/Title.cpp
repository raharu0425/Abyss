#include "Title.h"
#include <spine/Json.h>
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
    scheduleUpdate();
    
    //窓サイズを取得
    window_size = Director::getInstance()->getWinSize();
    
    //userDefaultの読み込み
    userDefault = UserDefault::getInstance();
    
    //タイトルの読み込み
    this->crateTitle();
    
    //ラベルの作成
    status = LabelTTF::create("アップロードシーケンスを開始...", "Arial", 15);
    status->setPosition(Point(window_size.width / 2 , window_size.height / 100 * 10));
    this->addChild(status);
    
    //DBチェックの開始
    sceneCondition->setCondition(TitleConditionManager::kConditionProcessing);
    sceneCondition->setProcessing(TitleConditionManager::kProcessDbCheck);

    return true;
}

//フレーム処理
void Title::update(float delta)
{
    if(sceneCondition->isConditionIdle()){
        CCLOG("idle");
    }else if (sceneCondition->isConditionLoading()){
        CCLOG("loading");
        if(sceneCondition->isLoadStoryIdMax()){
            CCLOG("loadStoryMax");
            this->loadStoryMax();
        }else if (sceneCondition->isLoadStoryIds()){
            CCLOG("loadStoryIds");
            this->loadStoryIds();
        }
    }else if (sceneCondition->isConditionProcessing()){
        CCLOG("process");
        if(sceneCondition->isProcessDbCheck()){
            CCLOG("dbCheck");
            this->dbCheck();
        }else if (sceneCondition->isProcessStoryIdCheck()){
            CCLOG("StoryIdCheck");
            //this->stroyIdCheck();
        }
    }
}

//DBの存在チェック
void Title::dbCheck()
{
    status->setString("DB構成の確認...");
    
    //DBが作成できなかったら
    CCASSERT(this->isExistDB(), "database not founds");
    
    //DB作成
    this->createDB();
    
    //StroyIDをロード
    sceneCondition->setCondition(TitleConditionManager::kConditionLoading);
    sceneCondition->setLoading(TitleConditionManager::kLoadStoryIdMax);
}

//MAXIDのロード
void Title::loadStoryMax()
{
    //アイドル状態
    sceneCondition->setCondition(TitleConditionManager::kConditionIdle);
    
    //状態変更
    status->setString("サーバー通信中...");
    
    //HTTP通信
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    HttpRequest *request = new HttpRequest();
    
    //現在のDB上の最大IDを取得
    request->setUrl("http://raharu.net/naraku/app/get/stroyidmax");
    request->setRequestType(HttpRequest::Type::GET);
    auto ad = &Title::onStoryMaxCheck;
    request->setResponseCallback(this, SEL_HttpResponse(ad));
    request->setHeaders(headers);
    HttpClient::getInstance()->send(request);
    request->release();
}

//IDのコールバック関数の取得
void Title::onStoryMaxCheck(HttpClient* sender, HttpResponse* response)
{
    //状態変更
    status->setString("チェック中...");
    
    //Exceptiom
    CCASSERT(response, "connecting failed");
    CCASSERT(response->isSucceed(), "response failed");
    
    picojson::value v;
    std::string err;
    
    //バッファの取得
    std::vector<char>* buffer = response->getResponseData();
    CCLOG("buffsize:%lu", buffer->size());
    char * json = (char *) malloc(buffer->size() + 1);
    std::string s2(buffer->begin(), buffer->end());
    strcpy(json, s2.c_str());
    picojson::parse(v, json, json + strlen(json), &err);
    
    //エラー
    CCASSERT(err.empty(), err.c_str());
  
    //MAXIDを取得
    picojson::object& all = v.get<picojson::object>();
    max_id = Value(all["max_id"].get<std::string>());
    default_max_id= Value(userDefault->getIntegerForKey("story_max", 0));
    
    //怖い話のID配列を取得
    if(max_id.asInt() > default_max_id.asInt()){
        sceneCondition->setCondition(TitleConditionManager::kConditionLoading);
        sceneCondition->setLoading(TitleConditionManager::kLoadStoryIds);
    }
}

void Title::loadStoryIds()
{
    //アイドル状態
    sceneCondition->setCondition(TitleConditionManager::kConditionIdle);
    
    //状態変更
    status->setString("サーバー通信中...");
    
    //HTTP通信
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    HttpRequest *request = new HttpRequest();
    
    //現在のDB上の最大IDを取得
    std::string url = "http://raharu.net/naraku/app/get/storyids?now_default_id=";
    url += default_max_id.asString();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    auto ad = &Title::onStoryIds;
    request->setResponseCallback(this, SEL_HttpResponse(ad));
    request->setHeaders(headers);
    HttpClient::getInstance()->send(request);
    request->release();
}


//IDのコールバック関数の取得
void Title::onStoryIds(HttpClient* sender, HttpResponse* response)
{
    //状態変更
    status->setString("チェック中...");
    
    //Exceptiom
    CCASSERT(response, "connecting failed");
    CCASSERT(response->isSucceed(), "response failed");
    
    picojson::value v;
    std::string err;
    
    //バッファの取得
    std::vector<char>* buffer = response->getResponseData();
    CCLOG("buffsize:%lu", buffer->size());
    char * json = (char *) malloc(buffer->size() + 1);
    std::string s2(buffer->begin(), buffer->end());
    strcpy(json, s2.c_str());
    picojson::parse(v, json, json + strlen(json), &err);
    
    //エラー
    CCASSERT(err.empty(), err.c_str());
    
    //MAXIDを取得
    picojson::array& all = v.get<picojson::array>();
    
    //一個ずつ持ってくるべさ
    for (picojson::array::iterator it = all.begin(); it != all.end(); it++) {
        status->setString("インポート中...");
        this->loadStory(it->get<std::string>());
    }
    
}

//ストーリーの読み込み
void Title::loadStory(std::string story_id)
{
    //状態変更
    status->setString("取得中...");
    
    //HTTP通信
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    HttpRequest *request = new HttpRequest();
    
    //現在のDB上の最大IDを取得
    std::string url = "http://raharu.net/naraku/app/get/story?story_id=";
    url += story_id;
    
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    auto ad = &Title::onStoryImport;
    request->setResponseCallback(this, SEL_HttpResponse(ad));
    request->setHeaders(headers);
    HttpClient::getInstance()->send(request);
    request->release();
}

//ストーリーのインポート
void Title::onStoryImport(HttpClient* sender, HttpResponse* response)
{
    //Exceptiom
    CCASSERT(response, "connecting failed");
    CCASSERT(response->isSucceed(), "response failed");
    
    picojson::value v;
    std::string err;
    
    //バッファの取得
    std::vector<char>* buffer = response->getResponseData();
    CCLOG("buffsize:%lu", buffer->size());
    char * json = (char *) malloc(buffer->size() + 1);
    std::string s2(buffer->begin(), buffer->end());
    strcpy(json, s2.c_str());
    picojson::parse(v, json, json + strlen(json), &err);
    
    //エラー
    CCASSERT(err.empty(), err.c_str());
    
    //MAXIDを取得
    picojson::object& record = v.get<picojson::object>();
    auto title = Value(record["title"].get<std::string>());
    auto story_id = Value(record["id"].get<std::string>());
    
    //ステータス
    std::string msg = "インポート...";
    msg += title.asString();
    status->setString(msg);
    
    if(max_id.asInt() == story_id.asInt()){
        status->setString("インポート完了");
    }
}

//HTTPリクエストの戻り値
void Title::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response)
{
    if (!response) return;
    
    // Check the HTTP Status Code
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    CCLOG("response code: %d", statusCode);
    
    // A connection failure
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    std::vector<char>* buffer = response->getResponseData();
    CCLOG("buffsize:%lu", buffer->size());
    char * json = (char *) malloc(buffer->size() + 1);
    std::string s2(buffer->begin(), buffer->end());
    strcpy(json, s2.c_str());
    
    picojson::value v;
    
    std::string err;
    picojson::parse(v, json, json + strlen(json), &err);
    
    if(err.empty()){
        picojson::object& all = v.get<picojson::object>();
        std::string title = all["title"].get<std::string>();
        CCLOG("name:%s", title.c_str());
    } else {
        CCLOG("err:%s", err.c_str());
    }
}


//必要なDBの作成
void Title::createDB()
{
    //DB作成中
    //Story作成
    status->setString("DB作成中...story");
    auto create_sql = "CREATE TABLE IF NOT EXISTS `story` (`id` INTEGER PRIMARY KEY AUTOINCREMENT,`hash` varchar(128)  NOT NULL ,`title` varchar(256) NOT NULL,`text` text NOT NULL,`length` varchar(16) NOT NULL,`type` varchar(16)  NOT NULL ,`number` int(2) NOT NULL,`max_number` int(2) NOT NULL DEFAULT '1',`report_id` int(10) DEFAULT NULL ,`is_release` int(2) NOT NULL DEFAULT '0' ,`is_selection` int(2) NOT NULL DEFAULT '0',`created_at` int(11) NOT NULL DEFAULT '0',`updated_at` int(11) NOT NULL DEFAULT '0',`deleted_at` int(11) NOT NULL DEFAULT '0' ,`active` int(2) NOT NULL DEFAULT '1')";
    
    auto sts = sqlite3_exec(useDataBase, create_sql, NULL, NULL, &errorMessage );
    if( sts != SQLITE_OK ) CCLOG("create table failed : %s", errorMessage);
    status->setString("DB作成中...storyOK");
    
    //Story_Vars作成
    status->setString("DB作成中...story_vars");
    auto story_vars = "CREATE TABLE IF NOT EXISTS `story_words` (`id` INTEGER PRIMARY KEY AUTOINCREMENT,`story_id` int(10) NOT NULL ,`story_hash` varchar(64) NOT NULL,`word` varchar(32) NOT NULL,`push_count` int(5) NOT NULL DEFAULT '0',`last_ip` varchar(32) DEFAULT NULL,`is_enable` int(2) NOT NULL DEFAULT '0',`created_at` int(11) NOT NULL DEFAULT '0',`updated_at` int(11) NOT NULL DEFAULT '0',`deleted_at` int(11) NOT NULL DEFAULT '0',`active` int(2) NOT NULL DEFAULT '1')";
    auto sts2 = sqlite3_exec(useDataBase, story_vars, NULL, NULL, &errorMessage );
    if( sts2 != SQLITE_OK ) CCLOG("create table failed : %s", errorMessage);
    
    auto story_vars_index_1 = "CREATE INDEX `story_id` ON story_words(`story_id`)";
    auto sts3 = sqlite3_exec(useDataBase, story_vars_index_1, NULL, NULL, &errorMessage );
    if( sts3 != SQLITE_OK ) CCLOG("create index failed : %s", errorMessage);
    
    auto story_vars_index_2 = "CREATE INDEX `tag` ON story_words(`word`)";
    auto sts4 = sqlite3_exec(useDataBase, story_vars_index_2, NULL, NULL, &errorMessage );
    if( sts4 != SQLITE_OK ) CCLOG("create index failed : %s", errorMessage);
    
    auto story_vars_index_3 = "CREATE INDEX `story_hash` ON story_words(`story_hash`)";
    auto sts5 = sqlite3_exec(useDataBase, story_vars_index_3, NULL, NULL, &errorMessage );
    if( sts5 != SQLITE_OK ) CCLOG("create index failed : %s", errorMessage);
    
    status->setString("DB作成中...story_varsOK");
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

//DBが存在しているのか
bool Title::isExistDB()
{
    //DBファイルの保存先のパス
    auto filePath = FileUtils::getInstance()->getWritablePath();
    filePath.append("Abyss.db");
    
    //Open
    auto status = sqlite3_open(filePath.c_str(), &useDataBase);
    
    //ステータスが0以外の場合はエラーを表示
    if (status != SQLITE_OK) return false;
    
    return true;
}


