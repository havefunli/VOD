#pragma once
#include "../src/Util.hpp"
#include <mysql/mysql.h>

// 全局变量声明
extern const char *DEFAULT_HOST;
extern const char *DEFAULT_USER;
extern const char *DEFAULT_PASSWD;
extern const char *DEFAULT_DB;
extern const uint16_t DEFAULT_PORT;
extern const char *DEFAULT_CHAR;

class SqlTable
{
public:
    /*连接mysql数据库特定的数据表*/
    static MYSQL *SQLConnect(const char* database = DEFAULT_DB);
    /*销毁连接*/
    static void SQLDestroy(MYSQL* conn);
    /*数据库执行*/
    static bool SQLQuery(MYSQL* conn, const std::string& cmd);
};