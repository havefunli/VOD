#include <iostream>
#include "../src/Util.hpp"

void test_1()
{
    if (FileUtil::FileExists("test-json.cpp"))
    {
        std::cout << "FileExists" << std::endl;
    }
    std::cout << "test_1()" << std::endl;
}

void test_2()
{
    int n = FileUtil::FileSize("test-json.cpp");
    std::cout << "file size = " << n << std::endl;
    std::cout << "test_2()" << std::endl;
}

void test_3()
{
    std::string file;
    FileUtil::GetContent("test-json.cpp", file);
    std::cout << "file = " << file << std::endl;
    std::cout << "test_3()" << std::endl;
}

void test_4()
{
    FileUtil::SetContent("test.txt", "12345678\r\n");
}

void test_5()
{
    FileUtil::CreateDirectory("test");
}

void test_6()
{
    Json::Value root;
    root["Name"] = "李权";
    root["Age"]  = 20;
    root["Score"].append(100);
    root["Score"].append(99);
    root["Score"].append(98);

    std::string out;
    JsonUtil::Serialize(root, out);
    std::cout << out << std::endl;

    Json::Value val;
    JsonUtil::Deserialize(out, val);
    std::cout << val.toStyledString() << std::endl;
}

int main()
{
    // test_1();
    // test_2();
    // test_3();
    // test_4();
    // test_5();
    test_6();
    return 0;
}