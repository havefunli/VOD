#include "../include/Server.h"

const int MAXNUM = 10000;
const uint16_t PORT = 8080;
const std::string BASEDIR = "../wwwroot";
const std::string VIDEODIR = "/video/";
const std::string IMAGEDIR = "/cover/";
const std::string ERROR_PAGE = "../wwwroot/error.html";

/*初始化数据库连接*/
SQLPtr Server::_conn = std::shared_ptr<SqlTable>(new SqlTable());


/*id name description type video cover*/
void Server::Insert(const httplib::Request &req, httplib::Response &rsp)
{
    /*数据格式有误*/
    if (!req.has_file("name") || !req.has_file("description") || !req.has_file("type") || !req.has_file("video") || !req.has_file("cover"))
    {
        rsp.status = 400;
        std::string error_page;
        FileUtil::GetContent(ERROR_PAGE, error_page);
        rsp.set_content(error_page, "text/plain");
        return;
    }

    /*提取数据*/
    httplib::MultipartFormData name = req.get_file_value("name");
    httplib::MultipartFormData desc  = req.get_file_value("description");
    httplib::MultipartFormData type = req.get_file_value("type");
    httplib::MultipartFormData video = req.get_file_value("video");
    httplib::MultipartFormData cover = req.get_file_value("cover");

    /*存储图片视频*/
    // std::string suffix = std::to_string(RandomNum());
    std::string video_path = BASEDIR + VIDEODIR + name.content + "_" + video.filename;
    std::string cover_path = BASEDIR + IMAGEDIR + name.content + "_" + cover.filename;
    if (!FileUtil::SetContent(video_path, video.content) 
    || !FileUtil::SetContent(cover_path, cover.content))
    {
        std::string error_page = R"(<!DOCTYPE html>
                                <html lang="zh-CN">
                                <head>
                                    <meta charset="UTF-8">
                                    <meta name="viewport" content="width=device-width, initial-scale=1.0">
                                    <title>视频保存出错</title>
                                    <style>
                                        body {
                                            font-family: Arial, sans-serif;
                                            text-align: center;
                                            margin-top: 50px;
                                        }
                                        .error-message {
                                            font-size: 24px;
                                            color: red;
                                        }
                                    </style>
                                </head>
                                <body>
                                    <div class="error-message">视频保存出错</div>
                                </body>
                                </html>)";
        rsp.status = 500;
        rsp.set_content(error_page, "text/plain");

        return;
    }

    /*保存到数据库*/
    Json::Value root;
    root["name"] = name.content;
    root["description"] = desc.content;
    root["type"] = std::stoi(type.content); // 这里注意
    root["video_path"] = VIDEODIR + name.content + "_" + video.filename;
    root["cover_path"] = IMAGEDIR + name.content + "_" + cover.filename;
    if (!_conn->Insert(root))
    {
        std::string error_page = R"(<!DOCTYPE html>
                                <html lang="zh-CN">
                                <head>
                                    <meta charset="UTF-8">
                                    <meta name="viewport" content="width=device-width, initial-scale=1.0">
                                    <title>视频保存出错</title>
                                    <style>
                                        body {
                                            font-family: Arial, sans-serif;
                                            text-align: center;
                                            margin-top: 50px;
                                        }
                                        .error-message {
                                            font-size: 24px;
                                            color: red;
                                        }
                                    </style>
                                </head>
                                <body>
                                    <div class="error-message">视频保存出错</div>
                                </body>
                                </html>)";
        rsp.status = 500;
        rsp.set_content(error_page, "text/plain");
        return;
    }
    rsp.set_content( R"({"result": true, "reason": "successfully inserted data"})", "text/plain");
    spdlog::info("Successful insert video = {}", name.content);
    return;
}

