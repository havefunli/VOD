#include <iostream>
#include <string>
#include <mysql/mysql.h>

const std::string LOCAL_LOOP = "0.0.0.0";
const std::string DEFAULT_USER = "lmy";
const std::string ROOT_PASSWD = "20040823@Lq";
const uint16_t DEFAULT_PORT = 3306;
const std::string DEFAULT_CHAR = "utf8";

MYSQL* mysql;

void WarnInfo()
{
    std::cerr << mysql_error(mysql) << std::endl;
    exit(-1);
}

int main()
{
    // 创建一个MySQL句柄
    mysql = mysql_init(nullptr);
    if (mysql == nullptr)
    {
        std::cerr << "MySQL Create error..." << std::endl;
    }
    // 连接到服务器
    MYSQL *ret = mysql_real_connect(mysql,
                                    LOCAL_LOOP.c_str(),
                                    DEFAULT_USER.c_str(),
                                    ROOT_PASSWD.c_str(),
                                    "School",
                                    DEFAULT_PORT,
                                    nullptr,
                                    0);
    if (ret == nullptr) { WarnInfo(); } 
    // 设置字符集
    int n = mysql_set_character_set(mysql, DEFAULT_CHAR.c_str());
    if (n != 0) { WarnInfo(); }
    // 插入语句
    // std::string sql_insert = "insert into Student (SID, NAME)"
    //                          "values (2, \'dyf\');";
    // n = mysql_query(mysql, sql_insert.c_str());
    // if (n != 0) { WarnInfo(); }
    // 查询语句
    std::string sql_query = "select * from Student;";
    n = mysql_query(mysql, sql_query.c_str());
    if (n != 0) { WarnInfo(); }
    
    MYSQL_RES* res = mysql_store_result(mysql);
    if (res == nullptr) { WarnInfo(); }
    int row = mysql_num_rows(res);
    int col = mysql_num_fields(res);
    for (int i = 0; i < row; i++)
    {
        MYSQL_ROW rw = mysql_fetch_row(res);
        for (int j = 0; j < col; j++)
        {
            std::cout << rw[j] << " ";
        }
        std::cout << std::endl;
    }


    return 0;
}