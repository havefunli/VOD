#include "../include/SQLTable.h"

// 全局变量定义
const char *DEFAULT_HOST = "0.0.0.0";
const char *DEFAULT_USER = "lmy";
const char *DEFAULT_PASSWD = "20040823@Lq";
const char *DEFAULT_DB = "VOD";
const uint16_t DEFAULT_PORT = 0;
const char *DEFAULT_CHAR = "utf8";
const int DEFAULT_SIZE = 4096;

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

SqlTable::SqlTable()
    : _conn(SQLConnect())
{}

SqlTable::~SqlTable()
{
    SQLDestroy(_conn);
}

bool SqlTable::Insert(const Json::Value& root)
{
    char tmp[DEFAULT_SIZE] = { '\0' };
    /*id name description type video_path cover_path*/
    snprintf(tmp, DEFAULT_SIZE, "insert into tb_video(name, description, video_path, cover_path) \
                                        values('%s', '%s', '%s', '%s');", 
                                        root["name"].asCString(), 
                                        root["description"].asCString(),
                                        root["video_path"].asCString(),
                                        root["cover_path"].asCString());
    return SQLQuery(_conn, tmp);
}
 
bool SqlTable::Delete(int video_id)
{
    char tmp[DEFAULT_SIZE] = { '\0' };
    snprintf(tmp, DEFAULT_SIZE, "delete from tb_video where id = %d", video_id);
    return SQLQuery(_conn, tmp);
}

/*id name description type video_path cover_path*/
bool SqlTable::Update(int video_id, const Json::Value& root)
{
    char tmp[DEFAULT_SIZE] = { '\0' };
    snprintf(tmp, DEFAULT_SIZE, "update tb_video set name = '%s', description = '%s' where id = %d", 
                                                root["name"].asCString(), 
                                                root["description"].asCString(), 
                                                video_id);
    return SQLQuery(_conn, tmp);
}

bool SqlTable::FetchResults(const std::string& query, Json::Value& root)
{
    MYSQL_RES* res;
    /*保证线程安全*/
    {
        std::unique_lock<std::mutex> lck(_mtx);
        bool ret = SQLQuery(_conn, query.c_str());
        
        if (!ret) { return false; }
        res = mysql_store_result(_conn);
    }
    if (res == nullptr)
    {
        spdlog::error("mysql_store_result error, info = {}", mysql_error(_conn));
        return false;
    }

    int num_rows = mysql_num_rows(res);
    for (int i = 0; i < num_rows; i++)
    {
        /*id name description type video_path cover_path*/
        MYSQL_ROW row = mysql_fetch_row(res);
        Json::Value data;
        data["id"] = row[0];
        data["name"] = row[1];
        data["description"] = row[2];
        data["video_path"] = row[4];
        data["cover_path"] = row[5];
        root.append(data);
    }
    
    mysql_free_result(res);
    return true;
}

bool SqlTable::SelectAll(Json::Value& root)
{
    return FetchResults("select * from tb_video;", root);
}

bool SqlTable::SelectOne(int video_id, Json::Value& root)
{
    Json::Value result;
    char tmp[DEFAULT_SIZE];
    snprintf(tmp, DEFAULT_SIZE, "select * from tb_video where id = %d", video_id);
    if (!FetchResults(tmp, result)) { return false; }
    root = result[0];
    return true;
}

bool SqlTable::SelectLike(const std::string& name, Json::Value& root)
{
    char tmp[DEFAULT_SIZE];
    snprintf(tmp, DEFAULT_SIZE, "select * from tb_video where name = '%s'", name.c_str());
    if (!FetchResults(tmp, root)) { return false; }
    return true;
}



