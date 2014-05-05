#include "Title.h"

//コンストラクタ
Title::Title()
{
    //if (websocket) websocket->close();
}



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
    if ( !Layer::init() )
    {
        return false;
    }
    
    //窓サイズを取得
    window_size = Director::getInstance()->getWinSize();
    
    //ラベルの作成
    status = LabelTTF::create("接続中...", "Arial", 15);
    status->setPosition(Point(window_size.width / 2 , window_size.height / 100 * 10));
    this->addChild(status);
    
    //タイトルの読み込み
    this->crateTitle();
    
    //内部DBの存在チェック
    status->setString("DB構成...");
    if(this->isExistDB()){
        
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
    
    //外部DBとの接続
    this->loadStory();
    
    
    return true;
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

//ストーリーのロード
void Title::loadStory()
{
    //WebSocket生成
    websocket = new WebSocket();
    websocket->init(*this, "ws://raharu.net:8080/echo");
}

void Title::onOpen(WebSocket* ws)
{
    status->setString("接続に成功しました");
    scheduleOnce(schedule_selector(Title::sendMessage), 1.0f);
}

void Title::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    auto *text = String::createWithFormat("%s", data.bytes);
    status->setString(text->getCString());
}

void Title::onClose(WebSocket* ws)
{
    websocket = nullptr;
    status->setString("Closed connection");
}

void Title::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    char buf[100] = {0};
    sprintf(buf, "Error code: %d", error);
    status->setString(buf);
}

void Title::sendMessage(float dt)
{
    if (websocket->getReadyState() == WebSocket::State::OPEN){
        websocket->send("Loading...\n初期処理には数分かかる事があります\n通信環境の良い環境で行ってください");
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


