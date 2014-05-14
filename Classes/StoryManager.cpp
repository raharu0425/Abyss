//
//  StoryManager.cpp
//  Abyss
//
//  Created by raharu on 2014/05/09.
//
//

#include "StoryManager.h"
#include "picojson.h"



static StoryManager *s_Shared = nullptr;

//getInstance
StoryManager* StoryManager::getInstance()
{
    s_Shared = new StoryManager();
    
    if (s_Shared && s_Shared->init()) {
        s_Shared->retain();
        return s_Shared;
    }else{
        CC_SAFE_DELETE(s_Shared);
        return NULL;
    }
}

// init
bool StoryManager::init()
{
    auto filePath = cocos2d::FileUtils::getInstance()->getWritablePath();
    filePath.append("Abyss.db");
    auto status = sqlite3_open(filePath.c_str(), &useDataBase);
    if (status != SQLITE_OK) return false;
    
    return true;
}

/*
 * 初期テーブルの作成
 */
void StoryManager::createInitTables()
{
    //Storyテーブル作成
    auto create_sql = "CREATE TABLE IF NOT EXISTS `story` (`id` INTEGER PRIMARY KEY AUTOINCREMENT,`hash` varchar(128)  NOT NULL ,`title` varchar(256) NOT NULL,`text` text NOT NULL,`length` varchar(16) NOT NULL,`type` varchar(16)  NOT NULL ,`number` int(2) NOT NULL,`max_number` int(2) NOT NULL DEFAULT '1',`report_id` int(10) DEFAULT NULL ,`is_release` int(2) NOT NULL DEFAULT '0' ,`is_selection` int(2) NOT NULL DEFAULT '0',`created_at` int(11) NOT NULL DEFAULT '0',`updated_at` int(11) NOT NULL DEFAULT '0',`deleted_at` int(11) NOT NULL DEFAULT '0' ,`active` int(2) NOT NULL DEFAULT '1')";
    
    auto sts = sqlite3_exec(useDataBase, create_sql, NULL, NULL, &errorMessage );
    if( sts != SQLITE_OK ) CCLOG("create table failed : %s", errorMessage);
    
    //StoryVarsテーブルの作成
    auto story_vars = "CREATE TABLE IF NOT EXISTS `story_words` (`id` INTEGER PRIMARY KEY AUTOINCREMENT,`story_id` int(10) NOT NULL ,`story_hash` varchar(64) NOT NULL,`word` varchar(32) NOT NULL,`push_count` int(5) NOT NULL DEFAULT '0',`last_ip` varchar(32) DEFAULT NULL,`is_enable` int(2) NOT NULL DEFAULT '0',`created_at` int(11) NOT NULL DEFAULT '0',`updated_at` int(11) NOT NULL DEFAULT '0',`deleted_at` int(11) NOT NULL DEFAULT '0',`active` int(2) NOT NULL DEFAULT '1')";
    auto sts2 = sqlite3_exec(useDataBase, story_vars, NULL, NULL, &errorMessage );
    if( sts2 != SQLITE_OK ) CCLOG("create table failed : %s", errorMessage);
    
    //indexの作成
    auto story_vars_index_1 = "CREATE INDEX `story_id` ON story_words(`story_id`)";
    auto sts3 = sqlite3_exec(useDataBase, story_vars_index_1, NULL, NULL, &errorMessage );
    if( sts3 != SQLITE_OK ) CCLOG("create index failed : %s", errorMessage);
    
    auto story_vars_index_2 = "CREATE INDEX `tag` ON story_words(`word`)";
    auto sts4 = sqlite3_exec(useDataBase, story_vars_index_2, NULL, NULL, &errorMessage );
    if( sts4 != SQLITE_OK ) CCLOG("create index failed : %s", errorMessage);
    
    auto story_vars_index_3 = "CREATE INDEX `story_hash` ON story_words(`story_hash`)";
    auto sts5 = sqlite3_exec(useDataBase, story_vars_index_3, NULL, NULL, &errorMessage );
    if( sts5 != SQLITE_OK ) CCLOG("create index failed : %s", errorMessage);
}

//複数の取得
std::vector<Story*> StoryManager::gets()
{
    sqlite3_stmt* stmt;
    auto sql = "select * from `story` group by `hash` order by `id`";
    const char *pzTest;
    sqlite3_prepare_v2(useDataBase, sql, std::strlen(sql), &stmt, &pzTest);
    sqlite3_reset(stmt);
    
    std::vector<Story*> storyies;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto story = Story::getInstance();
        story->setId(sqlite3_column_int(stmt, 0));
        story->setHash((char*)sqlite3_column_text(stmt, 1));
        story->setTitle((char*)sqlite3_column_text(stmt, 2));
        story->setText((char*)sqlite3_column_text(stmt, 3));
        story->setNumer(sqlite3_column_int(stmt, 4));
        story->setNumberMax(sqlite3_column_int(stmt, 5));
        
        storyies.push_back(story);
    }
    sqlite3_finalize(stmt);
    
    return storyies;
}


//ストーリーの作成
void StoryManager::addStory(picojson::value pico_value)
{
    sqlite3_stmt* stmt;
    char* sql = "INSERT INTO `story` (`id`, `hash`, `title`, `text`, `length`, `type`, `number`, `max_number`, `created_at`, `updated_at`) VALUES(?,?,?,?,?,?,?,?,?,?)";
    const char *pzTest;
    sqlite3_prepare_v2(useDataBase, sql, std::strlen(sql), &stmt, &pzTest);
    sqlite3_reset(stmt);
    
    //取得
    picojson::object& record = pico_value.get<picojson::object>();
    auto id = Value(record["id"].get<std::string>());
    std::string hash = record["hash"].get<std::string>();
    std::string title = record["title"].get<std::string>();
    std::string text = record["text"].get<std::string>();
    std::string length = record["length"].get<std::string>();
    auto type = Value(record["type"].get<std::string>());
    auto number = Value(record["number"].get<std::string>());
    auto max_number = Value(record["max_number"].get<std::string>());
    auto created_at = Value(record["created_at"].get<std::string>());
    auto updated_at = Value(record["updated_at"].get<std::string>());
    
    CCLOG("id:%d", id.asInt());
    CCLOG("hash:%s", hash.c_str());
    
    sqlite3_bind_int(stmt, 1, id.asInt());
    sqlite3_bind_text(stmt, 2, hash.c_str(), std::strlen(hash.c_str()), 0);
    sqlite3_bind_text(stmt, 3, title.c_str(), std::strlen(title.c_str()), 0);
    sqlite3_bind_text(stmt, 4, text.c_str(), std::strlen(text.c_str()), 0);
    sqlite3_bind_text(stmt, 5, length.c_str(), std::strlen(length.c_str()), 0);
    sqlite3_bind_int(stmt, 6, type.asInt());
    sqlite3_bind_int(stmt, 7, number.asInt());
    sqlite3_bind_int(stmt, 8, max_number.asInt());
    sqlite3_bind_int(stmt, 9, created_at.asInt());
    sqlite3_bind_int(stmt, 10, updated_at.asInt());
    
    // stmtのSQLを実行
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

//ストーリー数の取得
int StoryManager::getStoryCount()
{
    sqlite3_stmt* stmt;
    char* sql = "select count(DISTINCT(`hash`)) as count from `story`";
    const char *pzTest;
    sqlite3_prepare_v2(useDataBase, sql, std::strlen(sql), &stmt, &pzTest);
    sqlite3_reset(stmt);
    
    sqlite3_step(stmt);
    return sqlite3_column_int(stmt, 0);;
}
