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
    
    
    //ストーリー
    storyManager = StoryManager::getInstance();
    
    //話数
    story_count = storyManager->getStoryCount();
    
    //リスト
    story_list = storyManager->gets();
    
    /*
    
    //画面サイズサイズを取得
    window_size = Director::getInstance()->getWinSize();
    
    TableView* tableView = TableView::create(this, Size(window_size.width / 100 * 80, window_size.height / 100 * 80));
    //展開方向
    tableView->setDirection(TableView::Direction::VERTICAL);
    //表示順序上からしたへ
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setPosition(Point((window_size.width - tableView->getContentSize().width) / 2, 20));
    tableView->setBounceable(false);
    //追加
    tableView->setDelegate(this);
    addChild(tableView);
    tableView->reloadData();
     */
     
    return true;
}

/*
// セルのサイズを設定（横：画面いっぱい / 縦：ここでは画像サイズに合わせて100に設定）
Size Overview::cellSizeForTable(TableView *table){
    return Size(window_size.width, 25);
}

// セルの内容を設定
TableViewCell* Overview::tableCellAtIndex(TableView *table, ssize_t idx){
    std::string id = StringUtils::format("%zd", idx);
    std::string text = StringUtils::format("Line %zd", idx);
    TableViewCell *cell = table->dequeueCell();
    
    cell = new TableViewCell();
    cell->autorelease();
    
    
    // セルの背景
    auto background_color = Color3B(255,255,255);
    if (idx%2) {
        background_color = Color3B(200,200,200);
    }
    
    Sprite* bg = Sprite::create();
    bg->setAnchorPoint(Point(0, 0));
    bg->setTextureRect(Rect(0, 0, window_size.width, 24));
    bg->setColor(background_color);
    bg->setTag(100);
    cell->addChild(bg, 0);
    
    // ボーダーライン
    Sprite* line = Sprite::create();
    line->setAnchorPoint(Point(0, 0));
    line->setTextureRect(Rect(0, 0, window_size.width, 1));
    line->setColor(Color3B(0,0,0));
    cell->addChild(line, 1);
    
    //Story取得
    auto story = story_list.begin() + idx;
    
    // ID部分
    auto *label_1 = LabelTTF::create(story.operator*()->getTitle().c_str(), "Arial", 16);
    label_1->setAnchorPoint(Point(0, 0));
    label_1->setPosition(Point(10, 0));
    label_1->setColor(Color3B(0,0,0));
    cell->addChild(label_1, 1);
    return cell;
}

// セルの数を設定。インデックスは0から始まるので、この場合は0〜4になります。
ssize_t Overview::numberOfCellsInTableView(TableView *table)
{
    return story_count;
}

// セルを選択したときの処理。ログが出るようにしています。
void Overview::tableCellTouched(TableView* table, TableViewCell* cell){
    CCLOG("%ziのセルがタッチされました", cell->getIdx());
}

//タッチされたとき
void Overview::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    auto background = (Sprite*) cell->getChildByTag(100);
    background->setOpacity(125);
}

//離れたとき
void Overview::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    auto background = (Sprite*) cell->getChildByTag(100);
    background->setOpacity(255);
}
 */
