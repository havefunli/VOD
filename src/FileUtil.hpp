#pragma once
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <string>
#include <spdlog/spdlog.h>

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
    static void SetContent(const std::string& file_name, const std::string& out)
    {
        std::ofstream ofs(file_name, std::ios_base::binary);
        if (!ofs.is_open()) 
        { 
            spdlog::error("ofstream open failed...");
            return;
        }
        /*写入数据到文件*/
        ofs.write(out.c_str(), out.size());

        ofs.close();
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
};