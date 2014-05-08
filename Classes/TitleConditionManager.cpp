//
//  TitleConditionManager.cpp
//  Abyss
//
//  Created by raharu on 2014/05/07.
//
//

#include "TitleConditionManager.h"

static TitleConditionManager *s_SharedCondition = nullptr;


//getInstance
TitleConditionManager* TitleConditionManager::getInstance()
{
    if (!s_SharedCondition)
    {
        s_SharedCondition = new TitleConditionManager();
        s_SharedCondition->init();
    }
    
    return s_SharedCondition;
}

//init
bool TitleConditionManager::init(void)
{
    //初期化
    setCondition(1);
    setLoading(1);
    setProcessing(1);
    return true;
}

//状態フラグ
bool TitleConditionManager::isConditionIdle() {return this->_condition == kConditionIdle;}
bool TitleConditionManager::isConditionLoading() {return this->_condition == kConditionLoading;}
bool TitleConditionManager::isConditionProcessing() {return this->_condition == kConditionProcessing;}

//読み込み分類
bool TitleConditionManager::isLoadNone() {return this->_loading == kLoadNone;}
bool TitleConditionManager::isLoadStoryIdMax() {return this->_loading == kLoadStoryIdMax;}
bool TitleConditionManager::isLoadStoryIds() {return this->_loading == kLoadStoryIds;}
bool TitleConditionManager::isLoadStory() {return this->_loading == kLoadStory;}

//処理分類
bool TitleConditionManager::isProcessNone() {return this->_processing == kProcessNone;}
bool TitleConditionManager::isProcessDbCheck() {return this->_processing == kProcessDbCheck;}
bool TitleConditionManager::isProcessStoryIdCheck() {return this->_processing == kProcessStoryIdCheck;}
