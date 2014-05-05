#include "Title.h"

//コンストラクタ
Title::Title()
{
    if (websocket) websocket->close();
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
    
    //タイトルの読み込み
    this->crateTitle();
    
    //外部DBとの接続
    this->loadStory();
    
    
    return true;
}

//ストーリーのロード
void Title::loadStory()
{
    
    // ステータスラベル作成
    
    status = LabelTTF::create("オープン中...", "Arial", 15);
    status->setPosition(Point(window_size.width / 2 , window_size.height - 20));
    this->addChild(status);
    
    // WebSocket生成
    websocket = new WebSocket();
    websocket->init(*this, "ws://raharu.net:8080/echo");
}

void Title::onOpen(WebSocket* ws)
{
    status->setString("オープンしました");
    scheduleOnce(schedule_selector(Title::sendMessage), 1.0f);
}

void Title::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    String *text = String::createWithFormat("response msg: %s", data.bytes);
    status->setString(text->getCString());
}

void Title::onClose(WebSocket* ws)
{
    websocket = nullptr;
    status->setString("クローズしました");
}

void Title::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    char buf[100] = {0};
    sprintf(buf, "エラーが発生しました code: %d", error);
    status->setString(buf);
}

void Title::sendMessage(float dt)
{
    if (websocket->getReadyState() == WebSocket::State::OPEN){
        status->setString("メッセージを送信しました");
        websocket->send("Hello WebSocket,\nI'm a text message.");
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