void Server::Delete(const httplib::Request &req, httplib::Response &rsp)
{
    /*1. get video id*/
    int video_id = atoi(req.matches[1].str().c_str());
    spdlog::debug("delete video id = {}", video_id);
    /*2. get file path*/
    Json::Value root;
    bool ret = _conn->SelectOne(video_id, root);
    if (!ret)
    {
        rsp.status = 400;
        std::string error = R"({"result": false, "reason": "Not found video_id"})";
        rsp.set_content(error, "text/plain");
        return;
    }
    std::string video_path = root["video_path"].asCString();
    std::string cover_path = root["cover_path"].asCString();
    /*3. delete file in the mysql*/
    ret = _conn->Delete(video_id);
    /*4. delete file locally*/
    if (!FileUtil::DeleteFile(BASEDIR + video_path) 
    || !FileUtil::DeleteFile(BASEDIR + cover_path))
    {
        rsp.status = 500;
        std::string error = R"({"result": false, "reason": "Not found video_file"})";
        rsp.set_content(error, "text/plain");
        return;
    }
    rsp.set_content( R"({"result": true, "reason": "successfully delete data"})", "text/plain");
    spdlog::info("Successful execute Delete");
    return;
}

/*id name description type video_path cover_path*/
void Server::Update(const httplib::Request& req, httplib::Response& rsp)
{
    /*1. get video id*/
    int video_id = std::stoi(req.matches[1].str().c_str());
    /*2. get video new info*/
    Json::Value root;
    if (!JsonUtil::Deserialize(req.body, root))
    {
        rsp.status = 400;
        std::string error = R"({"result": false, "reason": "json format error"})";
        rsp.set_content(error, "text/plain");
        return;
    }
    /*3. update video info in the mysql*/
    if (!_conn->Update(video_id, root))
    {
        rsp.status = 500;
        std::string error = R"({"result": false, "reason": "mysql update error"})";
        rsp.set_content(error, "text/plain");
        return;
    }   
    rsp.set_content( R"({"result": true, "reason": "successfully updated data"})", "text/plain");
    spdlog::info("Successful execute Update");
    return;
}

// /video?keyword=info
void Server::SelectAll(const httplib::Request& req, httplib::Response& rsp)
{
    /*1. judge search type*/
    bool is_fuzzy = false;
    std::string keyword;
    if (req.has_param("keyword"))
    {
        is_fuzzy = true;
        keyword = req.get_param_value("keyword");
        spdlog::debug("the search key word = {}", keyword);
    }
    /*2. search info*/
    bool ret;
    Json::Value root;
    if (!is_fuzzy) { ret = _conn->SelectAll(root); }
    else { ret = _conn->SelectLike(keyword, root); }
    if (!ret)
    {
        rsp.status = 500;
        std::string error = R"({"result": false, "reason": "failed to get video"})";
        rsp.set_content(error, "text/plain");
        return;        
    }
    /*3. serialize the data*/
    std::string video_info;
    JsonUtil::Serialize(root, video_info);
    /*4. return data*/
    rsp.set_content(video_info, "application/json");
    spdlog::info("Successfully execute SelectAll");
    return;
}

void Server::SelectOne(const httplib::Request& req, httplib::Response& rsp)
{
    /*1. get video id*/
    int video_id = std::stoi(req.matches[1].str().c_str());
    /*2. get video from mysql*/
    Json::Value root;
    bool ret = _conn->SelectOne(video_id, root);
    if (!ret)
    {
        rsp.status = 500;
        std::string error = R"({"result": false, "reason": "failed to get video"})";
        rsp.set_content(error, "text/plain");
        return;
    }
    /*3. serialize the data*/
    std::string video_info;
    JsonUtil::Serialize(root, video_info);
    /*4. return data*/
    rsp.set_content(video_info, "application/json");
    spdlog::info("Successfully execute SelectOne");
    return;
}

Server::Server(uint16_t port)
    : _port(port)
{
    if (_conn == nullptr)
    {
        spdlog::error("MySQL is not connected");
        exit(-1);
    }
}

Server::~Server() = default;

void Server::RegisterFunc()
{
    _srv.Put("/video/(\\d+)", Server::Update);
    _srv.Post("/video/", Server::Insert);
    _srv.Get("/video/", Server::SelectAll);
    _srv.Get("/video/(\\d+)", Server::SelectOne);
    _srv.Delete("/video/(\\d+)", Server::Delete);
}

void Server::SetGet(const Handler& handle)
{
    _srv.Get("/", handle);
}

void Server::Start()
{
    // 1. set file path
    _srv.set_mount_point("/", BASEDIR);
    // 2. 设置处理函数
    RegisterFunc();
    // 3. 启动服务器
    spdlog::info("The server now is running on port = {}", _port);
    _srv.listen("0.0.0.0", _port);
}