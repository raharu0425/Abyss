//
//  Overview.cpp
//  Abyss
//
//  Created by raharu on 2014/05/10.
//
//

#include "Overview.h"

#include <extensions/cocos-ext.h>
USING_NS_CC_EXT;

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
    
    //お話の取得
    storyManager = StoryManager::getInstance();
    
    auto story_list = storyManager->gets();
    
    /*
    auto *scroll = ScrollView::create(window_size);
    scroll->setDirection(ScrollView::Direction::VERTICAL);
    addChild(scroll);
     */
    
    std::vector<Story*>::iterator it = story_list.begin();
    
    //テスト文言
    auto string_txt = "日本の地獄は自慢の地獄\n罪にあわせておもてなし\n(はい!)\n日本の地獄はじゅうろくしょう地獄!\n全部合わせて、にひゃくななじゅうにもあるーーーーーー!!\n\nここは地獄!地獄!素敵な地獄!\n地獄、じご、じご、じごくだよ～!!\n\nここは地獄!地獄!楽しい地獄!\n地獄、じご、じご、じごくだよ～!!\n\n等活(とうかつ)黒縄(こくじょう)衆合(しゅうごう)\n叫喚(きょうかん)\n大叫喚(だいきょうかん)\n焦熱(しょうねつ)に 大焦熱(だいしょうねつ) 阿鼻!！\n不喜処(ふきしょ)(に)瓮熟処(おうじゅくしょ)、\n如飛虫堕処(にょひちゅうだしょ)\n受苦無有数量処(じゅくむうすうりょうしょ)\n屎泥処(しでいしょ)(に)、刀輪処(とうりんしょ)\n多苦処(たくしょ)(に)闇冥処(あんみゅうしょ)(に)極苦処(ごくくしょ)\n\n頞部陀(あぶだ)、刺部陀(にらぶだ)\n頞听陀(あただ)、臛臛婆(かかば)、虎虎婆(ここば)\n嗢鉢羅(うばら)、(に)、鉢特摩(はどま)(に)摩訶鉢特摩(まかはどま)\n\n衆病処(しゅうびょうしょ)、(ったら) 雨鉄処(うてつしょ)、(ったら)\n悪杖処(あくじょうしょ)、黒色鼠狼処(こくしょくそうろうしょ)、\n針山、血の池、まだまだ色々!\n\n地獄はあるかもしれないよ!!\nこの世の行い気をつけてー!\n\nここは地獄!地獄!素敵な地獄!\n地獄、じご、じご、じごくだよ～!!\n\nここは地獄!地獄!楽しい地獄!\n地獄、じご、じご、じご、じご、じご、\nじご、じご、じご、じご、じご、じごくだよ～!!\n\n嗚呼～ \n";
    
    //Scrollview
    auto *scroll = ScrollView::create(window_size);
    //スクロールする方向は縦のみ
    scroll->setDirection(ScrollView::Direction::VERTICAL);
    addChild(scroll);
    
    //Label
    auto label = LabelTTF::create(string_txt, "Arial Rounded MT Bold", 15);
    label->setHorizontalAlignment(TextHAlignment::LEFT);
    label->setDimensions(Size(window_size.width,0));
    label->setDimensions(Size(label->getContentSize().width, label->getContentSize().height));
    
    scroll->setContainer(label);
    scroll->setContentOffset(Point(0, 0 - (label->getContentSize().height - window_size.height)));
    
    /*
    while (it != story_list.end()) {
        auto text = LabelTTF::create(string_txt, "Arial Rounded MT Bold", 15);
        text->setHorizontalAlignment(TextHAlignment::LEFT);
        text->setDimensions(Size(window_size.width,0));
        text->setDimensions(Size(text->getContentSize().width, text->getContentSize().height));
        scroll->setContainer(text);
     
        CCLOG("id:%d", it.operator*()->getId());
        CCLOG("hash:%s", it.operator*()->getHash().c_str());
        CCLOG("title:%s", it.operator*()->getTitle().c_str());
        ++it;
    }
    */
    
    return true;
}