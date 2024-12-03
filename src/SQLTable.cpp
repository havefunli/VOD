#include "../include/SQLTable.h"

// 全局变量定义
const char *DEFAULT_HOST = "0.0.0.0";
const char *DEFAULT_USER = "lmy";
const char *DEFAULT_PASSWD = "20040823@Lq";
const char *DEFAULT_DB = "VOD";
const uint16_t DEFAULT_PORT = 0;
const char *DEFAULT_CHAR = "utf8";

MYSQL *SqlTable::SQLConnect(const char* database)
{
    MYSQL *conn = mysql_init(nullptr);
    if (conn == nullptr)
    {
        spdlog::error("mysql_init error, info = {}", mysql_error(conn));
        return conn;
    }
    /*设置相应的信息*/
    MYSQL *ret = mysql_real_connect(conn, DEFAULT_HOST,
                                    DEFAULT_USER, DEFAULT_PASSWD,
                                    database, DEFAULT_PORT,
                                    nullptr, 0);
    if (ret == nullptr)
    {
        spdlog::error("mysql_real_connect, info = {}", mysql_error(conn));
        mysql_close(conn);
        return nullptr;
    }
    /*设置字符集*/
    if (mysql_set_character_set(conn, DEFAULT_CHAR) != 0)
    {
        spdlog::error("mysql_set_character_set, info = {}", mysql_error(conn));
        mysql_close(conn);
        return nullptr;
    }
    return conn;
}

void SqlTable::SQLDestroy(MYSQL* conn)
{
    if (conn != nullptr) { mysql_close(conn); } 
}

bool SqlTable::SQLQuery(MYSQL* conn, const std::string& cmd)
{
    int n = mysql_query(conn, cmd.c_str());
    if (n != 0)
    {
        spdlog::error("sql = {} error, info = {}", cmd, mysql_error(conn));
        return false;
    }
    return true;
}


