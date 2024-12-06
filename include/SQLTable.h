#pragma once
#include "../src/Util.hpp"
#include <utility>
#include <mysql/mysql.h>
#include <mutex>

// 全局变量声明
extern const char *DEFAULT_HOST;
extern const char *DEFAULT_USER;
extern const char *DEFAULT_PASSWD;
extern const char *DEFAULT_DB;
extern const uint16_t DEFAULT_PORT;
extern const char *DEFAULT_CHAR;
extern const int DEFAULT_SIZE;

using StringPair = std::pair<std::string, std::string>;

class SqlTable
{
private:
    bool FetchResults(const std::string&, Json::Value&);
public:
    /*连接mysql数据库特定的数据表*/
    static MYSQL *SQLConnect(const char* database = DEFAULT_DB);
    /*销毁连接*/
    static void SQLDestroy(MYSQL* conn);
    /*数据库执行*/
    static bool SQLQuery(MYSQL* conn, const std::string& cmd);
    
    /*初始化函数*/
    SqlTable();
    /*析构函数*/
    ~SqlTable();

    /*添加数据*/
    bool Insert(const Json::Value&);
    /*删除数据*/
    bool Delete(int);
    /*修改数据*/
    bool Update(int, const Json::Value&);
    /*查询数据*/
    bool SelectAll(Json::Value&);
    bool SelectOne(int, Json::Value&);
    bool SelectLike(const std::string&, Json::Value&);


private:
    MYSQL* _conn;
    std::mutex _mtx;
};