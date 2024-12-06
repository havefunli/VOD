#include "../include/Server.h"

const uint16_t DEFAULT_PORT = 8080;
const std::string DEFAULT_BASEDIR = "../wwwroot";
const std::string DEFAULT_VIDEODIR = "../wwwroot/video";
const std::string DEFAULT_IMAGEDIR = "../wwwroot/image";


Server::Server(uint16_t port)
    : _port(port)
{
    if (_conn == nullptr) { exit(-1); }
}
    
Server::~Server() = default;

void Server::RegisterFunc()
{
    _srv.Put("/video/(\\d+)", Server::Update);
    _srv.Get("/video", Server::SelectAll);
    _srv.Get("/video/(\\d+)", Server::SelectOne);
    _srv.Post("/video", Server::Insert);
    _srv.Delete("/video/(\\d+)", Server::Delete);
}

void Server::Start()
{
    // 1. set file path
    _srv.set_mount_point("/", DEFAULT_BASEDIR);
    // 2. 设置处理函数
    RegisterFunc();
    // 3. 启动服务器
    _srv.listen("0.0.0.0", _port);
}