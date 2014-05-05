#include "Title.h"

USING_NS_CC;

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
    
    //タイトルの読み込み
    this->crateTitle();
    
    //外部DBとの接続
    //this->loadDataBase();
    
    return true;
}


//タイトルの作成
void Title::crateTitle()
{
    //窓サイズを取得
    window_size = Director::getInstance()->getWinSize();
    
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
        auto subtitle = LabelTTF::create("for iOS", "Arial Rounded MT Bold", 25);
    #endif
    
    //サブタイトル
    subtitle->setPosition(window_size.width / 2, window_size.height / 100 * 20);
    this->addChild(subtitle, 1);

    
}


