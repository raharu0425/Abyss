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
    
    //画面サイズサイズを取得
    window_size = Director::getInstance()->getWinSize();
    
    //userDefaultの読み込み
    userDefault = UserDefault::getInstance();
    
    //ストーリー
    storyManager = StoryManager::getInstance();
    
    //ストーリーの読み込み
    loadStory();
    
    //Scrollview
    auto *scroll = ScrollView::create(Size(window_size.width, window_size.height * 0.8));
    scroll->setDirection(ScrollView::Direction::BOTH);
    addChild(scroll);
    
    //Label
    auto label = LabelTTF::create(story->getText(), "Arial Rounded MT Bold", 15);
    
    label->setDimensions(Size(window_size.width,0));
    label->setDimensions(Size(label->getContentSize().width, label->getContentSize().height));
    label->setHorizontalAlignment(TextHAlignment::LEFT);
    
    scroll->setContainer(label);
    scroll->setContentOffset(Point(0, 0 - (label->getContentSize().height - window_size.height)));
    
    
    //ボタン
    Scale9Sprite* sprite = Scale9Sprite::create("button.png");
    auto button = CCLabelTTF::create("戻る", "Arial", 15);
    button->setColor(Color3B(0,0,0));
    ControlButton* pCCButton =  ControlButton::create(button,sprite);
    pCCButton->setPosition(Point(window_size.width * 0.9, window_size.height * 0.9));
    pCCButton->setPreferredSize(Size(50, 30));
    addChild(pCCButton);
    
    pCCButton->addTargetWithActionForControlEvents(this, cccontrol_selector(StoryView::touchDownAction), Control::EventType::TOUCH_DOWN);
    
    return true;
}

//ストーリーの読み込み
void StoryView::loadStory()
{
    auto story_id = userDefault->getIntegerForKey("view_story_id");
    CCLOG("ssss:%d", story_id);
    story = storyManager->getByid(story_id, 1);
}

//ボタンが押されたとき
void StoryView::touchDownAction(CCObject *senderz, Control::EventType controlEvent)
{
    Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Overview::createScene()));
    
}