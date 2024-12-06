#pragma once
#include "SQLTable.h"
#include "httplib.h"
#include "../src/Util.hpp"
#include <ctime>
#include <functional>

extern const int MAXNUM;
extern const uint16_t PORT;
extern const std::string BASEDIR;
extern const std::string VIDEODIR;
extern const std::string IMAGEDIR;
extern const std::string ERROR_PAGE;
using SQLPtr = std::shared_ptr<SqlTable>;
using Handler = std::function<void(const httplib::Request&, httplib::Response&)>;

class Server
{
private:
    void RegisterFunc();

    /*产生一个随机数，命名使用*/
    static int RandomNum()
    {
        return std::rand() % MAXNUM;
    }

    /*对应的事件处理函数*/
    static void Insert(const httplib::Request&, httplib::Response&);
    static void Delete(const httplib::Request&, httplib::Response&);
    static void Update(const httplib::Request&, httplib::Response&);
    static void SelectAll(const httplib::Request&, httplib::Response&);
    static void SelectOne(const httplib::Request&, httplib::Response&);

public:
    explicit Server(uint16_t port = PORT);
    
    ~Server();

    void SetGet(const Handler&);

    /*开始运行*/
    void Start();


private:
    static SQLPtr   _conn;/*负责数据库的连接*/
    uint16_t        _port;/*设置端口*/
    httplib::Server _srv; /*负责服务端的建立*/
};