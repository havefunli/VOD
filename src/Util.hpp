#pragma once
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory>
#include <fstream>
#include <string>
#include <jsoncpp/json/json.h>
#include <spdlog/spdlog.h>

using SWPtr = std::unique_ptr<Json::StreamWriter>;
using CRPtr = std::unique_ptr<Json::CharReader>;

class FileUtil
{
public:
    /*判断文件是否存在*/
    static bool FileExists(const std::string& file_name)
    {
        if (access(file_name.c_str(), F_OK) == 0) { return true; }
        else 
        {
            spdlog::error("{} is not exists...", file_name);
            return false;
        }
    }

    /*获取文件的大小*/
    static size_t FileSize(const std::string& file_name)
    {
        /*如果文件不存在*/
        if (!FileExists(file_name)) { return 0; }
        struct stat st;
        int ret = stat(file_name.c_str(), &st);
        if (ret == 0) { return st.st_size; }
        else
        {
            perror("stat:");
            return 0;
        }   
    }

    /*获取文件的数据*/
    static void GetContent(const std::string& file_name, std::string& out)
    {
        if (!FileExists(file_name)) { return; }
        std::ifstream ifs(file_name, std::ios_base::binary);
        if (!ifs.is_open()) 
        { 
            spdlog::error("ifstream open failed...");
            return;
        }
        /*获取文件大小，并将指针回到起始位置*/
        ifs.seekg(0, ifs.end);
        size_t size = ifs.tellg();
        ifs.seekg(0, ifs.beg); 

        out.resize(size);
        ifs.read(&out[0], size);

        ifs.close();
    }

    /*向文件写入数据*/
    static bool SetContent(const std::string& file_name, const std::string& out)
    {
        std::ofstream ofs(file_name, std::ios_base::binary);
        if (!ofs.is_open()) 
        { 
            spdlog::error("ofstream open failed...");
            return false;
        }
        /*写入数据到文件*/
        ofs.write(out.c_str(), out.size());

        ofs.close();

        spdlog::debug("Successful save file = {}", file_name);
        return true;
    }

    /*创建文件目录*/
    static bool CreateDirectory(const std::string& dir_name)
    {
        if (FileExists(dir_name)) { return true; }
        else
        {
            if (mkdir(dir_name.c_str(), 0777) == 0) { return true; }
            else { return false; }
        }
    }

    /*delete file locally*/
    static bool DeleteFile(const std::string& file_path)
    {
        int n = remove(file_path.c_str());
        if (n == -1)
        {
            perror("remove");
            return false;
        }
        spdlog::debug("Successful delete file = {}", file_path);
        return true;
    }
};

class JsonUtil
{
public:
    /*对root存储的数据序列化*/
    static void Serialize(const Json::Value& root, std::string& out)
    {
        SWPtr sw(Json::StreamWriterBuilder().newStreamWriter());
        std::stringstream ss;
        int n = sw->write(root, &ss);
        if (n != 0)
        {
            spdlog::error("StreamWriter::write error...");
            return;
        }
        out = ss.str();
    }

    /*对string的内容反序列化*/
    static bool Deserialize(const std::string& in, Json::Value& root)
    {
        CRPtr cr(Json::CharReaderBuilder().newCharReader());
        Json::String err;
        bool n = cr->parse(in.c_str(), in.c_str() + in.size(), &root, &err);
        if (!n)
        {
            spdlog::error("CharReader::parse error...");
            return false;
        }
        return true;
    }
};