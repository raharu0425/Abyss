//
//  HTTPManager.cpp
//  Abyss
//
//  Created by raharu on 2014/05/09.
//
//

#include "HTTPManager.h"

static HTTPManager *s_Shared = nullptr;

//getInstance
HTTPManager* HTTPManager::getInstance()
{
    s_Shared = new HTTPManager();
    s_Shared->retain();
    return s_Shared;
}

//GET
void HTTPManager::get(Ref* layer,SEL_HttpResponse callback, std::string url)
{
    //HTTP通信
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    HttpRequest *request = new HttpRequest();
    
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(layer, callback);
    request->setHeaders(headers);
    HttpClient::getInstance()->send(request);
    request->release();
}

//Json形式で返す
picojson::value HTTPManager::getJsonByResponseData(HttpResponse* response)
{
    //Exceptiom
    CCASSERT(response, "connecting failed");
    CCASSERT(response->isSucceed(), "response failed");
    
    //バッファの取得
    std::vector<char>* buffer = response->getResponseData();
    char * json = (char *) malloc(buffer->size() + 1);
    std::string s2(buffer->begin(), buffer->end());
    strcpy(json, s2.c_str());
    picojson::parse(pico_value, json, json + strlen(json), &error_msg);
    
    //エラー
    CCASSERT(error_msg.empty(), error_msg.c_str());
    
    return pico_value;
}