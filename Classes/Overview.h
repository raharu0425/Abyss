//
//  Overview.h
//  Abyss
//
//  Created by raharu on 2014/05/10.
//
//

#ifndef __Abyss__Overview__
#define __Abyss__Overview__

#include <iostream>
#include "cocos2d.h"
#include <extensions/cocos-ext.h>
#include "StoryManager.h"

USING_NS_CC;
using namespace extension;

class Overview : public Layer,
                 public TableViewDataSource,
                 public TableViewDelegate
{
    Size window_size;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    StoryManager *storyManager;
    std::vector<Story*> story_list;
    int story_count = 0;
    
    //TableViewDataSourceの抽象クラス
    virtual Size cellSizeForTable(TableView* table);
    virtual TableViewCell* tableCellAtIndex(TableView* table,ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView* table);
    
    //TableViewDelegateの抽象クラス
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    // Options
    virtual void tableCellHighlight(TableView* table,TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
   
    //TableViewDelegateが継承しているScrollViewの抽象クラス
    virtual void scrollViewDidScroll(ScrollView* view){};
    virtual void scrollViewDidZoom(ScrollView* view){};
    
    CREATE_FUNC(Overview);
    
};
#endif /* defined(__Abyss__Overview__) */
