//
//  StoryView.cpp
//  Abyss
//
//  Created by raharu on 2014/05/18.
//
//

#include "StoryView.h"

Scene* StoryView::createScene()
{
    auto scene = Scene::create();
    auto layer = StoryView::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool StoryView::init()
{
    //初期化
    if ( !Layer::init() ) return false;
    
    return true;
}