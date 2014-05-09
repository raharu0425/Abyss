//
//  TitleConditionManager.h
//  Abyss
//
//  Created by raharu on 2014/05/07.
//
//

#ifndef __Abyss__TitleConditionManager__
#define __Abyss__TitleConditionManager__

#include <iostream>
#include "cocos2d.h"

class TitleConditionManager : public cocos2d::Ref
{
public:
    
    //Condition
    enum kCondition{
        kConditionIdle = 1,
        kConditionLoading,
        kConditionProcessing
    };
    
    //LoadingWork
    enum kLoad{
        kLoadNone = 1,
        kLoadStoryIdMax,
        kLoadStoryIds,
        kLoadStory
    };
    
    //ProcessWork
    enum kProcess{
        kProcessNone = 1,
        kProcessDbCheck
    };
    
    //Method
    static TitleConditionManager* getInstance();
    bool init();
    
    //Flagment
    CC_SYNTHESIZE(int, _condition, Condition);
    CC_SYNTHESIZE(int, _loading, Loading);
    CC_SYNTHESIZE(int, _processing, Processing);
    
    //Enable
    bool isConditionIdle();
    bool isConditionLoading();
    bool isConditionProcessing();
    
    //Load
    bool isLoadNone();
    bool isLoadStoryIdMax();
    bool isLoadStoryIds();
    bool isLoadStory();
    
    //Process
    bool isProcessNone();
    bool isProcessDbCheck();
    
};

#endif /* defined(__Abyss__TitleConditionManager__) */
