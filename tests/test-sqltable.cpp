#include "../include/SQLTable.h"
#include <iostream>
/*'知识' - 1,
  '娱乐' - 2,
  '生活' - 3,
  '情感' - 4,
  '才艺' - 5,
  '剧情' - 6,
  '社会' - 7,
  '游戏' - 8
*/

SqlTable st;


/*测试插入数据*/
void test_1()
{
    Json::Value root;
    root["name"] = "加菲猫的幸福生活";
    root["description"] = "一只猫和主人和一只小狗的幸福生活";
    root["type"] = 2;
    root["video_path"] = "./";
    root["cover_path"] = "./";
    st.Insert(root);
}

/*测试更新数据*/
void test_2()
{
    Json::Value root;
    root["name"] = "海绵宝宝";
    root["description"] = "海底世界一群动物的生活";
    root["type"] = 2;
    st.Update(1, root);
}

/*查询数据*/
void test_3()
{
    Json::Value root;
    st.SelectAll(root);
    std::cout << root.toStyledString() << std::endl;
}

void test_4()
{
    Json::Value root;
    st.SelectOne(1, root);
    std::cout << root.toStyledString() << std::endl;
}

void test_5()
{
    Json::Value root;
    st.SelectLike("海绵宝宝", root);
    std::cout << root.toStyledString() << std::endl;
}

int main()
{
    // test_1();
    // test_2();
    // test_3();
    test_4();
    test_5();
    return 0;
}