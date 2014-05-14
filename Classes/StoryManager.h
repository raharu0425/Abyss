//
//  StoryManager.h
//  Abyss
//
//  Created by raharu on 2014/05/09.
//
//

#ifndef __Abyss__StoryManager__
#define __Abyss__StoryManager__

#include <iostream>
#include "cocos2d.h"
#include "sqlite3.h"
#include "picojson.h"
#include "Story.h"

USING_NS_CC;

class StoryManager : public cocos2d::Ref
{
    //sqlite3
    sqlite3* useDataBase = NULL;
    char* errorMessage = NULL;
    
public:
    //Method
    static StoryManager* getInstance();
    bool init();
    
    std::vector<Story*> gets();
    
    //初期テーブルの作成
    void createInitTables();
    
    //ストーリーの追加
    void addStory(picojson::value values);
    
    //ストーリのカウント数
    int getStoryCount();
    
};

#endif /* defined(__Abyss__StoryManager__) */
