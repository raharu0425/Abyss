#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include <WebSocket.h>

USING_NS_CC;
using namespace network;

class Title : public cocos2d::Layer, WebSocket::Delegate
{
    //webSocket
    WebSocket* websocket;
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    Title();
    //~Title();
    CREATE_FUNC(Title);
    
    //Properties
    cocos2d::Size window_size;
    cocos2d::LabelTTF* status;
    
    //タイトル作成
    void crateTitle();
    
    //外部サーバーとの接続
    void loadStory();
    
    //webSocket
    virtual void onOpen(WebSocket* ws);
    virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
    virtual void onClose(WebSocket* ws);
    virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
    void sendMessage(float dt);
    
    
};

#endif // __TITLE_SCENE_H__
