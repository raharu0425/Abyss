//
//  HTTPManager.h
//  Abyss
//
//  Created by raharu on 2014/05/09.
//
//

#ifndef __HTTPManager__
#define __HTTPManager__

#include <iostream>
#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "library/json/picojson.h"


USING_NS_CC;
using namespace network;

class HTTPManager : public Ref
{
    picojson::value pico_value;
    std::string error_msg;
    
public:
    //Method
    static HTTPManager* getInstance();
    bool init();
    
    //GET
    static void get(Ref* test,SEL_HttpResponse callback, std::string url);
    
    //Json形式で返してもらう
    picojson::value getJsonByResponseData(HttpResponse* response);
};

#endif // __HTTPManager__
