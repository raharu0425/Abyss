//
//  Story.h
//  Abyss
//
//  Created by raharu on 2014/05/11.
//
//

#ifndef __Abyss__Story__
#define __Abyss__Story__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Story : public Ref
{
public:
    //Method
    static Story* getInstance();
    bool init();
    
    // struct
    CC_SYNTHESIZE(int, _id, Id);
    CC_SYNTHESIZE(std::string, _hash, Hash);
    CC_SYNTHESIZE(std::string, _title, Title);
    CC_SYNTHESIZE(std::string, _text, Text);
    CC_SYNTHESIZE(std::string, _length, Lenght);
    CC_SYNTHESIZE(int, _number, Numer);
    CC_SYNTHESIZE(int, _number_max, NumberMax);
};


#endif /* defined(__Abyss__Story__) */
