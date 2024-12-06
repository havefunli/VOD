#pragma once
#include "SQLTable.h"
#include "httplib.h"
#include <functional>

extern const uint16_t DEFAULT_PORT;
extern const std::string DEFAULT_BASEDIR;
extern const std::string DEFAULT_VIDEODIR;
extern const std::string DEFAULT_IMAGEDIR;
using SQLPtr = std::shared_ptr<SqlTable>;


class Server
{
private:
    void RegisterFunc();

    /*对应的事件处理函数*/
    static void Insert(const httplib::Request&, httplib::Response&);
    static void Delete(const httplib::Request&, httplib::Response&);
    static void Update(const httplib::Request&, httplib::Response&);
    static void SelectAll(const httplib::Request&, httplib::Response&);
    static void SelectOne(const httplib::Request&, httplib::Response&);

public:
    explicit Server(uint16_t port = DEFAULT_PORT);
    
    ~Server();

    /*开始运行*/
    void Start();


private:
    SQLPtr          _conn;/*负责数据库的连接*/
    uint16_t        _port;/*设置端口*/
    httplib::Server _srv; /*负责服务端的建立*/
};