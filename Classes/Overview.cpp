//
//  Overview.cpp
//  Abyss
//
//  Created by raharu on 2014/05/10.
//
//

#include "Overview.h"

Scene* Overview::createScene()
{
    auto scene = Scene::create();
    auto layer = Overview::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool Overview::init()
{
    //初期化
    if ( !Layer::init() ) return false;
    
    //窓サイズを取得
    window_size = Director::getInstance()->getWinSize();
    
    //奈落の底と出す
    auto title_1 = LabelTTF::create("新しい画面", "Arial Rounded MT Bold", 50);
    title_1->setPosition(window_size.width / 2, window_size.height / 100 * 80);
    this->addChild(title_1, 1);
    
    return true;
}