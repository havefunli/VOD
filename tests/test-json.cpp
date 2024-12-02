#include <iostream>
#include <string>
#include <sstream>
#include <jsoncpp/json/json.h>

struct Student
{
    Student(std::string name, float height, float weight)
        : _name(name), _height(height), _weight(weight)
    {}

    std::string _name;
    float       _height;
    float       _weight;

    std::string toJson()
    {
        Json::Value root;
        root["Name"]   = _name;
        root["Height"] = _height;
        root["Weight"] = _weight;

        Json::StreamWriterBuilder swb;
        Json::StreamWriter* sw = swb.newStreamWriter();
        std::stringstream out;
        sw->write(root, &out);
        return out.str();
    }

    void toStudent(const std::string stuStr)
    {
        Json::Value root;
        Json::CharReaderBuilder crb;
        Json::CharReader* cr = crb.newCharReader();
        
        std::string err;
        cr->parse(stuStr.c_str(), stuStr.c_str() + stuStr.length(), &root, &err);
        std::cout << root["Name"] << std::endl;
    }
}; 

int main()
{   
    Student s1("liquan", 1.68, 64);
    std::string test = "123456";
    std::cout << s1.toJson() << std::endl;
    std::cout << test.begin().base() << std::endl;
    std::cout << test.c_str() << std::endl;

    return 0;
}