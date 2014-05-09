#include "HelloWorldScene.h"
#include "picojson.h"

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() ) return false;
    
    //HTTP通信
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    HttpRequest *request = new HttpRequest();
    
    //Json取得
    request->setUrl("http://raharu.net/naraku/app/test");
    request->setRequestType(HttpRequest::Type::GET);
    auto ad = &HelloWorld::onHttpRequestCallBack;
    request->setResponseCallback(this, SEL_HttpResponse(ad));
    request->setHeaders(headers);
    HttpClient::getInstance()->send(request);
    request->release();
    
    return true;
}

//callback
void HelloWorld::onHttpRequestCallBack(HttpClient* sender, HttpResponse* response)
{
    picojson::value response_data;
    std::string err;
    
    CCASSERT(response, "response empty");
    CCASSERT(response->isSucceed(), "response failed");
    
    std::vector<char>* buffer = response->getResponseData();
    char * json = (char *) malloc(buffer->size() + 1);
    std::string s2(buffer->begin(), buffer->end());
    strcpy(json, s2.c_str());
    picojson::parse(response_data, json, json + strlen(json), &err);
    
    CCASSERT(err.empty(), err.c_str());
    
    picojson::object& values = response_data.get<picojson::object>();
    
    //id
    auto id = Value(values["id"].get<double>());
    CCLOG("%d", id.asInt());
    
    //title
    auto title = Value(values["title"].get<std::string>());
    CCLOG("%s", title.asString().c_str());
    
    //is_bool
    auto is_bool = Value(values["is_bool"].get<bool>());
    CCLOG("%s", is_bool.asString().c_str());
}


