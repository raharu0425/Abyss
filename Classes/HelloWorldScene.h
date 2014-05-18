#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "network/HttpRequest.h"
#include "network/HttpClient.h"


USING_NS_CC;
using namespace network;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);
    
    //コールバック
    void onHttpRequestCallBack(HttpClient* sender, HttpResponse* response);
    
};

#endif // __HELLOWORLD_SCENE_H__
